// Server/Login/Main.cpp
// 04 -- Login service exe entry.
#include "../Shared/WinService.h"
#include "../Shared/ServerInfo.h"
#include "../Shared/IOCPManager.h"
#include "../Shared/Socket_Acceptor.h"
#include "../Shared/ShineLogSystem.h"
#include "LoginClientSession.h"
#include "ClientVersionKeyInfo.h"

namespace fiesta {

static IOCPSession* MakeLoginClientSession() { return new LoginClientSession(); }

class LoginService : public WinService {
public:
    LoginService() : WinService("FiestaLogin") {}
    virtual bool OnStart() {
        m_kInfo.Load("LoginServerInfo.txt");
        ClientVersionKeyInfo::Get().Load("ClientVersionKeyInfo.txt");
        if (!m_kIOCP.Start()) return false;
        const ServiceEndpoint* pkSvc = m_kInfo.FindFirst(SK_Login, -1, 20);
        uint16 uiPort = pkSvc ? pkSvc->uiPort : 9010;
        if (!m_kAcceptor.Start(&m_kIOCP, uiPort, &MakeLoginClientSession))
            return false;
        SHINELOG_INFO("Login service running on :%u  versionKey=%s",
                      uiPort, ClientVersionKeyInfo::Get().Key().c_str());
        return true;
    }
    virtual void OnStop() { m_kAcceptor.Stop(); m_kIOCP.Stop(); }
private:
    ServerInfo      m_kInfo;
    IOCPManager     m_kIOCP;
    Socket_Acceptor m_kAcceptor;
};

} // namespace fiesta

int main(int argc, char** argv) { fiesta::LoginService s; return s.Run(argc, argv); }
