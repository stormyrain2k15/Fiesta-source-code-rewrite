// Server/Login/Main.cpp
// Login service exe entry. Owns:
//   * the IOCP listener clients connect to with NC_USER_LOGIN_REQ
//   * a co-resident SQLP_Account / SQLP_IPChecker pair so credential
//     verification is ODBC-direct (no extra hop)
//   * the AccountLogClient that records every login / logout event
//
// 로그인 서비스 실행 파일 진입점. 다음을 담당함:
//   * 클라이언트가 NC_USER_LOGIN_REQ 로 접속하는 IOCP 리스너
//   * SQLP_Account / SQLP_IPChecker 를 같은 프로세스에 두어
//     계정 인증을 ODBC 로 직접 처리 (추가 홉 없음)
//   * 모든 로그인 / 로그아웃 이벤트를 기록하는 AccountLogClient
#include "../Shared/WinService.h"
#include "../Shared/ServerInfo.h"
#include "../Shared/IOCPManager.h"
#include "../Shared/Socket_Acceptor.h"
#include "../Shared/ShineLogSystem.h"
#include "../DataServer/Common/Database.h"
#include "../DataServer/Common/SQLP.h"
#include "LoginClientSession.h"
#include "ClientVersionKeyInfo.h"

namespace shine {

// Globals consumed by LoginClientSession::HandleLogin.
Database*           g_pkLoginAccountDB = NULL;
Database*           g_pkLoginAcctLogDB = NULL;
SQLP_Account*       g_pkLoginAccount   = NULL;
SQLP_IPChecker*     g_pkLoginIPCheck   = NULL;
SQLP_AccountLog*    g_pkLoginAcctLog   = NULL;

static IOCPSession* MakeLoginClientSession() { return new LoginClientSession(); }

class LoginService : public WinService {
public:
    LoginService() : WinService("ShineLogin") {}
    virtual bool OnStart() {
        m_kInfo.Load("LoginServerInfo.txt");
        ServerInfo::SetCurrent(&m_kInfo);
        ClientVersionKeyInfo::Get().Load("ClientVersionKeyInfo.txt");

        // Connect the Account ODBC source for credential verification.
        g_pkLoginAccountDB = new Database();
        const OdbcEntry* od = m_kInfo.FindOdbc("Account");
        if (!g_pkLoginAccountDB->Connect(od ? od->kConnStr :
            "Driver={SQL Server Native Client 11.0};Server=.;Database=Account;Trusted_Connection=yes;")) {
            SHINELOG_WARN("Login: Account ODBC connect failed");
        }
        g_pkLoginAccount = new SQLP_Account  (g_pkLoginAccountDB);
        g_pkLoginIPCheck = new SQLP_IPChecker(g_pkLoginAccountDB);

        // Connect the AccountLog ODBC for the per-login audit row.
        g_pkLoginAcctLogDB = new Database();
        const OdbcEntry* odl = m_kInfo.FindOdbc("AccountLog");
        if (!g_pkLoginAcctLogDB->Connect(odl ? odl->kConnStr :
            "Driver={SQL Server Native Client 11.0};Server=.;Database=AccountLog;Trusted_Connection=yes;")) {
            SHINELOG_WARN("Login: AccountLog ODBC connect failed");
        }
        g_pkLoginAcctLog = new SQLP_AccountLog(g_pkLoginAcctLogDB);

        if (!m_kIOCP.Start()) return false;
        const ServiceEndpoint* pkSvc = m_kInfo.FindFirst(SK_Login, -1, 20);
        uint16 uiPort = pkSvc ? pkSvc->uiPort : 9010;
        if (!m_kAcceptor.Start(&m_kIOCP, uiPort, &MakeLoginClientSession))
            return false;
        SHINELOG_INFO("Login service running on :%u  versionKey=%s",
                      uiPort, ClientVersionKeyInfo::Get().Key().c_str());
        return true;
    }
    virtual void OnStop() {
        m_kAcceptor.Stop(); m_kIOCP.Stop();
        ServerInfo::SetCurrent(NULL);
        delete g_pkLoginAcctLog;  g_pkLoginAcctLog  = NULL;
        delete g_pkLoginIPCheck;  g_pkLoginIPCheck  = NULL;
        delete g_pkLoginAccount;  g_pkLoginAccount  = NULL;
        if (g_pkLoginAcctLogDB) {
            g_pkLoginAcctLogDB->Disconnect();
            delete g_pkLoginAcctLogDB; g_pkLoginAcctLogDB = NULL;
        }
        if (g_pkLoginAccountDB) {
            g_pkLoginAccountDB->Disconnect();
            delete g_pkLoginAccountDB; g_pkLoginAccountDB = NULL;
        }
    }
private:
    ServerInfo      m_kInfo;
    IOCPManager     m_kIOCP;
    Socket_Acceptor m_kAcceptor;
};

} // namespace shine

int main(int argc, char** argv) { shine::LoginService s; return s.Run(argc, argv); }
