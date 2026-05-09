// Server/DataServer/Account/Main.cpp
// Account DB service exe entry.
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
#include "../../Shared/PacketBuffer.h"
#include "../../Shared/GPacket.h"
#include "../../Common/NETCOMMAND.h"
#include "../../Common/SendPacket.h"
#include "../Common/Database.h"
#include "../Common/SQLP.h"

namespace fiesta {

// Single static facade -- each session pointer is non-owning.
static Database*        g_pkAccountDB = NULL;
static SQLP_Account*    g_pkSQLPAcc   = NULL;
static SQLP_IPChecker*  g_pkSQLPIPCk  = NULL;

// Inbound op codes routed from operator tools / remote managers.
//   1 = VerifyLogin   { string user, string pass }    -> { ok, accountId }
//   2 = IsIPBlocked   { string ip }                    -> { ok, blocked }
//   3 = SetBlocked    { uint32 acct, uint8 blocked }   -> { ok }
enum AcctDBOp { ADB_OP_VERIFY = 1, ADB_OP_IS_IP_BLOCKED = 2, ADB_OP_SET_BLOCKED = 3 };

class AccountDBSession : public IOCPSession {
public:
    virtual void OnPacket(const GPacket& rPkt) {
        if (rPkt.GetOpcode() != NC_INTER_ACCTLOG_QUERY) return;
        PacketBuffer body = rPkt.Body();
        uint8 op = 0; body.ReadU8(op);
        switch (op) {
            case ADB_OP_VERIFY: {
                std::string u, p; body.ReadString(u); body.ReadString(p);
                AccountID a = 0; int authId = 0; bool blocked = false;
                bool ok = g_pkSQLPAcc &&
                          g_pkSQLPAcc->VerifyLogin(u, p, a, authId, blocked);
                PacketBuffer ack; ack.WriteU8(op);
                ack.WriteU8(ok && !blocked ? 1 : 0);
                ack.WriteU32(a);
                ack.WriteI32(authId);
                SendPacket(this, NC_INTER_ACCTLOG_RESPONSE, ack.Data(), ack.Size());
                break;
            }
            case ADB_OP_IS_IP_BLOCKED: {
                std::string ip; body.ReadString(ip);
                uint8 blocked = (g_pkSQLPIPCk && g_pkSQLPIPCk->IsBlocked(ip)) ? 1 : 0;
                PacketBuffer ack; ack.WriteU8(op); ack.WriteU8(1); ack.WriteU8(blocked);
                SendPacket(this, NC_INTER_ACCTLOG_RESPONSE, ack.Data(), ack.Size());
                break;
            }
            case ADB_OP_SET_BLOCKED: {
                uint32 acct = 0; uint8 blocked = 0;
                body.ReadU32(acct); body.ReadU8(blocked);
                bool ok = g_pkSQLPAcc && g_pkSQLPAcc->SetBlocked(acct, blocked != 0);
                PacketBuffer ack; ack.WriteU8(op); ack.WriteU8(ok ? 1 : 0);
                SendPacket(this, NC_INTER_ACCTLOG_RESPONSE, ack.Data(), ack.Size());
                break;
            }
            default:
                SHINELOG_WARN("AccountDB: unknown op=%u", (uint32)op);
                break;
        }
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
