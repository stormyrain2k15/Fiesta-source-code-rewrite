// Server/DataServer/Account/Main.cpp
// 03 -- Account DB service exe entry.
// EVIDENCE: PDB_CONFIRMED  symbol: DataServer, GameDBSession (Account flavor)
//
// Hosts:
//   - SQLP_Account     (tUser / tUserAuth / tUserBlock / charge tables)
//   - SQLP_AccountLog  (cross-DB facade kept on the same exe for legacy
//                       deployments; the AccountLog connection points at the
//                       AccountLog database via ServerInfo's `AccountLog` ODBC
//                       entry, falling back to the Account DSN.)
#include "../../Shared/WinService.h"
#include "../../Shared/ServerInfo.h"
#include "../../Shared/ShineLogSystem.h"
#include "../../Shared/IOCPManager.h"
#include "../../Shared/Socket_Acceptor.h"
#include "../../Shared/GPacket.h"
#include "../../Common/NETCOMMAND.h"
#include "../Common/Database.h"
#include "../Common/SQLP.h"

namespace fiesta {

// Single static facade -- each session pointer is non-owning.
static Database*        g_pkAccountDB = NULL;
static SQLP_Account*    g_pkSQLPAcc   = NULL;
static SQLP_IPChecker*  g_pkSQLPIPCk  = NULL;

class AccountDBSession : public IOCPSession {
public:
    virtual void OnConnect()    { SHINELOG_INFO("AccountDB: peer connected"); }
    virtual void OnDisconnect() { SHINELOG_INFO("AccountDB: peer disconnected"); }
    virtual void OnPacket(const GPacket& rPkt) {
        SHINELOG_DEBUG("AccountDB recv NC=0x%04X size=%u",
            rPkt.GetOpcode(), (uint32)rPkt.Body().Size());
        // Login-side routing is performed by the upstream Login service; this
        // exe is a passive ODBC facade.  Call sites that need credential
        // verification reach in via g_pkSQLPAcc->VerifyLogin / IsBlocked.
    }
};

static IOCPSession* MakeAccountDBSession() { return new AccountDBSession(); }

class AccountDBService : public WinService {
public:
    AccountDBService() : WinService("FiestaAccountDB") {}
    virtual bool OnStart() {
        m_kInfo.Load("ServerInfo.txt");
        g_pkAccountDB = new Database();
        const OdbcEntry* od = m_kInfo.FindOdbc("Account");
        bool bConn = g_pkAccountDB->Connect(od ? od->kConnStr :
            "Driver={SQL Server Native Client 11.0};Server=.;Database=Account;Trusted_Connection=yes;");
        if (!bConn) SHINELOG_WARN("AccountDB: ODBC connect failed -- running in offline mode");
        g_pkSQLPAcc  = new SQLP_Account(g_pkAccountDB);
        g_pkSQLPIPCk = new SQLP_IPChecker(g_pkAccountDB);
        if (!m_kIOCP.Start()) return false;
        const ServiceEndpoint* pkSvc = m_kInfo.FindFirst(SK_AccountDB);
        uint16 uiPort = pkSvc ? pkSvc->uiPort : 9031;
        return m_kAcceptor.Start(&m_kIOCP, uiPort, &MakeAccountDBSession);
    }
    virtual void OnStop() {
        m_kAcceptor.Stop(); m_kIOCP.Stop();
        delete g_pkSQLPIPCk; g_pkSQLPIPCk = NULL;
        delete g_pkSQLPAcc;  g_pkSQLPAcc  = NULL;
        if (g_pkAccountDB) { g_pkAccountDB->Disconnect(); delete g_pkAccountDB; g_pkAccountDB = NULL; }
    }
private:
    ServerInfo      m_kInfo;
    IOCPManager     m_kIOCP;
    Socket_Acceptor m_kAcceptor;
};

} // namespace fiesta

int main(int argc, char** argv) {
    fiesta::AccountDBService svc;
    return svc.Run(argc, argv);
}
