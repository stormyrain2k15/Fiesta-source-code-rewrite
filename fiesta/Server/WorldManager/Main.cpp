// Server/WorldManager/Main.cpp
// WorldManager service exe entry. Builds five accept loops (Client, Zone,
// Login, CharDB, OPTool) on the IOCP, fans data load through DataFileServer
// at boot, and ticks every cross-zone subsystem via WMServicesTickAll() each
// main-loop iteration.
//
// 월드 매니저 서비스 실행 파일 진입점. IOCP 위에 다섯 개의
// 어셉트 루프 (Client, Zone, Login, CharDB, OPTool) 를 구성하고,
// 부팅 시 DataFileServer 를 통해 데이터 로딩을 분배하며,
// 메인 루프 매 회마다 WMServicesTickAll() 로 모든 크로스존
// 서브시스템을 갱신함.
#include "../Shared/WinService.h"
#include "../Shared/ServerInfo.h"
#include "../Shared/IOCPManager.h"
#include "../Shared/Socket_Acceptor.h"
#include "../Shared/ShineLogSystem.h"
#include "WorldManagerServer.h"
#include "WMServices.h"

namespace shine {

static IOCPSession* MakeWMClient()  { return new WMClientSession(); }
static IOCPSession* MakeWMZone()    { return new WMZoneSession(); }
static IOCPSession* MakeWMLogin()   { return new WMLoginSession(); }
static IOCPSession* MakeWMCharDB()  { return new WMCharDBSession(); }
static IOCPSession* MakeWMOPTool()  { return new WMOPToolSession(); }

class WorldManagerService : public WinService {
public:
    WorldManagerService() : WinService("ShineWorldManager") {}
    virtual bool OnStart() {
        m_kInfo.Load("ServerInfo.txt");

        // Boot the SHN registry first so KQ / NpcSchedule / GMEvent / Gamble
        // can read their tables on the very first Tick().
        std::string kDataRoot = m_kInfo.GetString("Data.Root", "Data");
        DataFileServer::Get().LoadAll(kDataRoot);

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
        WMServicesTickAll();
    }
private:
    ServerInfo      m_kInfo;
    IOCPManager     m_kIOCP;
    Socket_Acceptor m_kClientAcc, m_kZoneAcc, m_kLoginAcc, m_kCharAcc, m_kOPToolAcc;
};

} // namespace shine

int main(int argc, char** argv) { shine::WorldManagerService s; return s.Run(argc, argv); }
