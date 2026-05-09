// Server/Login/Main.cpp
// 04 -- Login service exe entry.
#include "../Shared/WinService.h"
#include "../Shared/ServerInfo.h"
#include "../Shared/IOCPManager.h"
#include "../Shared/Socket_Acceptor.h"
#include "../Shared/ShineLogSystem.h"
#include "LoginClientSession.h"

namespace fiesta {

static IOCPSession* MakeLoginClientSession() { return new LoginClientSession(); }

class LoginService : public WinService {
public:
    LoginService() : WinService("FiestaLogin") {}
    virtual bool OnStart() {
        m_kInfo.Load("ServerInfo.txt");
        if (!m_kIOCP.Start()) return false;
        if (!m_kAcceptor.Start(&m_kIOCP, m_kInfo.GetU16("Login.Port", 9010), &MakeLoginClientSession))
            return false;
        SHINELOG_INFO("Login service running on :%u", m_kInfo.GetU16("Login.Port", 9010));
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
