// Server/DataServer/GameLog/Main.cpp
// 03/41 -- GameLog DB service exe entry. Records combat / drop / mob hunt /
// trade logs into the World00_GameLog database.
// EVIDENCE: PDB_CONFIRMED  symbol: GameLogSession, MobHuntLog, ItemDropLog
#include "../../Shared/WinService.h"
#include "../../Shared/ServerInfo.h"
#include "../../Shared/ShineLogSystem.h"
#include "../../Shared/IOCPManager.h"
#include "../../Shared/Socket_Acceptor.h"
#include "../Common/Database.h"
#include "../Common/SQLP.h"

namespace fiesta {

static Database*     g_pkGameLogDB = NULL;
static SQLP_GameLog* g_pkSQLPGL    = NULL;
static SQLP_Report*  g_pkSQLPRpt   = NULL;

class GameLogSession : public IOCPSession {
public:
    virtual void OnPacket(const GPacket& rPkt) {
        SHINELOG_DEBUG("GameLog recv NC=0x%04X", rPkt.GetOpcode());
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
