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
#include "ZoneAssetLoader.h"
#include "ShineNPCTable.h"
#include "ClassParamTable.h"
#include "ChrCommonTable.h"
#include "MobSpawnSystem.h"
#include "../DataReader/ShnRegistry.h"
#include "../../Lua/LuaRuntime.h"

namespace fiesta {

extern void RegisterZoneHandlers();

static IOCPSession* MakeZoneClient() { return new ClientSession(); }

class ZoneService : public WinService {
public:
    ZoneService() : WinService("FiestaZone") {}
    virtual bool OnStart() {
        m_kInfo.Load("ZoneServerInfo.txt");
        m_kReader.SetRoot(m_kInfo.GetString("Data.Root", "9Data"));
        // Two distinct roots used across the loader chain:
        //   * `dataRoot`  -- top-level "Data" directory; ShnRegistry walks
        //                    `<dataRoot>\Shine\*.shn` and `<dataRoot>\Shine-1\*.shn`.
        //   * `shineRoot` -- "Data\Shine" directory; every per-folder parser
        //                    (World/, AbState/, AreaBMP/, BlockInfo/, ...) treats
        //                    this as their root and joins their subdir name.
        const std::string& dataRoot  = m_kReader.GetRoot();
        const std::string  shineRoot = dataRoot + "\\Shine";

        // Register every documented *Tab in DataBox (20 headline schemas).
        RegisterAllSchemaTabs();
        DataBox::Get().LoadAll(m_kReader);
        // Universal SHN ingest -- loads every *.shn under Data\Shine[-1].
        ShnRegistry::Get().LoadAll(dataRoot);
        // Build typed group accessors (Item/Mob/Skill/Map/AbState/...).
        BindAllGroupTables();
        BindAllMiscTables();
        BindAllMoreTables();
        BindAllExtendedTables();
        // Walk World/PineScript.txt and load every ScenarioBookShelf
        // .ps script (KQ, Promote, Wedding, Guild, instance dungeons).
        LoadAllPineScripts(shineRoot);
        // World/Karen.txt and World/MobChat.txt are not in the
        // headline DataBox set; load them explicitly.
        KarenAttackTable::Get().Load(shineRoot);
        MobChatTable::Get()    .Load(shineRoot);
        // Charged-item booster tables (sourced externally via the website
        // shop -- the Zone only consumes their effects).
        ChargedEffectTable::Get()  .Load(shineRoot);
        ChargedEffectManager::Get().LoadDeletable(shineRoot);

        // Walk every per-folder data source under Data\Shine -- AbState/,
        // AreaBMP/, BlockInfo/, MobAttackSequence/, MobBehaviorDescript/,
        // MobRegen/, MobRoam/, MobSetting/, NPCItemList/, Script/, plus
        // every .lua under LuaScript/.
        m_kLua.Open();
        m_kLua.RegisterCBindings();
        ZoneAssetLoader::LoadAll(shineRoot, &m_kLua);

        // World/NPC.txt -> spawn every static NPC; register (NpcId -> mob-name)
        // so NPCItemListBox lookups have a key. Must run *after*
        // BindAllGroupTables (MapTables) and ZoneAssetLoader (Block grids).
        ShineNPCTable::Get().Load   (shineRoot);
        ShineNPCTable::Get().SpawnAll();

        // Per-class stat / progression tables (World/Param*Server.txt) and
        // the global ChrCommon stat ladder (World/ChrCommon.txt). These are
        // hot-path lookups for level-up, free-stat allocation, and PvP
        // exp-loss; load before any character can log in.
        ClassParamTable::Get().Load(shineRoot);
        ChrCommonTable ::Get().Load(shineRoot);

        // Build the per-zone mob spawn driver. After this, every populated
        // MobRegen group has an entry in MobSpawnSystem and `Tick()` will
        // top each one up to its MobNum target.
        MobSpawnSystem::Get().LoadAll();

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
        MobSpawnSystem::Get().Tick(::GetTickCount64());
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
        m_kLua.Close();
    }
private:
    ServerInfo      m_kInfo;
    IOCPManager     m_kIOCP;
    Socket_Acceptor m_kAcceptor;
    DataReader      m_kReader;
    LuaRuntime      m_kLua;
};

} // namespace fiesta

int main(int argc, char** argv) { fiesta::ZoneService s; return s.Run(argc, argv); }
