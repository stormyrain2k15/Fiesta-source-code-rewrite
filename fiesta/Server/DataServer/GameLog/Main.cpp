// Server/DataServer/GameLog/Main.cpp
// GameLog DB service exe entry. Records combat / drop / trade events into
// the World00_GameLog database via SQLP_GameLog. Reports are routed
// through SQLP_Report.
#include "../../Shared/WinService.h"
#include "../../Shared/ServerInfo.h"
#include "../../Shared/ShineLogSystem.h"
#include "../../Shared/IOCPManager.h"
#include "../../Shared/Socket_Acceptor.h"
#include "../../Shared/PacketBuffer.h"
#include "../../Shared/GPacket.h"
#include "../../Common/NETCOMMAND.h"
#include "../Common/Database.h"
#include "../Common/SQLP.h"

namespace fiesta {

static Database*     g_pkGameLogDB = NULL;
static SQLP_GameLog* g_pkSQLPGL    = NULL;
static SQLP_Report*  g_pkSQLPRpt   = NULL;

enum GameLogOp {
    GL_OP_KILL   = 1,
    GL_OP_DROP   = 2,
    GL_OP_TRADE  = 3,
    GL_OP_REPORT = 4
};

class GameLogSession : public IOCPSession {
public:
    virtual void OnPacket(const GPacket& rPkt) {
        if (rPkt.GetOpcode() != NC_INTER_GAMELOG_QUERY) return;
        PacketBuffer body = rPkt.Body();
        uint8 op = 0; body.ReadU8(op);
        switch (op) {
            case GL_OP_KILL: {
                uint32 killer = 0, mob = 0; uint16 mapId = 0;
                body.ReadU32(killer); body.ReadU32(mob); body.ReadU16(mapId);
                if (g_pkSQLPGL) g_pkSQLPGL->LogKill(killer, mob, mapId);
                break;
            }
            case GL_OP_DROP: {
                uint32 owner = 0, itemId = 0; uint16 count = 0;
                body.ReadU32(owner); body.ReadU32(itemId); body.ReadU16(count);
                if (g_pkSQLPGL) g_pkSQLPGL->LogDrop(owner, itemId, count);
                break;
            }
            case GL_OP_TRADE: {
                uint32 from = 0, to = 0, lo = 0, hi = 0; uint16 count = 0;
                body.ReadU32(from); body.ReadU32(to);
                body.ReadU32(lo);   body.ReadU32(hi);
                body.ReadU16(count);
                uint64 itemKey = ((uint64)hi << 32) | (uint64)lo;
                if (g_pkSQLPGL) g_pkSQLPGL->LogTrade(from, to, itemKey, count);
                break;
            }
            case GL_OP_REPORT: {
                std::string kind, payload;
                body.ReadString(kind); body.ReadString(payload);
                if (g_pkSQLPRpt) g_pkSQLPRpt->File(kind, payload);
                break;
            }
            default:
                SHINELOG_WARN("GameLog: unknown op=%u", (uint32)op);
                break;
        }
    }
};
static IOCPSession* MakeGameLogSession() { return new GameLogSession(); }

class GameLogService : public WinService {
public:
    GameLogService() : WinService("FiestaGameLog") {}
    virtual bool OnStart() {
        m_kInfo.Load("ServerInfo.txt");
        g_pkGameLogDB = new Database();
        g_pkGameLogDB->Connect(m_kInfo.GetString("GameLog.ConnStr",
            "Driver={SQL Server Native Client 11.0};Server=.;Database=World00_GameLog;Trusted_Connection=yes;"));
        g_pkSQLPGL  = new SQLP_GameLog(g_pkGameLogDB);
        g_pkSQLPRpt = new SQLP_Report (g_pkGameLogDB);
        if (!m_kIOCP.Start()) return false;
        return m_kAcceptor.Start(&m_kIOCP, m_kInfo.GetU16("GameLog.Port", 27603),
                                 &MakeGameLogSession);
    }
    virtual void OnStop() {
        m_kAcceptor.Stop(); m_kIOCP.Stop();
        delete g_pkSQLPRpt; g_pkSQLPRpt = NULL;
        delete g_pkSQLPGL;  g_pkSQLPGL  = NULL;
        if (g_pkGameLogDB) { g_pkGameLogDB->Disconnect(); delete g_pkGameLogDB; g_pkGameLogDB = NULL; }
    }
private:
    ServerInfo m_kInfo; IOCPManager m_kIOCP; Socket_Acceptor m_kAcceptor;
};

} // namespace fiesta

int main(int argc, char** argv) { fiesta::GameLogService s; return s.Run(argc, argv); }
