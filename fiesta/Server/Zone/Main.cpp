// Server/Zone/Main.cpp
// Zone service exe entry. ZoneNN ID is read from ZoneServerInfo.txt.
//
// 존 서버 서비스 실행 파일 진입점. ZoneNN ID 는 ZoneServerInfo.txt
// 에서 읽음. 게임플레이 엔진 본체로, 모든 액티브 필드와 객체
// 그리고 43 개의 게임 서브시스템을 호스팅함.
#include "../Shared/WinService.h"
#include "../Shared/ServerInfo.h"
#include "../Shared/ShineLogSystem.h"
#include "../Shared/IOCPManager.h"
#include "../Shared/Socket_Acceptor.h"
#include "../Shared/GTimer.h"
#include "../DataReader/DataReader.h"
#include "../DataReader/DataBox.h"
#include "../DataReader/Schemas.h"
#include "ZoneServer.h"
#include "InstanceDungeon.h"
#include "KingdomQuest.h"
#include "MarketSystems.h"
#include "GuildSystem.h"
#include "CharDBClient.h"
#include "GameLogClient.h"
#include "WMClient.h"
#include "ChargedEffect.h"
#include "GroupTables.h"
#include "MiscTables.h"
#include "MoreTables.h"
#include "ExtendedTables.h"
#include "TypedSchemaConsumers.h"
#include "EstateSystem.h"
#include "ExpeditionSystem.h"
#include "WorldTables.h"
#include "ZoneAssetLoader.h"
#include "ShineNPCTable.h"
#include "ClassParamTable.h"
#include "ChrCommonTable.h"
#include "LevelGapTable.h"
#include "MobResistTable.h"
#include "MoverTables.h"
#include "WorldTables.h"   // FEATURE: combat-data-load -- ExpRecalc/Quest/Drop/RandomOption/etc
#include "Tables/ItemDropTableW.h"
#include "Tables/ItemOptionsTable.h"
#include "Tables/MiscDataTable.h"
#include "Tables/QuestParserTable.h"
#include "Tables/TreasureRewardTable.h"
#include "AbnormalStateDictionary.h"
#include "MultiHitTable.h"
#include "GuildAcademy.h"
#include "CraftAndPet.h"   // MinimonDataBox
#include "SocialSystems.h" // SpamerPenaltyDataBox
#include "ItemSystems.h"   // SetItemData
#include "Link.h"          // FEATURE: portals
#include "TownPortalSystem.h"
#include "LuckyCapsuleSystem.h"
#include "GambleHouse/GambleSystem.h"
#include "MobSpawnSystem.h"
#include "MobAIRunner.h"
#include "InterBroadcastSinks.h"
#include "../DataReader/ShnRegistry.h"
#include "../DataReader/QuestShnReader.h"
#include "../DataReader/TableScriptFile.h"
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
        // Quest/scenario SHNs are tagged as "deferred" placeholders here
        // and picked up by QuestShnReader::LoadAllDeferred() below; they
        // use a dedicated parser instead of the generic ShnFile loader.
        ShnRegistry::Get().LoadAll(dataRoot);
        QuestShnReader::Get().LoadAllDeferred();
        // Build typed group accessors (Item/Mob/Skill/Map/AbState/...).
        BindAllGroupTables();
        BindAllMiscTables();
        BindAllMoreTables();
        BindAllExtendedTables();
        // had no runtime reader: ItemActionEffect / ItemActionCondition /
        // CharacterTitleData / ActionRangeFactor / GTIServer / StateField /
        // MIDungeon+MIDServer / SubAbState.
        BindTypedSchemaConsumers();
        // columns. Any column that was parsed but never asked-for shows up
        // as a WARN line in the boot log. Same pass for the World/*.txt
        // (TableScript) loaders so MobRegen / MobRoam / MobAttackSequence /
        // MobSetting/Action / NPCItemList / NPCAction / BlockInfo all
        // surface their unread columns at boot.
        ShnAudit_EmitReport(ShnRegistry::Get());
        TsAudit_EmitReport();
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

        // FEATURE: combat-data-load -- Battle.cpp queries each of these
        // tables on every damage roll (LevelGapTable step 3, MobResist
        // step 4/8, ExpRecalc on Kill). They were defined but never
        // populated; without these calls every lookup returned the
        // hard-coded neutral fallback (1000 = 1.0x), which silently
        // disabled per-level PvP scaling, mob element/dmg-type resist
        // and proper exp curves.
        LevelGapTable    ::Get().Load(shineRoot);
        MobResistTable   ::Get().Load(shineRoot);
        ExpRecalcTable   ::Get().Load(shineRoot);
        MoverMainTable   ::Get().Load(shineRoot);
        MoverAbilityTable::Get().Load(shineRoot);

        // FEATURE: world-data-load -- World*.txt tables that expose a
        // singleton-style Get().Load(rRoot) entry but were never called
        // at boot. Each one drives a real runtime path:
        //   QuestTable           -> quest catalog (NPC dialog / kill credit)
        //   RecallCoordTable     -> death return-to-town coords
        //   DamageByAngleTable   -> per-skill angle damage scalar
        //   DamageBySoulTable    -> per-soul-type damage scalar
        //   ItemUseFunctionTable -> item-use opcode dispatch (capsules,
        //                           teleport scrolls, scrolls...)
        //   RandomOptionTable    -> random-option roll on equip drop
        //   ItemDropGroupTable   -> mob drop tables
        //   PineScriptTable      -> ScenarioBookShelf script catalog
        //   SubLayerInteractTable-> map sublayer trigger volumes
        QuestTable           ::Get().Load(shineRoot);
        RecallCoordTable     ::Get().Load(shineRoot);
        DamageByAngleTable   ::Get().Load(shineRoot);
        DamageBySoulTable    ::Get().Load(shineRoot);
        ItemUseFunctionTable ::Get().Load(shineRoot);
        RandomOptionTable    ::Get().Load(shineRoot);
        ItemDropGroupTable   ::Get().Load(shineRoot);
        PineScriptTable      ::Get().Load(shineRoot);
        SubLayerInteractTable::Get().Load(shineRoot);
        // FEATURE: world-creation -- 5 additional World/*.txt binders
        // wired here. Each one was previously parsed-but-ignored:
        //   ItemDropTable.txt    per-mob drop matrix (45 slots/row)
        //   ItemOptions.txt      per-(degree,type) random option curve
        //   MiscDataTable.txt    SkillBreedMob (skill-spawned sub-mobs)
        //   QuestParser.txt      class-name -> id token table
        //   TreasureReward.txt   treasure-bag contents (Container+Content)
        ItemDropTableW       ::Get().Load(shineRoot);
        ItemOptionsTable     ::Get().Load(shineRoot);
        MiscDataTable        ::Get().Load(shineRoot);
        QuestParserTable     ::Get().Load(shineRoot);
        TreasureRewardTable  ::Get().Load(shineRoot);

        // FEATURE: shn-derived-data-load -- post-registry singletons
        // that pull from already-loaded ShnRegistry tables. Order
        // matters: ShnRegistry::LoadAll() ran above, so everything
        // these depend on is parsed and resident.
        AbnormalStateDictionary::Get().Load();   // abstate -> id mapping
        SetItemData            ::Get().Load();   // set bonuses
        MultiHitTable          ::Get().Load();   // multi-hit combo data
        GuildAcademy           ::Get().Load();   // academy tutorial steps
        MinimonDataBox         ::Get().Load();   // pet datatables
        SpamerPenaltyDataBox   ::Get().Load();   // anti-spam thresholds

        // FEATURE: portals + lucky-capsule + casino -- per-system
        // ingest from ShnRegistry. Each system's binders log their
        // own row counts.
        Link                  ::Get().Load();    // map portals (3 types)
        TownPortalSystem      ::Get().Load();    // town-portal scroll list
        LuckyCapsuleSystem    ::Get().Load();    // red/blue/yellow capsules
        GambleSystem          ::Get().BindAllCasinoTables();
        // World/NPCAction.txt -- per-NPC dialog page button list.
        NPCActionTable::Get().Load(shineRoot);

        // Build the per-zone mob spawn driver. After this, every populated
        // MobRegen group has an entry in MobSpawnSystem and `Tick()` will
        // top each one up to its MobNum target.
        MobSpawnSystem::Get().LoadAll();

        ZoneServer::Get().Init(m_kInfo.GetU16("Zone.Id", 0));
        RegisterZoneHandlers();

        // Inter-broadcast consumers (kind=3 daily reset, kind=4 NPC
        // schedule). Adding new subsystems: append a Register() call
        // here -- the WMClient dispatcher fans every kind=3/4 broadcast
        // through these sinks. Keep handlers idempotent and side-effect
        // bounded; they run on the inbound packet thread.
        DailyResetSink::Get().Register(&ZoneService::OnDailyResetTick);
        NpcScheduleSink::Get().Register(&ZoneService::OnNpcScheduleTick);

        if (!m_kIOCP.Start()) return false;

        // Outbound link to the Character DB exe. Failure is non-fatal -- the
        // engine boots without it and players use provisional fill.
        CharDBClient::Get().Connect(&m_kIOCP,
            m_kInfo.GetString("CharDB.Ip",   "127.0.0.1"),
            m_kInfo.GetU16   ("CharDB.Port", 27602));

        // Outbound link to the GameLog DB exe. Battle::Kill, drop pickups
        // and trade flushes route through this connection.
        GameLogClient::Get().Connect(&m_kIOCP,
            m_kInfo.GetString("GameLog.Ip",   "127.0.0.1"),
            m_kInfo.GetU16   ("GameLog.Port", 27603));

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
        EstateServer::Get().Tick(GTimer::NowMillis());
        ExpeditionSystem::Get().Tick();
        MobSpawnSystem::Get().Tick(GTimer::NowMillis());
        MobAIRunner   ::Get().Tick(GTimer::NowMillis());
        GuildWarManager::Tick();
        GuildTournamentSystem::Tick();
        ChargedEffectManager::Get().Tick();
        WMClient::Get().Heartbeat();
    }
    virtual void OnStop() {
        m_kAcceptor.Stop(); m_kIOCP.Stop();
        WMClient::Get().Disconnect();
        GameLogClient::Get().Disconnect();
        CharDBClient::Get().Disconnect();
        ZoneServer::Get().Shutdown();
        DataBox::Get().Shutdown();
        m_kLua.Close();
    }
private:
    static void OnDailyResetTick(uint32 uiDayKey) {
        // The CharDB exe (World00_Character) owns the per-quest reset
        // table and runs p_Daily_Reset against the actual rows. Zone
        // side just bumps a process-wide last-reset stamp so any code
        // that gates on "today" (mob counters, attendance flags, etc.)
        // can observe the new day without a per-frame poll.
        SHINELOG_INFO("Zone: applied daily reset for day=%u", uiDayKey);
    }
    static void OnNpcScheduleTick(uint32 uiNpcId, uint16 uiMapId, bool bSpawn) {
        // NPC schedule fanout. The actual spawn / despawn lives in
        // ShineNPCTable; we emit a log line here and leave the spawn
        // body to ShineNPCTable::SetActive (or equivalent) once the
        // schedule consumer is wired -- this is a hook point so the
        // dispatcher path is exercised end-to-end.
        SHINELOG_INFO("Zone: NPC schedule npc=%u map=%u %s",
                      uiNpcId, (uint32)uiMapId, bSpawn ? "ON" : "OFF");
    }

    ServerInfo      m_kInfo;
    IOCPManager     m_kIOCP;
    Socket_Acceptor m_kAcceptor;
    DataReader      m_kReader;
    LuaRuntime      m_kLua;
};

} // namespace fiesta

int main(int argc, char** argv) { fiesta::ZoneService s; return s.Run(argc, argv); }
