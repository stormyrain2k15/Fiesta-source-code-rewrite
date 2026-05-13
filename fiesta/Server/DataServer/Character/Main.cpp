// Server/DataServer/Character/Main.cpp
// Character DB service exe entry. Hosts every World00_Character SQLP
// module; the Wedding / HolyPromise / Guild / Item / Quest / Skill / Friend
// helpers all share the same physical ODBC connection.
// Inter-server protocol served:
//   NC_INTER_CHAR_DB_QUERY  -- WM-side request: { uint8 op, uint32 charId, ... }
//                              op = 1 (CharLogin), 2 (CharLogout)
//   NC_INTER_CHAR_DB_RESPONSE -- reply: { uint8 op, uint8 ok, repeated row text columns NUL-terminated }
#include "../../Shared/WinService.h"
#include "../../Shared/ServerInfo.h"
#include "../../Shared/ShineLogSystem.h"
#include "../../Shared/IOCPManager.h"
#include "../../Shared/Socket_Acceptor.h"
#include "../../Shared/PacketBuffer.h"
#include "../../Common/NETCOMMAND.h"
#include "../../Common/SendPacket.h"
#include "../Common/Database.h"
#include "../Common/SQLP.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdarg.h>

namespace shine {

// sprintf-style proc-arg builder, bounded to 256 bytes (matches the
// internal helper in SQLP.cpp). VS2010-safe: uses _vsnprintf_s under
// MSVC to avoid the deprecation warning + truncation hazard of the
// stock vsnprintf signature in the v100 CRT.
static std::string F(const char* fmt, ...) {
    char buf[256];
    va_list ap; va_start(ap, fmt);
#if defined(_MSC_VER)
    _vsnprintf_s(buf, sizeof(buf), _TRUNCATE, fmt, ap);
#else
    vsnprintf(buf, sizeof(buf), fmt, ap);
#endif
    va_end(ap);
    buf[sizeof(buf)-1] = '\0';
    return std::string(buf);
}

static Database*         g_pkCharDB    = NULL;
static SQLP_Character*   g_pkSQLPChar  = NULL;
static SQLP_Wedding*     g_pkSQLPWed   = NULL;
static SQLP_HolyPromise* g_pkSQLPHoly  = NULL;
static SQLP_Guild*       g_pkSQLPGuild = NULL;
static SQLP_Item*        g_pkSQLPItem  = NULL;
static SQLP_Quest*       g_pkSQLPQuest = NULL;
static SQLP_Skill*       g_pkSQLPSkill = NULL;
static SQLP_Friend*      g_pkSQLPFr    = NULL;
static SQLP_Estate*      g_pkSQLPEst   = NULL;

enum CharDBOp {
    CHARDB_OP_LOGIN              = 1,
    CHARDB_OP_LOGOUT             = 2,
    // Wedding (SQLP_Wedding)
    CHARDB_OP_WED_PROPOSE        = 10,
    CHARDB_OP_WED_CANCEL_PROPOSE = 11,
    CHARDB_OP_WED_DO             = 12,
    CHARDB_OP_WED_DIVORCE        = 13,
    // HolyPromise (SQLP_HolyPromise)
    CHARDB_OP_HP_SET             = 20,
    CHARDB_OP_HP_SETDATE         = 21,
    CHARDB_OP_HP_DELCHAR         = 22,
    // Estate (SQLP_Estate)
    CHARDB_OP_EST_CREATE         = 30,
    CHARDB_OP_EST_DEMOLISH       = 31,
    CHARDB_OP_EST_SAVE           = 32,
    CHARDB_OP_EST_LOAD           = 33,
    // Item (SQLP_Item)
    CHARDB_OP_ITEM_CREATE        = 40,   // (cOwner, itemId, count, slot, endure, inxName)
    CHARDB_OP_ITEM_DELETE        = 41,   // (itemKey)
    CHARDB_OP_ITEM_SETOPT        = 42,   // (itemKey, optType, optData)
    // Quest (SQLP_Quest)
    CHARDB_OP_QUEST_SET          = 50,   // (cid, questNo, status, subStatus)
    CHARDB_OP_QUEST_GETDOING     = 51,   // (cid)  -- replies with row vector
    // Skill (SQLP_Skill)
    CHARDB_OP_SKILL_SETPOWER     = 60,   // (cid, skillNo, slot, value, isActive)
    CHARDB_OP_SKILL_SETPOWERALL  = 61,   // (cid)
    // Friend (SQLP_Friend)
    CHARDB_OP_FRIEND_DELALL      = 70,   // (cid)
    // Guild (SQLP_Guild)
    CHARDB_OP_GUILD_TOURNSET     = 80    // (gtNo, guildNo, status)
};

static void HandleCharLogin(IOCPSession* pkSrc, const GPacket& rPkt) {
    PacketBuffer body = rPkt.Body();
    uint8 op = 0; uint32 cid = 0;
    body.ReadU8(op); body.ReadU32(cid);

    PacketBuffer ack;
    ack.WriteU8(op);

    if (!g_pkSQLPChar) {
        ack.WriteU8(0); // ok=false
        SendPacket(pkSrc, NC_INTER_CHAR_DB_RESPONSE, ack.Data(), ack.Size());
        return;
    }
    std::vector<DBRecord> rows;
    bool bOK = g_pkSQLPChar->Login((CharID)cid, rows) && !rows.empty();
    ack.WriteU8(bOK ? 1 : 0);
    if (bOK) {
        const DBRecord& r = rows[0];
        ack.WriteU16((uint16)r.kCols.size());
        for (size_t i = 0; i < r.kCols.size(); ++i) {
            ack.WriteString(r.kCols[i]);
        }
    }
    SendPacket(pkSrc, NC_INTER_CHAR_DB_RESPONSE, ack.Data(), ack.Size());
    SHINELOG_INFO("CharDB: login cid=%u -> %s (%u cols)",
                  cid, bOK ? "OK" : "FAIL",
                  (uint32)(bOK ? rows[0].kCols.size() : 0));
}

static void HandleCharLogout(IOCPSession* pkSrc, const GPacket& rPkt) {
    PacketBuffer body = rPkt.Body();
    uint8 op = 0; uint32 cid = 0;
    body.ReadU8(op); body.ReadU32(cid);
    bool bOK = g_pkSQLPChar && g_pkSQLPChar->Logout((CharID)cid);
    PacketBuffer ack; ack.WriteU8(op); ack.WriteU8(bOK ? 1 : 0);
    SendPacket(pkSrc, NC_INTER_CHAR_DB_RESPONSE, ack.Data(), ack.Size());
}

// Marriage / HolyPromise / Estate dispatcher. Most calls are
// fire-and-forget (no return body); the only query is Estate_Load which
// echoes the row vector back as a column-text response.
static void HandleSocialOp(IOCPSession* pkSrc, const GPacket& rPkt, uint8 op) {
    PacketBuffer body = rPkt.Body();
    uint8 dummy = 0; body.ReadU8(dummy);
    uint32 a = 0; body.ReadU32(a);
    bool   bOK = false;

    switch (op) {
    case CHARDB_OP_WED_PROPOSE:        { uint32 b = 0; body.ReadU32(b); bOK = g_pkSQLPWed  && g_pkSQLPWed->Propose      ((CharID)a, (CharID)b); break; }
    case CHARDB_OP_WED_CANCEL_PROPOSE: { uint32 b = 0; body.ReadU32(b); bOK = g_pkSQLPWed  && g_pkSQLPWed->CancelPropose((CharID)a, (CharID)b); break; }
    case CHARDB_OP_WED_DO:             { uint32 b = 0; body.ReadU32(b); bOK = g_pkSQLPWed  && g_pkSQLPWed->Do           ((CharID)a, (CharID)b); break; }
    case CHARDB_OP_WED_DIVORCE:        { bOK = g_pkSQLPWed  && g_pkSQLPWed ->DivorceDo  ((CharID)a); break; }
    case CHARDB_OP_HP_SET:             { uint32 b = 0; body.ReadU32(b); bOK = g_pkSQLPHoly && g_pkSQLPHoly->Set         ((CharID)a, (CharID)b); break; }
    case CHARDB_OP_HP_SETDATE:         { bOK = g_pkSQLPHoly && g_pkSQLPHoly->SetDate    ((CharID)a); break; }
    case CHARDB_OP_HP_DELCHAR:         { bOK = g_pkSQLPHoly && g_pkSQLPHoly->DelChar    ((CharID)a); break; }
    case CHARDB_OP_EST_CREATE:         {
        uint32 hid = 0, tier = 0;
        body.ReadU32(hid); body.ReadU32(tier);
        bOK = g_pkSQLPEst && g_pkSQLPEst->Create((CharID)a, hid, tier);
        break;
    }
    case CHARDB_OP_EST_DEMOLISH:       { bOK = g_pkSQLPEst && g_pkSQLPEst->Demolish((CharID)a); break; }
    case CHARDB_OP_EST_SAVE:           {
        uint32 uiLen = 0; body.ReadU32(uiLen);
        size_t avail = body.Size() - body.ReadCursor();
        if (uiLen > avail) { bOK = false; break; }
        const uint8* pData = body.Data() + body.ReadCursor();
        bOK = g_pkSQLPEst && g_pkSQLPEst->Save((CharID)a, pData, uiLen);
        break;
    }
    case CHARDB_OP_EST_LOAD: {
        std::vector<DBRecord> rows;
        bOK = g_pkSQLPEst && g_pkSQLPEst->Load((CharID)a, rows);
        PacketBuffer ack; ack.WriteU8(op); ack.WriteU8(bOK ? 1 : 0);
        ack.WriteU16((uint16)rows.size());
        for (size_t i = 0; i < rows.size(); ++i) {
            ack.WriteU16((uint16)rows[i].kCols.size());
            for (size_t j = 0; j < rows[i].kCols.size(); ++j)
                ack.WriteString(rows[i].kCols[j]);
        }
        SendPacket(pkSrc, NC_INTER_CHAR_DB_RESPONSE, ack.Data(), ack.Size());
        return;     // already replied
    }
    case CHARDB_OP_ITEM_CREATE: {
        // a = owner CharID. Body: itemId / count / slot / storage / endure / inx
        // The Item_Create proc returns the new ItemKey via the &out parameter;
        // we relay it back so the Zone-side Inventory can store it for the
        // matching Item_Delete / Item_SetOption calls.
        uint32 itemId = 0; uint16 count = 0; uint16 slot = 0; uint16 endure = 0;
        uint8  storage = 0; std::string inx;
        body.ReadU32(itemId); body.ReadU16(count); body.ReadU16(slot);
        body.ReadU8 (storage); body.ReadU16(endure); body.ReadString(inx);
        uint64 keyOut = 0;
        bOK = g_pkSQLPItem &&
              g_pkSQLPItem->Create((CharID)a, itemId, count, storage, slot, keyOut);
        PacketBuffer ack;
        ack.WriteU8 (op); ack.WriteU8(bOK ? 1 : 0);
        ack.WriteU32((uint32)(keyOut & 0xFFFFFFFFu));
        ack.WriteU32((uint32)(keyOut >> 32));
        SendPacket(pkSrc, NC_INTER_CHAR_DB_RESPONSE, ack.Data(), ack.Size());
        return;
    }
    case CHARDB_OP_ITEM_DELETE: {
        uint32 hi = 0; body.ReadU32(hi);
        uint64 itemKey = ((uint64)hi << 32) | (uint64)a;
        bOK = g_pkSQLPItem && g_pkSQLPItem->Delete(itemKey);
        break;
    }
    case CHARDB_OP_ITEM_SETOPT: {
        uint32 hi = 0; uint8 type = 0; int32 data = 0;
        body.ReadU32(hi); body.ReadU8(type); body.ReadI32(data);
        uint64 itemKey = ((uint64)hi << 32) | (uint64)a;
        bOK = g_pkSQLPItem && g_pkSQLPItem->SetOption(itemKey, type, data);
        break;
    }
    case CHARDB_OP_QUEST_SET: {
        uint32 questNo = 0; int32 status = 0, sub = 0;
        body.ReadU32(questNo); body.ReadI32(status); body.ReadI32(sub);
        bOK = g_pkSQLPQuest && g_pkSQLPQuest->Set((CharID)a, questNo, status, sub);
        break;
    }
    case CHARDB_OP_QUEST_GETDOING: {
        std::vector<DBRecord> rows;
        bOK = g_pkSQLPQuest && g_pkSQLPQuest->GetAllDoing((CharID)a, rows);
        PacketBuffer ack; ack.WriteU8(op); ack.WriteU8(bOK ? 1 : 0);
        ack.WriteU16((uint16)rows.size());
        for (size_t i = 0; i < rows.size(); ++i) {
            ack.WriteU16((uint16)rows[i].kCols.size());
            for (size_t j = 0; j < rows[i].kCols.size(); ++j)
                ack.WriteString(rows[i].kCols[j]);
        }
        SendPacket(pkSrc, NC_INTER_CHAR_DB_RESPONSE, ack.Data(), ack.Size());
        return;
    }
    case CHARDB_OP_SKILL_SETPOWER: {
        // p_Skill_SetPower(cid, skillNo, slot, value, damage, cool, keep, sp)
        uint32 sk = 0; int32 slot = 0, val = 0, dmg = 0, cool = 0, keep = 0, sp = 0;
        body.ReadU32(sk); body.ReadI32(slot); body.ReadI32(val);
        body.ReadI32(dmg); body.ReadI32(cool); body.ReadI32(keep); body.ReadI32(sp);
        bOK = g_pkSQLPSkill && g_pkSQLPSkill->SetPower((CharID)a, sk, slot, val,
                                                       dmg, cool, keep, sp);
        break;
    }
    case CHARDB_OP_SKILL_SETPOWERALL:
        bOK = g_pkSQLPSkill && g_pkSQLPSkill->SetPowerAll((CharID)a);
        break;
    case CHARDB_OP_FRIEND_DELALL:
        bOK = g_pkSQLPFr && g_pkSQLPFr->DelAll((CharID)a);
        break;
    case CHARDB_OP_GUILD_TOURNSET: {
        uint32 guildNo = 0; int32 status = 0;
        body.ReadU32(guildNo); body.ReadI32(status);
        bOK = g_pkSQLPGuild && g_pkSQLPGuild->TournamentSet(a, guildNo, status);
        break;
    }
    // ---- WM-owned cross-zone ops (90+) ---------------------------------
    case 90 /*GUILD_CREATE*/: {
        std::string name; body.ReadString(name);
        uint32 newGid = 0;
        bOK = g_pkSQLPGuild && g_pkSQLPGuild->Create((CharID)a, name, newGid);
        PacketBuffer ack; ack.WriteU8(op); ack.WriteU8(bOK ? 1 : 0); ack.WriteU32(newGid);
        SendPacket(pkSrc, NC_INTER_CHAR_DB_RESPONSE, ack.Data(), ack.Size());
        return;
    }
    case 91 /*GUILD_DISSOLVE*/:
        bOK = g_pkSQLPGuild && g_pkSQLPGuild->Dissolve(a); break;
    case 92 /*GUILD_ADD_MEMBER*/: {
        uint32 c = 0; uint8 r = 0; body.ReadU32(c); body.ReadU8(r);
        bOK = g_pkSQLPGuild && g_pkSQLPGuild->AddMember(a, c, r); break;
    }
    case 93 /*GUILD_DEL_MEMBER*/: {
        uint32 c = 0; body.ReadU32(c);
        bOK = g_pkSQLPGuild && g_pkSQLPGuild->DelMember(a, c); break;
    }
    case 94 /*GUILD_SET_RANK*/: {
        uint32 c = 0; uint8 r = 0; body.ReadU32(c); body.ReadU8(r);
        bOK = g_pkSQLPGuild && g_pkSQLPGuild->SetRank(a, c, r); break;
    }
    case 95 /*GUILD_ADD_FUNDS*/: {
        int64 d = 0; body.ReadI64(d);
        bOK = g_pkSQLPGuild && g_pkSQLPGuild->AddFunds(a, d); break;
    }
    case 96 /*GUILD_SET_EMBLEM*/: {
        uint32 k = 0; body.ReadU32(k);
        bOK = g_pkSQLPGuild && g_pkSQLPGuild->SetEmblem(a, k); break;
    }
    case 97 /*GUILD_WAR_BEGIN*/: {
        uint32 d = 0; body.ReadU32(d);
        bOK = g_pkSQLPGuild && g_pkSQLPGuild->WarBegin(a, d); break;
    }
    case 98 /*GUILD_WAR_KILL*/: {
        uint32 d = 0, kr = 0, kd = 0;
        body.ReadU32(d); body.ReadU32(kr); body.ReadU32(kd);
        bOK = g_pkSQLPGuild && g_pkSQLPGuild->WarKill(a, d, kr, kd); break;
    }
    case 99 /*GUILD_WAR_END*/: {
        uint32 d = 0, w = 0; body.ReadU32(d); body.ReadU32(w);
        bOK = g_pkSQLPGuild && g_pkSQLPGuild->WarEnd(a, d, w); break;
    }
    case 100 /*GUILD_ACADEMY_JOIN*/: {
        uint32 c = 0; body.ReadU32(c);
        bOK = g_pkSQLPGuild && g_pkSQLPGuild->AcademyJoin(a, c); break;
    }
    case 101 /*GUILD_ACADEMY_LEAVE*/: {
        uint32 c = 0; body.ReadU32(c);
        bOK = g_pkSQLPGuild && g_pkSQLPGuild->AcademyLeave(a, c); break;
    }
    case 102 /*GUILD_TOURN_SET*/: {
        uint32 g = 0; int32 s = 0; body.ReadU32(g); body.ReadI32(s);
        bOK = g_pkSQLPGuild && g_pkSQLPGuild->TournamentSet(a, g, s); break;
    }
    case 110 /*FRIEND_ADD*/: {
        uint32 b2 = 0; body.ReadU32(b2);
        bOK = g_pkCharDB && g_pkCharDB->ExecProc("p_Friend_Add",
                                         F("%u,%u", a, b2));
        break;
    }
    case 111 /*FRIEND_DEL*/: {
        uint32 b2 = 0; body.ReadU32(b2);
        bOK = g_pkCharDB && g_pkCharDB->ExecProc("p_Friend_Del",
                                         F("%u,%u", a, b2));
        break;
    }
    case 120 /*MAIL_SEND*/: {
        uint32 to = 0; std::string title, msg; int64 attach = 0;
        body.ReadU32(to); body.ReadString(title); body.ReadString(msg); body.ReadI64(attach);
        bOK = g_pkCharDB && g_pkCharDB->ExecProc("p_Mail_Send",
            F("%u,%u,'%s','%s',%lld", a, to,
              Database::Quote(title).c_str(),
              Database::Quote(msg).c_str(), attach));
        break;
    }
    case 130 /*KQ_RESULT_SET*/: {
        uint32 winK = 0; body.ReadU32(winK);
        bOK = g_pkCharDB && g_pkCharDB->ExecProc("p_KQ_Result_Set", F("%u,%u", a, winK));
        break;
    }
    case 140 /*EVENT_ATTEND*/: {
        uint8 day = 0; body.ReadU8(day);
        bOK = g_pkCharDB && g_pkCharDB->ExecProc("p_EventAttendance_Mark",
                                         F("%u,%u", a, (uint32)day));
        break;
    }
    case 150 /*DAILY_RESET*/:
        bOK = g_pkCharDB && g_pkCharDB->ExecProc("p_Daily_Reset", F("%u", a));
        break;
    case 160 /*RANKING_PUBLISH*/: {
        // Ranking is bulk; we just stamp the run-id on tRankingHistory and
        // let the proc rebuild from tCharacter / tGuild snapshots.
        bOK = g_pkCharDB && g_pkCharDB->ExecProc("p_Ranking_Publish", F("%u", a));
        break;
    }
    default: SHINELOG_WARN("CharDB: unknown social op=%u", (uint32)op); return;
    }
    SHINELOG_DEBUG("CharDB op=%u cid=%u -> %s", (uint32)op, a, bOK ? "OK" : "FAIL");
    (void)pkSrc;     // fire-and-forget for ops other than EST_LOAD
}

class CharDBSession : public IOCPSession {
public:
    virtual void OnPacket(const GPacket& rPkt) {
        switch (rPkt.GetOpcode()) {
            case NC_INTER_CHAR_DB_QUERY: {
                // Peek op without consuming
                PacketBuffer body = rPkt.Body();
                if (body.Size() < 1) break;
                uint8 op = body.Data()[0];
                if      (op == CHARDB_OP_LOGIN)  HandleCharLogin (this, rPkt);
                else if (op == CHARDB_OP_LOGOUT) HandleCharLogout(this, rPkt);
                else if (op >= 10 && op <= 80)   HandleSocialOp  (this, rPkt, op);
                else if (op >= 90 && op <= 165)  HandleSocialOp  (this, rPkt, op);
                else SHINELOG_WARN("CharDB: unknown op=%u", (uint32)op);
                break;
            }
            default:
                SHINELOG_DEBUG("CharDB recv NC=0x%04X (no handler)", rPkt.GetOpcode());
                break;
        }
    }
};
static IOCPSession* MakeCharDBSession() { return new CharDBSession(); }

class CharacterDBService : public WinService {
public:
    CharacterDBService() : WinService("ShineCharacterDB") {}
    virtual bool OnStart() {
        m_kInfo.Load("ServerInfo.txt");
        g_pkCharDB = new Database();
        g_pkCharDB->Connect(m_kInfo.GetString("Character.ConnStr",
            "Driver={SQL Server Native Client 11.0};Server=.;Database=World00_Character;Trusted_Connection=yes;"));
        g_pkSQLPChar  = new SQLP_Character  (g_pkCharDB);
        g_pkSQLPWed   = new SQLP_Wedding    (g_pkCharDB);
        g_pkSQLPHoly  = new SQLP_HolyPromise(g_pkCharDB);
        g_pkSQLPGuild = new SQLP_Guild      (g_pkCharDB);
        g_pkSQLPItem  = new SQLP_Item       (g_pkCharDB);
        g_pkSQLPQuest = new SQLP_Quest      (g_pkCharDB);
        g_pkSQLPSkill = new SQLP_Skill      (g_pkCharDB);
        g_pkSQLPFr    = new SQLP_Friend     (g_pkCharDB);
        g_pkSQLPEst   = new SQLP_Estate     (g_pkCharDB);
        if (!m_kIOCP.Start()) return false;
        return m_kAcceptor.Start(&m_kIOCP, m_kInfo.GetU16("Character.Port", 27602),
                                 &MakeCharDBSession);
    }
    virtual void OnStop() {
        m_kAcceptor.Stop(); m_kIOCP.Stop();
        delete g_pkSQLPEst;   g_pkSQLPEst   = NULL;
        delete g_pkSQLPFr;    g_pkSQLPFr    = NULL;
        delete g_pkSQLPSkill; g_pkSQLPSkill = NULL;
        delete g_pkSQLPQuest; g_pkSQLPQuest = NULL;
        delete g_pkSQLPItem;  g_pkSQLPItem  = NULL;
        delete g_pkSQLPGuild; g_pkSQLPGuild = NULL;
        delete g_pkSQLPHoly;  g_pkSQLPHoly  = NULL;
        delete g_pkSQLPWed;   g_pkSQLPWed   = NULL;
        delete g_pkSQLPChar;  g_pkSQLPChar  = NULL;
        if (g_pkCharDB) { g_pkCharDB->Disconnect(); delete g_pkCharDB; g_pkCharDB = NULL; }
    }
private:
    ServerInfo m_kInfo; IOCPManager m_kIOCP; Socket_Acceptor m_kAcceptor;
};

} // namespace shine

int main(int argc, char** argv) { shine::CharacterDBService s; return s.Run(argc, argv); }
