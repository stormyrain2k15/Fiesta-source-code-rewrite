// Server/DataServer/AccountLog/Main.cpp
// 03 -- AccountLog DB service exe entry. Append-only login history, IP-block
// table, friend-event audit.
// EVIDENCE: PDB_CONFIRMED  symbol: AccountLog, AccountLogSession
#include "../../Shared/WinService.h"
#include "../../Shared/ServerInfo.h"
#include "../../Shared/ShineLogSystem.h"
#include "../../Shared/IOCPManager.h"
#include "../../Shared/Socket_Acceptor.h"
#include "../Common/Database.h"
#include "../Common/SQLP.h"

namespace fiesta {

static Database*         g_pkAcctLogDB = NULL;
static SQLP_AccountLog*  g_pkSQLPAcctL = NULL;

class AccountLogSession : public IOCPSession {
public:
    virtual void OnPacket(const GPacket& rPkt) {
        SHINELOG_DEBUG("AccountLog recv NC=0x%04X", rPkt.GetOpcode());
        // Append-only writes: AppendLogin/AppendLogout/IPBlockIsBlock are
        // exposed via g_pkSQLPAcctL.
    }
};
static IOCPSession* MakeAccountLogSession() { return new AccountLogSession(); }

class AccountLogService : public WinService {
public:
    AccountLogService() : WinService("FiestaAccountLog") {}
    virtual bool OnStart() {
        m_kInfo.Load("ServerInfo.txt");
        g_pkAcctLogDB = new Database();
        const OdbcEntry* od = m_kInfo.FindOdbc("AccountLog");
        g_pkAcctLogDB->Connect(od ? od->kConnStr :
            "Driver={SQL Server Native Client 11.0};Server=.;Database=AccountLog;Trusted_Connection=yes;");
        g_pkSQLPAcctL = new SQLP_AccountLog(g_pkAcctLogDB);
        if (!m_kIOCP.Start()) return false;
        return m_kAcceptor.Start(&m_kIOCP, m_kInfo.GetU16("AccountLog.Port", 27601),
                                 &MakeAccountLogSession);
    }
    virtual void OnStop() {
        m_kAcceptor.Stop(); m_kIOCP.Stop();
        delete g_pkSQLPAcctL; g_pkSQLPAcctL = NULL;
        if (g_pkAcctLogDB) { g_pkAcctLogDB->Disconnect(); delete g_pkAcctLogDB; g_pkAcctLogDB = NULL; }
    }
private:
    ServerInfo m_kInfo; IOCPManager m_kIOCP; Socket_Acceptor m_kAcceptor;
};

} // namespace fiesta

int main(int argc, char** argv) { fiesta::AccountLogService s; return s.Run(argc, argv); }
