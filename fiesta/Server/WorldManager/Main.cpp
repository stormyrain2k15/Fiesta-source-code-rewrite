// Server/WorldManager/Main.cpp
// WorldManager service exe entry.
#include "../Shared/WinService.h"
#include "../Shared/ServerInfo.h"
#include "../Shared/IOCPManager.h"
#include "../Shared/Socket_Acceptor.h"
#include "../Shared/ShineLogSystem.h"
#include "WorldManagerServer.h"

namespace fiesta {

static IOCPSession* MakeWMClient()  { return new WMClientSession(); }
static IOCPSession* MakeWMZone()    { return new WMZoneSession(); }
static IOCPSession* MakeWMLogin()   { return new WMLoginSession(); }
static IOCPSession* MakeWMCharDB()  { return new WMCharDBSession(); }
static IOCPSession* MakeWMOPTool()  { return new WMOPToolSession(); }

class WorldManagerService : public WinService {
public:
    WorldManagerService() : WinService("FiestaWorldManager") {}
    virtual bool OnStart() {
        m_kInfo.Load("ServerInfo.txt");
        if (!m_kIOCP.Start()) return false;
        m_kClientAcc.Start(&m_kIOCP, m_kInfo.GetU16("WM.ClientPort", 28000), &MakeWMClient);
        m_kZoneAcc  .Start(&m_kIOCP, m_kInfo.GetU16("WM.ZonePort"  , 28001), &MakeWMZone);
        m_kLoginAcc .Start(&m_kIOCP, m_kInfo.GetU16("WM.LoginPort" , 28002), &MakeWMLogin);
        m_kCharAcc  .Start(&m_kIOCP, m_kInfo.GetU16("WM.CharDBPort", 28003), &MakeWMCharDB);
        // Admin / OperatorTool entry: loopback-only. The OperatorTool exe is
        // not part of this tree -- any external admin panel that connects
        // here must run on the same host. See WMOPToolSession docs.
        m_kOPToolAcc.Start(&m_kIOCP, m_kInfo.GetU16("WM.OPToolPort", 28004), &MakeWMOPTool,
                           /*bLoopbackOnly*/ true);
        return true;
    }
    virtual void OnStop() {
        m_kClientAcc.Stop(); m_kZoneAcc.Stop(); m_kLoginAcc.Stop();
        m_kCharAcc.Stop(); m_kOPToolAcc.Stop(); m_kIOCP.Stop();
    }
    virtual void OnTick() {
        WorldManagerServer::Get().PartyFinder()->Tick();
        WorldManagerServer::Get().Ranking    ()->Tick();
        WorldManagerServer::Get().ChatSteal  ()->Tick();
    }
private:
    ServerInfo      m_kInfo;
    IOCPManager     m_kIOCP;
    Socket_Acceptor m_kClientAcc, m_kZoneAcc, m_kLoginAcc, m_kCharAcc, m_kOPToolAcc;
};

} // namespace fiesta

int main(int argc, char** argv) { fiesta::WorldManagerService s; return s.Run(argc, argv); }
