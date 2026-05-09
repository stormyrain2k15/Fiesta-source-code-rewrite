// Server/Zone/Main.cpp
// 06 -- Zone service exe entry. ZoneNN ID is read from ZoneServerInfo.txt.
#include "../Shared/WinService.h"
#include "../Shared/ServerInfo.h"
#include "../Shared/ShineLogSystem.h"
#include "../Shared/IOCPManager.h"
#include "../Shared/Socket_Acceptor.h"
#include "../DataReader/DataReader.h"
#include "../DataReader/DataBox.h"
#include "../DataReader/Tables.h"
#include "ZoneServer.h"
#include "InstanceDungeon.h"
#include "KingdomQuest.h"
#include "MarketSystems.h"
#include "GuildSystem.h"

namespace fiesta {

extern void RegisterZoneHandlers();

static IOCPSession* MakeZoneClient() { return new ClientSession(); }

class ZoneService : public WinService {
public:
    ZoneService() : WinService("FiestaZone") {}
    virtual bool OnStart() {
        m_kInfo.Load("ZoneServerInfo.txt");
        m_kReader.SetRoot(m_kInfo.GetString("Data.Root", "Data"));
        // Register every *Tab in DataBox.
        DataBox::Get().Register(&g_ItemInfoTab);
        DataBox::Get().Register(&g_AbStateInfoTab);
        DataBox::Get().Register(&g_ActSkillInfoTab);
        DataBox::Get().Register(&g_KQTeamTable);
        DataBox::Get().Register(&g_ChargedEffectTable);
        DataBox::Get().LoadAll(m_kReader);

        ZoneServer::Get().Init(m_kInfo.GetU16("Zone.Id", 0));
        RegisterZoneHandlers();
        if (!m_kIOCP.Start()) return false;
        if (!m_kAcceptor.Start(&m_kIOCP, m_kInfo.GetU16("Zone.ClientPort", 28100), &MakeZoneClient))
            return false;
        SHINELOG_INFO("Zone%02u started on :%u", m_kInfo.GetU16("Zone.Id", 0),
                      m_kInfo.GetU16("Zone.ClientPort", 28100));
        return true;
    }
    virtual void OnTick() {
        ZoneServer::Get().Tick();
        MIDServer::Get().Tick();
        KQServer::Get().Tick();
        AuctionSystem::Get().Tick();
        GuildWarManager::Tick();
        GuildTournamentSystem::Tick();
    }
    virtual void OnStop() {
        m_kAcceptor.Stop(); m_kIOCP.Stop();
        ZoneServer::Get().Shutdown();
        DataBox::Get().Shutdown();
    }
private:
    ServerInfo      m_kInfo;
    IOCPManager     m_kIOCP;
    Socket_Acceptor m_kAcceptor;
    DataReader      m_kReader;
};

} // namespace fiesta

int main(int argc, char** argv) { fiesta::ZoneService s; return s.Run(argc, argv); }
