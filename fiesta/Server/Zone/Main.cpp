// Server/Zone/Main.cpp
// 06 -- Zone service exe entry. ZoneNN ID is read from ZoneServerInfo.txt.
#include "../Shared/WinService.h"
#include "../Shared/ServerInfo.h"
#include "../Shared/ShineLogSystem.h"
#include "../Shared/IOCPManager.h"
#include "../Shared/Socket_Acceptor.h"
#include "../DataReader/DataReader.h"
#include "../DataReader/DataBox.h"
#include "../DataReader/Schemas.h"
#include "ZoneServer.h"
#include "InstanceDungeon.h"
#include "KingdomQuest.h"
#include "MarketSystems.h"
#include "GuildSystem.h"
#include "CharDBClient.h"
#include "WMClient.h"
#include "ChargedEffect.h"
#include "GroupTables.h"
#include "MiscTables.h"
#include "MoreTables.h"
#include "ExtendedTables.h"
#include "EstateSystem.h"
#include "ExpeditionSystem.h"
#include "WorldTables.h"
#include "../DataReader/ShnRegistry.h"

namespace fiesta {

extern void RegisterZoneHandlers();

static IOCPSession* MakeZoneClient() { return new ClientSession(); }

class ZoneService : public WinService {
public:
    ZoneService() : WinService("FiestaZone") {}
    virtual bool OnStart() {
        m_kInfo.Load("ZoneServerInfo.txt");
        m_kReader.SetRoot(m_kInfo.GetString("Data.Root", "Data"));
        // Register every documented *Tab in DataBox (20 headline schemas).
        RegisterAllSchemaTabs();
        DataBox::Get().LoadAll(m_kReader);
        // Universal SHN ingest -- loads every *.shn under Data\Shine[-1].
        ShnRegistry::Get().LoadAll(m_kReader.GetRoot());
        // Build typed group accessors (Item/Mob/Skill/Map/AbState/...).
        BindAllGroupTables();
        BindAllMiscTables();
        BindAllMoreTables();
        BindAllExtendedTables();
        // Walk World/PineScript.txt and load every ScenarioBookShelf
        // .ps script (KQ, Promote, Wedding, Guild, instance dungeons).
        LoadAllPineScripts(m_kReader.GetRoot());
        // World/Karen.txt and World/MobChat.txt are not in the
        // headline DataBox set; load them explicitly.
        KarenAttackTable::Get().Load(m_kReader.GetRoot());
        MobChatTable::Get()    .Load(m_kReader.GetRoot());
        // Charged-item booster tables (sourced externally via the website
        // shop -- the Zone only consumes their effects).
        ChargedEffectTable::Get()  .Load(m_kReader.GetRoot());
        ChargedEffectManager::Get().LoadDeletable(m_kReader.GetRoot());

        ZoneServer::Get().Init(m_kInfo.GetU16("Zone.Id", 0));
        RegisterZoneHandlers();
        if (!m_kIOCP.Start()) return false;

        // Outbound link to the Character DB exe. Failure is non-fatal -- the
        // engine boots without it and players use provisional fill.
        CharDBClient::Get().Connect(&m_kIOCP,
            m_kInfo.GetString("CharDB.Ip",   "127.0.0.1"),
            m_kInfo.GetU16   ("CharDB.Port", 27602));

        // Outbound link to WorldManager. Same non-fatal posture.
        WMClient::Get().Connect(&m_kIOCP,
            m_kInfo.GetString("WM.Ip",   "127.0.0.1"),
            m_kInfo.GetU16   ("WM.Port", 28001),
            m_kInfo.GetU16   ("Zone.Id", 0),
            m_kInfo.GetString("Zone.PublicIp", "127.0.0.1"),
            m_kInfo.GetU16   ("Zone.ClientPort", 28100));

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
        BoothManager::Tick();
        EstateServer::Get().Tick(::GetTickCount64());
        ExpeditionSystem::Get().Tick();
        GuildWarManager::Tick();
        GuildTournamentSystem::Tick();
        ChargedEffectManager::Get().Tick();
        WMClient::Get().Heartbeat();
    }
    virtual void OnStop() {
        m_kAcceptor.Stop(); m_kIOCP.Stop();
        WMClient::Get().Disconnect();
        CharDBClient::Get().Disconnect();
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
