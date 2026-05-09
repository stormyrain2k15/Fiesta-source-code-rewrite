// Server/DataServer/AccountLog/Main.cpp
// 03 -- AccountLog DB service exe entry.
// EVIDENCE: PDB_CONFIRMED  symbol: AccountLog, AccountLogSession
#include "../../Shared/WinService.h"
#include "../../Shared/ServerInfo.h"
#include "../../Shared/ShineLogSystem.h"
#include "../../Shared/IOCPManager.h"
#include "../../Shared/Socket_Acceptor.h"
#include "../Common/Database.h"

namespace fiesta {

class AccountLogSession : public IOCPSession {
public:
    virtual void OnPacket(const GPacket& rPkt) {
        SHINELOG_DEBUG("AccountLog recv NC=0x%04X", rPkt.GetOpcode());
        // Append-only writes: login_history, world_pick_history, kick_history, ban_history, etc.
    }
};
static IOCPSession* MakeAccountLogSession() { return new AccountLogSession(); }

class AccountLogService : public WinService {
public:
    AccountLogService() : WinService("FiestaAccountLog"), m_pkDB(NULL) {}
    virtual bool OnStart() {
        m_kInfo.Load("ServerInfo.txt");
        m_pkDB = new Database();
        m_pkDB->Connect(m_kInfo.GetString("AccountLog.ConnStr",
            "Driver={SQL Server Native Client 11.0};Server=.;Database=AccountLog;Trusted_Connection=yes;"));
        if (!m_kIOCP.Start()) return false;
        return m_kAcceptor.Start(&m_kIOCP, m_kInfo.GetU16("AccountLog.Port", 27601), &MakeAccountLogSession);
    }
    virtual void OnStop() {
        m_kAcceptor.Stop(); m_kIOCP.Stop();
        if (m_pkDB) { m_pkDB->Disconnect(); delete m_pkDB; m_pkDB = NULL; }
    }
private:
    ServerInfo m_kInfo; IOCPManager m_kIOCP; Socket_Acceptor m_kAcceptor; Database* m_pkDB;
};

} // namespace fiesta

int main(int argc, char** argv) { fiesta::AccountLogService s; return s.Run(argc, argv); }
