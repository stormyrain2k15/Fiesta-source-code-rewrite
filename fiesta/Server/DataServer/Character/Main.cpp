// Server/DataServer/Character/Main.cpp
// 03 -- Character DB service exe entry. Hosts every World00_Character SQLP
// module; the Wedding / HolyPromise / Guild / Item / Quest / Skill / Friend
// helpers all share the same physical ODBC connection.
//
// Inter-server protocol served:
//   NC_INTER_CHAR_DB_QUERY  -- WM-side request: { uint8 op, uint32 charId, ... }
//                              op = 1 (CharLogin), 2 (CharLogout)
//   NC_INTER_CHAR_DB_RESPONSE -- reply: { uint8 op, uint8 ok, repeated row text columns NUL-terminated }
//
// EVIDENCE: PDB_CONFIRMED  symbol: WMCharDBSession, GameDBSession
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

namespace fiesta {

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
    CHARDB_OP_EST_LOAD           = 33
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
                else if (op >= 10 && op <= 33)   HandleSocialOp  (this, rPkt, op);
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
    CharacterDBService() : WinService("FiestaCharacterDB") {}
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

} // namespace fiesta

int main(int argc, char** argv) { fiesta::CharacterDBService s; return s.Run(argc, argv); }
