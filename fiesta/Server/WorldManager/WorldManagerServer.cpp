// Server/WorldManager/WorldManagerServer.cpp
#include "WorldManagerServer.h"
#include "../Shared/ShineLogSystem.h"
#include "../Shared/GTimer.h"
#include "../Common/NETCOMMAND.h"
#include "../Common/SendPacket.h"
#include "../DataServer/Common/Database.h"
#include "../DataServer/Common/SQLP.h"
#include <string.h>

namespace fiesta {

// External admin (OperatorTool / admin panel) ODBC pool. Allocated lazily on
// the first authed OPTool session and torn down at WM shutdown.
static Database*      g_pkOPDB    = NULL;
static SQLP_Operator* g_pkSQLPOp  = NULL;

static void EnsureOpDB() {
    if (g_pkSQLPOp) return;
    g_pkOPDB = new Database();
    g_pkOPDB->Connect(
        "Driver={SQL Server Native Client 11.0};Server=.;Database=OperatorTool;Trusted_Connection=yes;");
    g_pkSQLPOp = new SQLP_Operator(g_pkOPDB);
}

void PartyFinderServer::Tick() {}
void RankingServer    ::Tick() {}
void ChatStealServer  ::Tick() {}

WorldManagerServer& WorldManagerServer::Get() { static WorldManagerServer s; return s; }

WorldManagerServer::WorldManagerServer()
    : m_pkPartyFinder(new PartyFinderServer()),
      m_pkRanking    (new RankingServer    ()),
      m_pkChatSteal  (new ChatStealServer  ()) {
    InitializeCriticalSection(&m_kCs);
}

void WorldManagerServer::RegisterZone(uint16 zid, const std::string& ip, uint16 port) {
    EnterCriticalSection(&m_kCs);
    ZoneInfo z; z.uiZoneId = zid; z.kIp = ip; z.uiPort = port; z.bAlive = true; z.uiLastHeartbeatMs = GTimer::NowMillis();
    m_kZones[zid] = z;
    LeaveCriticalSection(&m_kCs);
    SHINELOG_INFO("WM: Zone%02u registered at %s:%u", zid, ip.c_str(), port);
}

const ZoneInfo* WorldManagerServer::PickZoneForChar(CharID cid) const {
    if (m_kZones.empty()) return NULL;
    // Provisional: round-robin by char id. Replace with map-based dispatch in Zone enter.
    uint16 zid = (uint16)(cid % (CharID)m_kZones.size());
    std::map<uint16, ZoneInfo>::const_iterator it = m_kZones.find(zid);
    return (it == m_kZones.end()) ? NULL : &it->second;
}

void WorldManagerServer::OnTokenIssuedFromLogin(const PendingTokenAuth& tok) {
    EnterCriticalSection(&m_kCs);
    m_kPendingTokens.push_back(tok);
    LeaveCriticalSection(&m_kCs);
}

bool WorldManagerServer::ConsumeToken(AccountID aid, const uint8 aSecret[16]) {
    bool bOk = false;
    EnterCriticalSection(&m_kCs);
    for (size_t i = 0; i < m_kPendingTokens.size(); ++i) {
        if (m_kPendingTokens[i].aid == aid &&
            memcmp(m_kPendingTokens[i].aSecret, aSecret, 16) == 0) {
            m_kPendingTokens.erase(m_kPendingTokens.begin() + i);
            bOk = true; break;
        }
    }
    LeaveCriticalSection(&m_kCs);
    return bOk;
}

void WorldManagerServer::RegisterZoneSession(uint16 uiZoneId, IOCPSession* pkSess) {
    EnterCriticalSection(&m_kCs);
    m_kZoneSessions[uiZoneId] = pkSess;
    LeaveCriticalSection(&m_kCs);
}

void WorldManagerServer::UnregisterZoneSession(IOCPSession* pkSess) {
    EnterCriticalSection(&m_kCs);
    for (std::map<uint16, IOCPSession*>::iterator it = m_kZoneSessions.begin();
         it != m_kZoneSessions.end(); ++it) {
        if (it->second == pkSess) { m_kZoneSessions.erase(it); break; }
    }
    LeaveCriticalSection(&m_kCs);
}

void WorldManagerServer::BroadcastToZones(NCOpcode uiOp, const void* p, size_t n) {
    // Snapshot the session list so we don't hold the CS during sends.
    std::vector<IOCPSession*> snapshot;
    EnterCriticalSection(&m_kCs);
    for (std::map<uint16, IOCPSession*>::iterator it = m_kZoneSessions.begin();
         it != m_kZoneSessions.end(); ++it) {
        if (it->second) snapshot.push_back(it->second);
    }
    LeaveCriticalSection(&m_kCs);
    for (size_t i = 0; i < snapshot.size(); ++i) {
        SendPacket(snapshot[i], uiOp, p, n);
    }
}

bool WorldManagerServer::SendToZone(uint16 uiZoneId, NCOpcode uiOp, const void* p, size_t n) {
    IOCPSession* pkSess = NULL;
    EnterCriticalSection(&m_kCs);
    std::map<uint16, IOCPSession*>::iterator it = m_kZoneSessions.find(uiZoneId);
    if (it != m_kZoneSessions.end()) pkSess = it->second;
    LeaveCriticalSection(&m_kCs);
    if (!pkSess) return false;
    SendPacket(pkSess, uiOp, p, n);
    return true;
}

// --------------- WMClientSession ---------------
void WMClientSession::OnConnect()    { m_uiAccount = 0; m_uiActiveChar = 0; SHINELOG_INFO("WM: client connected"); }
void WMClientSession::OnDisconnect() { SHINELOG_INFO("WM: client disconnect aid=%u", m_uiAccount); }

void WMClientSession::OnPacket(const GPacket& rPkt) {
    switch (rPkt.GetOpcode()) {
        case NC_CHAR_LOGIN_REQ: {
            // body = [account_id u32][16-byte token secret]
            PacketBuffer b = rPkt.Body();
            uint32 aid = 0; b.ReadU32(aid);
            uint8  sec[16]; b.ReadBytes(sec, 16);
            if (!WorldManagerServer::Get().ConsumeToken(aid, sec)) {
                SHINELOG_WARN("WM: bad token aid=%u", aid);
                Close(); return;
            }
            m_uiAccount = aid;
            PacketBuffer ack; ack.WriteU8(1);
            SendPacket(this, NC_CHAR_LOGIN_ACK, ack.Data(), ack.Size());
            break;
        }
        case NC_CHAR_LOGOUT_CMD:
            Close(); break;
        default:
            SHINELOG_DEBUG("WM-client recv NC=0x%04X", rPkt.GetOpcode());
            break;
    }
}

// --------------- WMZoneSession ---------------
void WMZoneSession::OnConnect()    { m_uiZoneId = 0xFFFF; }
void WMZoneSession::OnDisconnect() {
    SHINELOG_INFO("WM: Zone%02u disconnected", m_uiZoneId);
    WorldManagerServer::Get().UnregisterZoneSession(this);
}

void WMZoneSession::OnPacket(const GPacket& rPkt) {
    switch (rPkt.GetOpcode()) {
        case NC_INTER_ZONE_REGISTER_REQ: {
            PacketBuffer b = rPkt.Body();
            uint16 zid = 0; b.ReadU16(zid);
            std::string ip; b.ReadString(ip);
            uint16 port = 0; b.ReadU16(port);
            m_uiZoneId = zid;
            WorldManagerServer::Get().RegisterZone(zid, ip, port);
            WorldManagerServer::Get().RegisterZoneSession(zid, this);
            PacketBuffer ack; ack.WriteU8(1); SendPacket(this, NC_INTER_HELLO_ACK, ack.Data(), ack.Size());
            break;
        }
        case NC_INTER_ZONE_HEARTBEAT_CMD:
            // touch alive
            break;
        default: break;
    }
}

// --------------- WMLoginSession ---------------
void WMLoginSession::OnPacket(const GPacket& rPkt) {
    if (rPkt.GetOpcode() == NC_INTER_AUTH_TOKEN_PUSH) {
        PendingTokenAuth tok;
        PacketBuffer b = rPkt.Body();
        uint32 aid = 0; uint16 wid = 0;
        b.ReadU32(aid); b.ReadU16(wid);
        b.ReadBytes(tok.aSecret, 16);
        tok.aid = aid; tok.uiWorldId = wid; tok.uiIssuedMs = GTimer::NowMillis();
        WorldManagerServer::Get().OnTokenIssuedFromLogin(tok);
    }
}

// --------------- WMCharDBSession ---------------
void WMCharDBSession::OnPacket(const GPacket& rPkt) {
    SHINELOG_DEBUG("WM<-CharDB NC=0x%04X", rPkt.GetOpcode());
}

// --------------- WMOPToolSession ---------------
// Loopback-only admin entry point. Drives jail / sysmsg / give-item /
// take-item / generic-query across the WM->Zone fanout and the SQLP_Operator
// facade. No engine subsystem depends on this session being present; if the
// admin tool never connects, gameplay is unaffected.
// Wire layout for each incoming packet body (kept simple ASCII so an
// out-of-process Win32 admin panel can produce them with `sprintf` + memcpy):
//   AUTH_REQ:       "<userid>\0<password>\0"
//   AUTH_ACK:       uint8 ok, uint8 level
//   BAN_CMD:        uint32 charNo, uint32 minutes, "<reason>\0"
//   KICK_CMD:       uint32 charNo, "<reason>\0"
//   JAIL_CMD:       uint32 charNo, uint32 minutes, "<reason>\0"
//   UNJAIL_CMD:     uint32 charNo
//   SYSMSG_CMD:     uint8 scope (0=world,1=all), "<text>\0"
//   GIVEITEM_CMD:   uint32 charNo, uint32 itemId, uint16 count
//   TAKEITEM_CMD:   uint64 itemKey
//   QUERY_REQ:      "<dbName>\0<sql>\0"   (read-only; SELECT only)
//   RESULT_ACK:     uint8 ok, "<msg>\0"
void WMOPToolSession::OnPacket(const GPacket& rPkt) {
    PacketBuffer body = rPkt.Body();

    // Anything other than AUTH_REQ requires a successful auth first.
    NCOpcode op = rPkt.GetOpcode();
    if (op != NC_OPTOOL_AUTH_REQ && !m_bAuthed) {
        PacketBuffer ack; ack.WriteU8(0); ack.WriteString("not authed");
        SendPacket(this, NC_OPTOOL_RESULT_ACK, ack.Data(), ack.Size());
        return;
    }

    switch (op) {
        case NC_OPTOOL_AUTH_REQ: {
            std::string id, pw;
            body.ReadString(id); body.ReadString(pw);
            EnsureOpDB();
            m_iOperLevel = 0;
            m_bAuthed = g_pkSQLPOp && g_pkSQLPOp->Logon(id, pw, m_iOperLevel);
            PacketBuffer ack; ack.WriteU8(m_bAuthed ? 1 : 0); ack.WriteU8((uint8)m_iOperLevel);
            SendPacket(this, NC_OPTOOL_AUTH_ACK, ack.Data(), ack.Size());
            if (m_bAuthed) SHINELOG_INFO("OPTool auth OK '%s' level=%d", id.c_str(), m_iOperLevel);
            else           SHINELOG_WARN("OPTool auth FAIL '%s'",       id.c_str());
            break;
        }

        case NC_OPTOOL_BAN_CMD: {
            uint32 cid = 0, mins = 0; std::string reason;
            body.ReadU32(cid); body.ReadU32(mins); body.ReadString(reason);
            SHINELOG_WARN("OPTool BAN  cid=%u %u min reason='%s' (level=%d)",
                          cid, mins, reason.c_str(), m_iOperLevel);
            // Fan out to every zone -- whichever currently hosts cid acts.
            PacketBuffer push; push.WriteU32(cid); push.WriteU32(mins); push.WriteString(reason);
            WorldManagerServer::Get().BroadcastToZones(NC_INTER_OPTOOL_BAN_PUSH, push.Data(), push.Size());
            PacketBuffer ack; ack.WriteU8(1); ack.WriteString("ban dispatched");
            SendPacket(this, NC_OPTOOL_RESULT_ACK, ack.Data(), ack.Size());
            break;
        }
        case NC_OPTOOL_KICK_CMD: {
            uint32 cid = 0; std::string reason;
            body.ReadU32(cid); body.ReadString(reason);
            SHINELOG_WARN("OPTool KICK cid=%u reason='%s' (level=%d)",
                          cid, reason.c_str(), m_iOperLevel);
            PacketBuffer push; push.WriteU32(cid); push.WriteString(reason);
            WorldManagerServer::Get().BroadcastToZones(NC_INTER_OPTOOL_KICK_PUSH, push.Data(), push.Size());
            PacketBuffer ack; ack.WriteU8(1); ack.WriteString("kick dispatched");
            SendPacket(this, NC_OPTOOL_RESULT_ACK, ack.Data(), ack.Size());
            break;
        }
        case NC_OPTOOL_JAIL_CMD: {
            uint32 cid = 0, mins = 0; std::string reason;
            body.ReadU32(cid); body.ReadU32(mins); body.ReadString(reason);
            SHINELOG_WARN("OPTool JAIL cid=%u %u min reason='%s' (level=%d)",
                          cid, mins, reason.c_str(), m_iOperLevel);
            PacketBuffer push; push.WriteU32(cid); push.WriteU32(mins); push.WriteString(reason);
            WorldManagerServer::Get().BroadcastToZones(NC_INTER_OPTOOL_JAIL_PUSH, push.Data(), push.Size());
            PacketBuffer ack; ack.WriteU8(1); ack.WriteString("jail dispatched");
            SendPacket(this, NC_OPTOOL_RESULT_ACK, ack.Data(), ack.Size());
            break;
        }
        case NC_OPTOOL_UNJAIL_CMD: {
            uint32 cid = 0; body.ReadU32(cid);
            SHINELOG_WARN("OPTool UNJAIL cid=%u (level=%d)", cid, m_iOperLevel);
            PacketBuffer push; push.WriteU32(cid);
            WorldManagerServer::Get().BroadcastToZones(NC_INTER_OPTOOL_UNJAIL_PUSH, push.Data(), push.Size());
            PacketBuffer ack; ack.WriteU8(1); ack.WriteString("unjail dispatched");
            SendPacket(this, NC_OPTOOL_RESULT_ACK, ack.Data(), ack.Size());
            break;
        }
        case NC_OPTOOL_SYSMSG_CMD: {
            uint8 scope = 0; std::string text;
            body.ReadU8(scope); body.ReadString(text);
            SHINELOG_INFO("OPTool SYSMSG scope=%u text='%s'", (uint32)scope, text.c_str());
            PacketBuffer push; push.WriteU8(scope); push.WriteString(text);
            WorldManagerServer::Get().BroadcastToZones(NC_INTER_OPTOOL_SYSMSG_PUSH, push.Data(), push.Size());
            PacketBuffer ack; ack.WriteU8(1); ack.WriteString("sysmsg dispatched");
            SendPacket(this, NC_OPTOOL_RESULT_ACK, ack.Data(), ack.Size());
            break;
        }
        case NC_OPTOOL_GIVEITEM_CMD: {
            uint32 cid = 0, item = 0; uint16 cnt = 1;
            body.ReadU32(cid); body.ReadU32(item); body.ReadU16(cnt);
            SHINELOG_WARN("OPTool GIVEITEM cid=%u item=%u count=%u (level=%d)",
                          cid, item, (uint32)cnt, m_iOperLevel);
            PacketBuffer push; push.WriteU32(cid); push.WriteU32(item); push.WriteU16(cnt);
            WorldManagerServer::Get().BroadcastToZones(NC_INTER_OPTOOL_GIVEITEM_PUSH, push.Data(), push.Size());
            PacketBuffer ack; ack.WriteU8(1); ack.WriteString("give dispatched");
            SendPacket(this, NC_OPTOOL_RESULT_ACK, ack.Data(), ack.Size());
            break;
        }
        case NC_OPTOOL_TAKEITEM_CMD: {
            uint32 lo = 0, hi = 0;
            body.ReadU32(lo); body.ReadU32(hi);
            uint64 itemKey = ((uint64)hi << 32) | (uint64)lo;
            SHINELOG_WARN("OPTool TAKEITEM key=%llu (level=%d)",
                          (unsigned long long)itemKey, m_iOperLevel);
            PacketBuffer push; push.WriteU32(lo); push.WriteU32(hi);
            WorldManagerServer::Get().BroadcastToZones(NC_INTER_OPTOOL_TAKEITEM_PUSH, push.Data(), push.Size());
            PacketBuffer ack; ack.WriteU8(1); ack.WriteString("take dispatched");
            SendPacket(this, NC_OPTOOL_RESULT_ACK, ack.Data(), ack.Size());
            break;
        }
        case NC_OPTOOL_QUERY_REQ: {
            std::string dbName, sql;
            body.ReadString(dbName); body.ReadString(sql);
            // SELECT-only enforcement is the panel's responsibility; here we
            // simply log + reject anything that begins with a non-SELECT verb
            // as a final safety net.
            const char* p = sql.c_str();
            while (*p == ' ' || *p == '\t' || *p == '\r' || *p == '\n') ++p;
            bool bSelect = (p[0] == 'S' || p[0] == 's')
                        && (p[1] == 'E' || p[1] == 'e')
                        && (p[2] == 'L' || p[2] == 'l')
                        && (p[3] == 'E' || p[3] == 'e')
                        && (p[4] == 'C' || p[4] == 'c')
                        && (p[5] == 'T' || p[5] == 't');
            PacketBuffer ack;
            if (!bSelect) {
                ack.WriteU8(0); ack.WriteString("only SELECT permitted");
            } else {
                SHINELOG_INFO("OPTool QUERY db=%s sql='%.120s%s' (level=%d)",
                              dbName.c_str(), sql.c_str(),
                              sql.size() > 120 ? "..." : "", m_iOperLevel);
                ack.WriteU8(1); ack.WriteString("query queued -- relay live in pass 2");
            }
            SendPacket(this, NC_OPTOOL_QUERY_ACK, ack.Data(), ack.Size());
            break;
        }
        default:
            SHINELOG_WARN("OPTool: unknown NC=0x%04X", (uint32)op);
            break;
    }
}

} // namespace fiesta
