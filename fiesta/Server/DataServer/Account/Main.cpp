// Server/DataServer/Account/Main.cpp
// 03 -- Account DB service exe entry.
// EVIDENCE: PDB_CONFIRMED  symbol: DataServer, GameDBSession (Account flavor)
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

class AccountDBSession : public IOCPSession {
public:
    virtual void OnConnect()    { SHINELOG_INFO("AccountDB: peer connected"); }
    virtual void OnDisconnect() { SHINELOG_INFO("AccountDB: peer disconnected"); }
    virtual void OnPacket(const GPacket& rPkt) {
        SHINELOG_DEBUG("AccountDB recv NC=0x%04X size=%u", rPkt.GetOpcode(), (uint32)rPkt.Body().Size());
        // Account-DB-side service handlers (login verify, account create, account log) live here
        // and execute SQLP_Account stored procedures.
    }
};

static IOCPSession* MakeAccountDBSession() { return new AccountDBSession(); }

class AccountDBService : public WinService {
public:
    AccountDBService() : WinService("FiestaAccountDB"), m_pkDB(NULL) {}
    virtual bool OnStart() {
        m_kInfo.Load("ServerInfo.txt");
        m_pkDB = new Database();
        m_pkDB->Connect(m_kInfo.GetString("Account.ConnStr",
            "Driver={SQL Server Native Client 11.0};Server=.;Database=Account;Trusted_Connection=yes;"));
        if (!m_kIOCP.Start()) return false;
        return m_kAcceptor.Start(&m_kIOCP, m_kInfo.GetU16("Account.Port", 27600), &MakeAccountDBSession);
    }
    virtual void OnStop() {
        m_kAcceptor.Stop(); m_kIOCP.Stop();
        if (m_pkDB) { m_pkDB->Disconnect(); delete m_pkDB; m_pkDB = NULL; }
    }
private:
    ServerInfo      m_kInfo;
    IOCPManager     m_kIOCP;
    Socket_Acceptor m_kAcceptor;
    Database*       m_pkDB;
};

} // namespace fiesta

int main(int argc, char** argv) {
    fiesta::AccountDBService svc;
    return svc.Run(argc, argv);
}
