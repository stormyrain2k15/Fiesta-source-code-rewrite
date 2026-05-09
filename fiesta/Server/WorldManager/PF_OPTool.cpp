// Server/WorldManager/PF_OPTool.cpp
// Protocol family: WM <-> OperatorTool admin panel. Loopback-only listener,
// per-session auth via SQLP_Operator (p_Operator_Logon), then ban / kick /
// jail / unjail / sysmsg / give-item / take-item / read-only SELECT relay.
// Each authoritative action is fanned out to every connected zone via
// NC_INTER_OPTOOL_*_PUSH so whichever zone hosts the target charNo can act.
#include "WorldManagerServer.h"
#include "../Shared/ShineLogSystem.h"
#include "../Common/NETCOMMAND.h"
#include "../Common/SendPacket.h"
#include "../DataServer/Common/Database.h"
#include "../DataServer/Common/SQLP.h"
#include <map>
#include <string>

namespace fiesta {

// External admin (OperatorTool) ODBC handle. Lazily allocated on first
// successful AUTH_REQ, kept open for the lifetime of the WM process.
static Database*      g_pkOPDB    = NULL;
static SQLP_Operator* g_pkSQLPOp  = NULL;

static void EnsureOpDB() {
    if (g_pkSQLPOp) return;
    g_pkOPDB = new Database();
    g_pkOPDB->Connect(
        "Driver={SQL Server Native Client 11.0};Server=.;Database=OperatorTool;Trusted_Connection=yes;");
    g_pkSQLPOp = new SQLP_Operator(g_pkOPDB);
}

// Per-named-DB ODBC pool used by the OPTool generic-SELECT relay.
static std::map<std::string, Database*> g_kOPQueryDBs;

static Database* OPQueryDB(const std::string& rDbName) {
    std::map<std::string, Database*>::iterator it = g_kOPQueryDBs.find(rDbName);
    if (it != g_kOPQueryDBs.end()) return it->second;
    Database* d = new Database();
    char conn[256];
    _snprintf(conn, sizeof(conn),
        "Driver={SQL Server Native Client 11.0};Server=.;Database=%s;Trusted_Connection=yes;",
        rDbName.c_str());
    if (!d->Connect(conn)) { delete d; return NULL; }
    g_kOPQueryDBs[rDbName] = d;
    return d;
}

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
                Database* pDb = OPQueryDB(dbName);
                if (!pDb) {
                    ack.WriteU8(0); ack.WriteString("db connect failed");
                } else {
                    std::vector<DBRecord> rows;
                    bool ok = pDb->Query(sql, rows);
                    SHINELOG_INFO("OPTool QUERY db=%s rows=%u ok=%d (level=%d)",
                                  dbName.c_str(), (uint32)rows.size(),
                                  ok ? 1 : 0, m_iOperLevel);
                    ack.WriteU8(ok ? 1 : 0);
                    ack.WriteU16((uint16)rows.size());
                    for (size_t i = 0; i < rows.size(); ++i) {
                        ack.WriteU16((uint16)rows[i].kCols.size());
                        for (size_t j = 0; j < rows[i].kCols.size(); ++j)
                            ack.WriteString(rows[i].kCols[j]);
                    }
                }
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
