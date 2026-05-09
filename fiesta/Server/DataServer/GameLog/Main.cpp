// Server/DataServer/GameLog/Main.cpp
// 03/41 -- GameLog DB service exe entry. Records combat / drop / mob hunt / item drop logs.
// EVIDENCE: PDB_CONFIRMED  symbol: GameLogSession, MobHuntLog, ItemDropLog
#include "../../Shared/WinService.h"
#include "../../Shared/ServerInfo.h"
#include "../../Shared/ShineLogSystem.h"
#include "../../Shared/IOCPManager.h"
#include "../../Shared/Socket_Acceptor.h"
#include "../Common/Database.h"

namespace fiesta {

class GameLogSession : public IOCPSession {
public:
    virtual void OnPacket(const GPacket& rPkt) {
        SHINELOG_DEBUG("GameLog recv NC=0x%04X", rPkt.GetOpcode());
    }
};
static IOCPSession* MakeGameLogSession() { return new GameLogSession(); }

class GameLogService : public WinService {
public:
    GameLogService() : WinService("FiestaGameLog"), m_pkDB(NULL) {}
    virtual bool OnStart() {
        m_kInfo.Load("ServerInfo.txt");
        m_pkDB = new Database();
        m_pkDB->Connect(m_kInfo.GetString("GameLog.ConnStr",
            "Driver={SQL Server Native Client 11.0};Server=.;Database=GameLog;Trusted_Connection=yes;"));
        if (!m_kIOCP.Start()) return false;
        return m_kAcceptor.Start(&m_kIOCP, m_kInfo.GetU16("GameLog.Port", 27603), &MakeGameLogSession);
    }
    virtual void OnStop() {
        m_kAcceptor.Stop(); m_kIOCP.Stop();
        if (m_pkDB) { m_pkDB->Disconnect(); delete m_pkDB; m_pkDB = NULL; }
    }
private:
    ServerInfo m_kInfo; IOCPManager m_kIOCP; Socket_Acceptor m_kAcceptor; Database* m_pkDB;
};

} // namespace fiesta

int main(int argc, char** argv) { fiesta::GameLogService s; return s.Run(argc, argv); }
