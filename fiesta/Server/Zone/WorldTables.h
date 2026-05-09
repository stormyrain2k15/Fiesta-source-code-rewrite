// Server/Zone/WorldTables.h
// 09 / 12 / 15 -- typed loaders for the remaining World/*.txt files that
// drive significant runtime behaviour:
//
//   QuestTable             -- World/Quest.txt (6 sub-tables)
//   ExpRecalcTable         -- World/ExpRecalculation.txt (party + level-diff bonus)
//   RecallCoordTable       -- World/RecallCoord.txt (recall-scroll dest map)
//   DamageByAngleTable     -- World/DamageByAngle.txt (chr/mob hit-angle scaler)
//   DamageBySoulTable      -- World/DamageBySoul.txt (DemandSoul x SoulNN matrix)
//   ItemUseFunctionTable   -- World/ItemUseFunction.txt (function dispatch)
//   RandomOptionTable      -- World/RandomOptionTable.txt (drop random options)
//   ItemDropGroupTable     -- World/ItemDropGroup.txt (drop group resolver)
//   ItemOptionsTable       -- World/ItemOptions.txt (option roll table)
//   PineScriptTable        -- World/PineScript.txt (load list of .ps scripts)
//   SubLayerInteractTable  -- World/SubLayerInteract.txt (visibility/attackable matrix)
//   TreasureRewardTable    -- World/TreasureReward.txt (loot box content)
//   MiscDataTable          -- World/MiscDataTable.txt (skill-breed-mob)
//   NPCActionTable         -- World/NPCAction.txt (NPCCondition routing)
//
// All loaders are typed wrappers over `TableScriptFile` -- they parse once
// at zone start and expose row lookups for the runtime systems.
//
// EVIDENCE: DATA_CONFIRMED  source: project-owner-supplied World/*.txt.
#ifndef FIESTA_ZONE_WORLDTABLES_H
#define FIESTA_ZONE_WORLDTABLES_H
#include "../Shared/ShineTypes.h"
#include <map>
#include <string>
#include <vector>

namespace fiesta {

// =============================================================================
// QuestTable
// =============================================================================
struct QuestHeader {
    uint16      uiHandle;
    std::string kTitle;
    uint16      uiLevLow, uiLevHigh;
    std::string kReceiveScript;
    std::string kMissionScript;
    std::string kProgressScript;
    std::string kCompleteScript;
    std::string kStartNPC;
    std::string kStopNPC;
    uint16      uiTimeLimitMin;
};
struct QuestRewardRow {
    uint16      uiHandle;
    uint8       uiFlag;
    std::string kType;
    uint32      uiQuantity;
    std::string kItemName;
    uint8       uiItemUpgrade;
};
struct QuestLootRow {
    uint16      uiHandle;
    std::string kMob;
    std::string kItem;
    uint8       uiNumber;
    uint16      uiPermill;
};
struct QuestHuntRow {
    uint16      uiHandle;
    std::string kMob;
    uint16      uiNumber;
};
struct QuestProduceRow {
    uint16      uiHandle;
    std::string aRawItem[4];
    std::string kToItem;
    std::string kMap;
    uint32      uiCenterX, uiCenterY, uiRegion;
};

class QuestTable {
public:
    static QuestTable& Get();
    bool Load(const std::string& rRoot);

    const QuestHeader*    Header(uint16 uiHandle) const;
    const std::vector<QuestRewardRow>&  RewardsFor(uint16 uiHandle) const;
    const std::vector<QuestLootRow>&    LootsFor  (uint16 uiHandle) const;
    const std::vector<QuestHuntRow>&    HuntsFor  (uint16 uiHandle) const;
    const std::vector<QuestProduceRow>& ProducesFor(uint16 uiHandle) const;
    bool  IsMeeting(uint16 uiHandle) const;

private:
    QuestTable() {}
    std::map<uint16, QuestHeader>      m_kHeaders;
    std::map<uint16, std::vector<QuestRewardRow> >  m_kRewards;
    std::map<uint16, std::vector<QuestLootRow>   >  m_kLoots;
    std::map<uint16, std::vector<QuestHuntRow>   >  m_kHunts;
    std::map<uint16, std::vector<QuestProduceRow> > m_kProduces;
    std::map<uint16, bool>             m_kMeetings;
};

// =============================================================================
// ExpRecalcTable
// =============================================================================
class ExpRecalcTable {
public:
    static ExpRecalcTable& Get();
    bool Load(const std::string& rRoot);

    // Multipliers x1000 (1000 = 1.0x).
    int32 PartyBonus(uint8 uiPartyMembers) const;       // ByPartyMem
    int32 LevelDiffBonus(int32 nLevelDiff) const;       // ByLevelDiff
private:
    ExpRecalcTable() {}
    int32 m_aPartyBonus[8];
    std::map<int32, int32> m_kLevelDiffBonus;
};

// =============================================================================
// RecallCoordTable
// =============================================================================
struct RecallCoordRow {
    std::string kItemIndex;
    uint32      uiItemIdent;
    std::string kMapName;
    uint16      uiLinkX, uiLinkY;
};
class RecallCoordTable {
public:
    static RecallCoordTable& Get();
    bool Load(const std::string& rRoot);
    const RecallCoordRow* FindByItem(const std::string& rItemIndex) const;
private:
    RecallCoordTable() {}
    std::map<std::string, RecallCoordRow> m_kRows;
};

// =============================================================================
// DamageByAngleTable / DamageBySoulTable
// =============================================================================
class DamageByAngleTable {
public:
    static DamageByAngleTable& Get();
    bool Load(const std::string& rRoot);
    int32 ChrAttackerScalerX1k(uint16 uiAngleDeg) const;
    int32 MobAttackerScalerX1k(uint16 uiAngleDeg) const;
private:
    DamageByAngleTable() {}
    std::map<uint16, int32> m_kChr;
    std::map<uint16, int32> m_kMob;
};

class DamageBySoulTable {
public:
    static DamageBySoulTable& Get();
    bool Load(const std::string& rRoot);
    int32 BonusX1k(uint8 uiDemandSoul, uint8 uiSoulCount) const;   // 0..7 each
private:
    DamageBySoulTable() {}
    int32 m_aMatrix[8][8];                              // 1000 default
};

// =============================================================================
// ItemUseFunctionTable
// =============================================================================
struct ItemUseFunctionRow {
    std::string kItemIndex;
    uint8       uiBroadcast;
    std::string kUseFunction;     // dispatched by name in Inventory::UseItem
};
class ItemUseFunctionTable {
public:
    static ItemUseFunctionTable& Get();
    bool Load(const std::string& rRoot);
    const ItemUseFunctionRow* Find(const std::string& rItemIndex) const;
private:
    ItemUseFunctionTable() {}
    std::map<std::string, ItemUseFunctionRow> m_kRows;
};

// =============================================================================
// RandomOptionTable / ItemOptionsTable / ItemDropGroupTable / TreasureRewardTable
// =============================================================================
struct RandomOptionRow {
    std::string kDropItemIndex;
    uint8 uiOptionHide, uiMinOpCount, uiMaxOpCount;
    uint16 aMin[5];   // Str/Con/Dex/Int/Men minima
    uint16 aMax[5];
};
class RandomOptionTable {
public:
    static RandomOptionTable& Get();
    bool Load(const std::string& rRoot);
    const RandomOptionRow* Find(const std::string& rDropItemIndex) const;
private:
    RandomOptionTable() {}
    std::map<std::string, RandomOptionRow> m_kRows;
};

struct ItemDropGroupRow {
    std::string kItemGroupIdx;
    std::string kItemID;
    uint16      uiMinQty, uiMaxQty;
    uint16      aUpgradeChance[16];
};
class ItemDropGroupTable {
public:
    static ItemDropGroupTable& Get();
    bool Load(const std::string& rRoot);
    const ItemDropGroupRow* Find(const std::string& rItemGroupIdx) const;
private:
    ItemDropGroupTable() {}
    std::map<std::string, ItemDropGroupRow> m_kRows;
};

// =============================================================================
// PineScriptTable / SubLayerInteractTable / NPCActionTable / MiscDataTable
// =============================================================================
class PineScriptTable {
public:
    static PineScriptTable& Get();
    bool Load(const std::string& rRoot);
    const std::vector<std::string>& Scripts() const { return m_kScripts; }
private:
    PineScriptTable() {}
    std::vector<std::string> m_kScripts;
};

class SubLayerInteractTable {
public:
    static SubLayerInteractTable& Get();
    bool Load(const std::string& rRoot);

    // Visibility / attack matrices (rows = my-state, cols = other-state).
    bool CanSeeMyBrief(uint8 uiMyState, uint8 uiOtherState) const;
    bool CanAttack    (uint8 uiMyState, uint8 uiOtherState) const;
private:
    SubLayerInteractTable() {}
    uint8 m_aBrief [8][8];
    uint8 m_aAttack[8][8];
};

class NPCActionTable {
public:
    static NPCActionTable& Get();
    bool Load(const std::string& rRoot);
    // NPCCondition rows -- expose as raw rows; the per-condition evaluator
    // lives in the NPC AI tick.
    struct Row {
        uint8 uiConditionID;
        std::string kConditionA, kTypeA;
        uint32 uiAX, uiAY;
        std::string kConditionB, kTypeB;
        uint32 uiBX, uiBY;
    };
    const std::vector<Row>& Rows() const { return m_kRows; }
private:
    NPCActionTable() {}
    std::vector<Row> m_kRows;
};

// Walk PineScript.txt and load every ScenarioBookShelf entry it lists,
// plus always-on Wedding / Guild scenarios. Returns the count of
// successfully loaded .ps scripts.
size_t LoadAllPineScripts(const std::string& rRoot);

// World/Karen.txt   -- "Karen" boss attack sequence (table AttSeq).
// World/MobChat.txt -- per-mob chat lines (PIECE/ATTACK/DAMAGED/DEAD/HELPMAIN/HELPSUB).
class KarenAttackTable {
public:
    static KarenAttackTable& Get();
    bool Load(const std::string& rRoot);
    const std::string& AttackAt(uint16 uiOrder) const;
    size_t Size() const { return m_kRows.size(); }
private:
    KarenAttackTable() {}
    std::map<uint16, std::string> m_kRows;
    std::string m_kEmpty;
};

class MobChatTable {
public:
    static MobChatTable& Get();
    bool Load(const std::string& rRoot);
    enum eMobChatBucket { MC_PIECE = 0, MC_ATTACK, MC_DAMAGED, MC_DEAD,
                          MC_HELPMAIN, MC_HELPSUB, MC_BUCKETS };
    // Returns one of (Script0..Script3) sampled by Rate0/Rate1; empty if none.
    std::string Pick(eMobChatBucket eBucket, const std::string& rMobIndex) const;
private:
    MobChatTable() {}
    struct Row { std::string kMobIndex; uint32 uiRate0, uiRate1;
                 std::string aScript[4]; };
    std::vector<Row> m_kBucket[MC_BUCKETS];
    std::map<std::string, std::vector<size_t> > m_kIndex[MC_BUCKETS];
};

} // namespace fiesta
#endif
