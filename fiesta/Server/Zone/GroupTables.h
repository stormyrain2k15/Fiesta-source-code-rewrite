// Server/Zone/GroupTables.h
// 06+ -- Typed wrappers around `ShnRegistry`-loaded SHN tables, grouped by
// game system. Each group class holds a non-owning pointer to the
// underlying ShnFile and exposes a Find()-style accessor that returns
// either a strongly-typed POD struct or a row index plus column-name
// helpers.
// Why string-keyed reads instead of column-index reads?  The original
// SHN file headers carry the column NAMES, so referencing them by name is
// resilient to the data team swapping column order between drops. Each
// accessor also caches the column index it cares about on first use so
// the per-row hot path is O(1).
//                                   Shine-1/*.shn (2026-02 drop).
#ifndef FIESTA_ZONE_GROUPTABLES_H
#define FIESTA_ZONE_GROUPTABLES_H
#include "../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace fiesta {

// =============================================================================
//  Forward-declared row POD structs -- one per typed accessor below.
//  Only the columns each system actually consults are mirrored; everything
//  else stays accessible through the underlying ShnFile via raw row index.
// =============================================================================

// ----- Item* (18 SHNs) ------------------------------------------------------
// Extended in pass 1.23 to carry every gameplay-relevant column from
// ItemInfo.shn (91 fields). Class permissions are kept as a packed 32-bit
// mask -- bit `eClassEnum` is set when ItemInfo.<ClassTag> != 0.
//   bit 0  Fig   bit 1  Cfig  bit 2  War   bit 3  Gla   bit 4  Kni
//   bit 5  Cle   bit 6  Hcle  bit 7  Pal   bit 8  Hol   bit 9  Gua
//   bit 10 Arc   bit 11 Harc  bit 12 Sco   bit 13 Sha   bit 14 Ran
//   bit 15 Mag   bit 16 Wmag  bit 17 Enc   bit 18 Warl  bit 19 Wiz
//   bit 20 Jok   bit 21 Chs   bit 22 Cru   bit 23 Cls   bit 24 Ass
struct ItemInfoRow {
    uint32      uiID;
    std::string kInxName;
    std::string kName;
    uint32      uiType;
    uint32      uiClass;
    uint32      uiMaxLot;
    uint32      uiEquip;             // equip-slot bitmask (-> EquipEnumChanger)
    uint32      uiItemAuctionSubGroup;
    uint32      uiItemGradeType;
    uint32      uiTwoHand;
    uint32      uiAtkSpeed;
    uint32      uiDemandLv;
    uint32      uiGrade;
    uint32      uiMinWC, uiMaxWC, uiAC, uiMinMA, uiMaxMA, uiMR;
    uint32      uiWCRate, uiMARate, uiACRate, uiMRRate;
    uint32      uiCriRate, uiCriMinWc, uiCriMaxWc, uiCriMinMa, uiCriMaxMa, uiCrlTB;
    uint32      uiMaxHP, uiMaxSP, uiMaxAP;
    uint32      uiClassMask;         // packed class permissions (see comment)
    uint32      uiBuyPrice;
    uint32      uiSellPrice;
    uint32      uiBuyFame;
    uint32      uiBuyGToken;
    uint32      uiBuyGBCoin;
    uint32      uiWeaponType;
    uint32      uiArmorType;
    uint32      uiUpLimit;           // max enchant level
    uint32      uiUpSucRatio;
    uint32      uiUpLuckRatio;
    uint32      uiUpResource;
    uint32      uiBasicUpInx;
    uint32      uiAddUpInx;
    uint32      uiTH, uiTB;
    uint32      uiShieldAC;
    uint32      uiHitRatePlus, uiEvaRatePlus;
    uint32      uiMACriPlus, uiCriDamPlus, uiMagCriDamPlus;
    uint8       bPutOnBelonged;       // bind-on-equip
    uint8       bBelonged;            // bind-on-acquire (always BoP)
    uint8       bNoDrop;
    uint8       bNoSell;
    uint8       bNoStorage;
    uint8       bNoTrade;
    uint8       bNoDelete;
    std::string kTitleName;
    std::string kItemUseSkill;        // skill triggered on Use()
    std::string kSetItemIndex;        // SetItem grouping key
    uint32      uiItemFunc;
    uint8       bAutoMon;             // auto-summon mover when used
};
// Extended ItemInfoServer.shn (25 fields) -- every column is now consumed:
//   * kMarketIndex      -> MarketSearchInfo lookup key
//   * uiKingdom         -> per-kingdom drop weight (Rou/Eld/...) projected
//                          via DropResolver
//   * kDropGroupA/B     -> chained ItemDropTable IDs
//   * kRandomOptionDropGroup -> RandomOption roll set
//   * uiVanish          -> seconds-on-ground before despawn
//   * uiLooting         -> belong-policy override (FREE/MASTER/PARTY)
//   * uiDropRateKilledByMob/Player -> rate scalars for PvE / PvP drops
//   * uiISET_Index      -> CompoundSetItem group
//   * bKQItem etc.      -> KingdomQuest gating
struct ItemInfoServerRow {
    uint32      uiID;
    std::string kInxName;
    std::string kMarketIndex;
    uint32      uiRou, uiEld, uiUrg, uiAll;
    uint32      uiFer, uiKas, uiSad, uiKor, uiVer, uiMys;
    std::string kDropGroupA;
    std::string kDropGroupB;
    std::string kRandomOptionDropGroup;
    uint32      uiVanishSecs;
    uint32      uiLootingMode;
    uint16      uiDropRateKilledByMob;
    uint16      uiDropRateKilledByPlayer;
    uint32      uiISETIndex;
    uint8       bKQItem;
    uint8       bPK_KQ_USE;
    uint8       bKQ_Item_Drop;
    uint8       bPreventAttack;
};
struct ItemUpgradeRow {
    uint32      uiID;
    std::string kUpgrade;
    uint32      uiSucRate;
    uint32      uiFailGrade;
    uint32      uiResource;
    uint32      uiQty;
};
struct ItemActionRow {
    uint32      uiID;
    std::string kAction;
    uint32      uiCondition;
    uint32      uiEffect;
    uint32      uiCoolTime;
};

// ----- Mob* (14 SHNs) -------------------------------------------------------
struct MobInfoRow {
    uint32      uiID;
    std::string kInxName;
    std::string kName;
    uint32      uiLevel, uiMaxHP, uiWalkSpeed, uiRunSpeed;
    uint32      uiIsNPC, uiSize, uiWeaponType, uiArmorType,
                uiGradeType, uiType, uiIsPlayerSide, uiAbsoluteSize;
    uint32      uiExp;
    uint32      uiMoney;     // base gold drop (MobInfo.shn "Money" column;
                             // 0 if missing -- MoneyRateX1k still applies
                             // proportionally so Lucky Hour stays consistent).
};
struct MobSpeciesRow {
    uint32      uiID;
    std::string kMobName;
};
struct MobLifeTimeRow { uint32 uiID; uint32 uiLifeTimeMs; };

// ----- Skill family ---------------------------------------------------------
struct ActiveSkillRow {
    uint32      uiID;
    std::string kInxName;
    uint32      uiClass;
    uint32      uiMaxLevel;
    uint32      uiSP;
    uint32      uiCoolMs;
    uint32      uiCastMs;
};
struct ActiveSkillInfoServerRow {
    uint32      uiID;
    uint32      uiBaseDamage;
    uint32      uiTargetType;
    uint32      uiRange;
    uint32      uiAggroBonus;
};
struct PassiveSkillRow { uint32 uiID; std::string kInxName; uint32 uiClass; uint32 uiMaxLevel; };
struct AreaSkillRow    { uint32 uiID; uint32 uiRange; uint32 uiDuration; uint32 uiTickMs; };

// ----- Map family -----------------------------------------------------------
struct MapInfoRow {
    uint32      uiID;
    std::string kMapName;
    std::string kName;
    uint32      uiIsWMLink;
    int32       iRegenX, iRegenY;
    uint32      uiKingdomMap;
    std::string kFolder;
    uint32      uiInSide;
    uint32      uiSight;
};

// ----- AbState family -------------------------------------------------------
struct AbStateRow {
    uint32      uiID;
    std::string kInxName;
    uint32      uiAbStataIndex;
    uint32      uiKeepTimeRatio;
    uint32      uiKeepTimePower;
    uint32      uiStateGrade;
    uint32      uiAbStateSaveType;
};

// ----- Charged family is in ChargedEffect.h --------------------------------

// ----- Hair / Face / Class / Title (player presentation) -------------------
struct HairInfoRow      { uint32 uiID; std::string kIndexName; std::string kName; uint32 uiGrade; uint32 uiFighter, uiArcher, uiCleric, uiMage; };
struct FaceInfoRow      { uint32 uiID; std::string kFaceName; uint32 uiGrade; };
struct ClassNameRow     { uint32 uiClassID; std::string kPrefix; std::string kEngName; std::string kLocalName; };
struct CharacterTitleRow{ uint32 uiID; std::string kInxName; uint32 uiGrade; };
struct WeaponTitleRow   { uint32 uiID; std::string kInxName; };

// ----- Pup (pet) family -----------------------------------------------------
struct PupMainRow       { uint32 uiPupID; std::string kPupIDX; std::string kItemIDX; uint32 uiPupSpeed; };
struct PupServerRow     { uint32 uiPupID; uint32 uiSpawnHP; uint32 uiSpawnSP; uint32 uiCooldown; uint32 uiAggro; };
struct PupCaseRow       { uint32 uiCaseID; uint32 uiPriority; uint32 uiAction; };

// ----- Mover family (mounts) -----------------------------------------------
struct MoverMainRow     { uint32 uiMoverID; std::string kMoverIDX; uint32 uiCastMs; uint32 uiCoolMs; uint32 uiRun; uint32 uiWalk; uint32 uiHours; uint32 uiMaxSlot; };
struct MoverHGRow       { uint32 uiID; std::string kInxName; };
struct MoverItemRow     { uint32 uiID; std::string kItemIDX; };

// ----- MiniHouse family ----------------------------------------------------
struct MiniHouseRow     { uint32 uiID; std::string kInxName; std::string kName; uint32 uiGrade; uint32 uiDurationHour; };
struct MiniHouseFurnRow { uint32 uiID; std::string kInxName; uint32 uiCategory; uint32 uiHP; };

// ----- Guild family --------------------------------------------------------
struct GuildAcademyRow         { std::string kBuffName; uint32 uiLeastJoinTime; uint32 uiRankAggregation; };
struct GuildGradeDataRow       { uint32 uiGrade; uint32 uiMaxMember; uint32 uiStorageSlot; };
struct GuildTournamentRow      { uint32 uiGTNo; uint32 uiMatchNumber; uint32 uiPrizeCoin; };
struct GuildTournamentRewardRow{ uint32 uiRank; uint32 uiCoin; uint32 uiFame; };

// ----- Collect family ------------------------------------------------------
struct CollectCardRow      { uint32 uiCardID; std::string kItemInx; uint32 uiCardGradeType; uint32 uiMobGroup; };
struct CollectCardRewardRow{ uint32 uiID; uint32 uiPercent; std::string kReward; uint32 uiQty; uint32 uiBonus; };

// ----- Random / Set / Grade / Misc ------------------------------------------
struct RandomOptionRow2    { std::string kDropItemIndex; uint32 uiRandomOptionType; int32 iMin; int32 iMax; uint32 uiTypeDropRate; };
struct SetItemRow          { uint32 uiIndex; uint32 uiPiece; uint32 uiEffect; };
// GradeItemOption.shn (20 wide cols): per-item-grade stat boost set. The
// runtime keeps the full wide row -- consumers (EquipSummaryBuilder) sum
// across the 20 columns when an item's grade matches.
struct GradeItemOptionRow {
    uint32 uiItemIndex;
    uint16 uiSTR, uiCON, uiDEX, uiINT, uiMEN;
    uint16 uiResistPoison, uiResistDeaseas, uiResistCurse, uiResistMoveSpdDown;
    uint16 uiCritical, uiToHitRate, uiToHitPlus, uiToBlockRate, uiToBlockPlus;
    uint16 uiMaxHP, uiMaxSP, uiMoveSpdRate, uiAbsoluteAttack, uiPickupLimit;
};

// ----- KingdomQuest family --------------------------------------------------
struct KingdomQuestRow     { uint32 uiID; std::string kName; uint32 uiMinLevel; uint32 uiMaxLevel; uint32 uiTeamSize; };
struct KingdomQuestRewRow  { uint32 uiID; uint32 uiRank; std::string kReward; uint32 uiQty; };
struct KingdomQuestMapRow  { uint32 uiID; std::string kMapName; uint32 uiX, uiY; };

// =============================================================================
//  Group accessors. Each Bind()s once at boot and exposes Find() / Row().
// =============================================================================
class ItemTables {
public:
    static ItemTables& Get();
    void Bind();

    const ItemInfoRow*       FindItem    (uint32 uiID) const;
    const ItemInfoRow*       FindByInx   (const std::string& rInx) const;
    const ItemInfoServerRow* FindServer  (uint32 uiID) const;
    const ItemInfoServerRow* FindServerByInx(const std::string& rInx) const;
    const ItemUpgradeRow*    FindUpgrade (uint32 uiID) const;
    const ItemActionRow*     FindAction  (uint32 uiID) const;

    // ItemInfoServer-derived projections used at gameplay time.
    //   KingdomWeight : 1..1000 multiplier for the supplied kingdom tag
    //                   ("Rou","Eld","Urg","Fer","Kas","Sad","Kor","Ver",
    //                    "Mys"); "All" is the fallback.
    //   VanishSecs    : seconds the item lingers on the ground.
    //   LootingMode   : 0=FREE 1=MASTER 2=PARTY (matches the value in
    //                   ItemInfoServer.looting).
    uint32  KingdomWeight(uint32 uiID, const std::string& rTag) const;
    uint32  VanishSecs   (uint32 uiID) const;
    uint32  LootingMode  (uint32 uiID) const;

    size_t ItemCount()   const { return m_kItems.size(); }
    size_t ServerCount() const { return m_kServer.size(); }
private:
    std::vector<ItemInfoRow>                  m_kItems;
    std::map<uint32, size_t>                  m_kItemById;
    std::map<std::string, size_t>             m_kItemByInx;
    std::vector<ItemInfoServerRow>            m_kServer;
    std::map<uint32, size_t>                  m_kServerById;
    std::map<std::string, size_t>             m_kServerByInx;
    std::vector<ItemUpgradeRow>               m_kUpgrade;
    std::map<uint32, size_t>                  m_kUpgradeById;
    std::vector<ItemActionRow>                m_kActions;
    std::map<uint32, size_t>                  m_kActionsById;
};

class MobTables {
public:
    static MobTables& Get();
    void Bind();

    const MobInfoRow*       FindMob   (uint32 uiID) const;
    const MobInfoRow*       FindByInx (const std::string& rInx) const;
    const MobSpeciesRow*    FindSpec  (uint32 uiID) const;
    const MobLifeTimeRow*   FindLife  (uint32 uiID) const;
    size_t Count() const { return m_kMobs.size(); }
private:
    std::vector<MobInfoRow>                   m_kMobs;
    std::map<uint32, size_t>                  m_kMobById;
    std::map<std::string, size_t>             m_kMobByInx;
    std::vector<MobSpeciesRow>                m_kSpec;
    std::map<uint32, size_t>                  m_kSpecById;
    std::vector<MobLifeTimeRow>               m_kLife;
    std::map<uint32, size_t>                  m_kLifeById;
};

class SkillTables {
public:
    static SkillTables& Get();
    void Bind();

    const ActiveSkillRow*           FindActive (uint32 uiID) const;
    const ActiveSkillInfoServerRow* FindActiveS(uint32 uiID) const;
    const PassiveSkillRow*          FindPassive(uint32 uiID) const;
    const AreaSkillRow*             FindArea   (uint32 uiID) const;
    size_t ActiveCount()  const { return m_kActive.size(); }
    size_t PassiveCount() const { return m_kPassive.size(); }
private:
    std::vector<ActiveSkillRow>                  m_kActive;
    std::map<uint32, size_t>                     m_kActiveById;
    std::vector<ActiveSkillInfoServerRow>        m_kActiveS;
    std::map<uint32, size_t>                     m_kActiveSById;
    std::vector<PassiveSkillRow>                 m_kPassive;
    std::map<uint32, size_t>                     m_kPassiveById;
    std::vector<AreaSkillRow>                    m_kArea;
    std::map<uint32, size_t>                     m_kAreaById;
};

class MapTables {
public:
    static MapTables& Get();
    void Bind();
    const MapInfoRow* Find(uint32 uiID) const;
    const MapInfoRow* FindByName(const std::string& rMapName) const;
    const std::vector<MapInfoRow>& Maps() const { return m_kMaps; }
    size_t Count() const { return m_kMaps.size(); }
private:
    std::vector<MapInfoRow>           m_kMaps;
    std::map<uint32, size_t>          m_kById;
    std::map<std::string, size_t>     m_kByName;
};

class AbStateTables {
public:
    static AbStateTables& Get();
    void Bind();
    const AbStateRow* Find(uint32 uiID) const;
    const AbStateRow* FindByInx(const std::string& rInx) const;
private:
    std::vector<AbStateRow>           m_kRows;
    std::map<uint32, size_t>          m_kById;
    std::map<std::string, size_t>     m_kByInx;
};

class PresentationTables {
public:
    static PresentationTables& Get();
    void Bind();
    const HairInfoRow*       FindHair (uint32 uiID) const;
    const FaceInfoRow*       FindFace (uint32 uiID) const;
    const ClassNameRow*      FindClass(uint32 uiClassID) const;
    const CharacterTitleRow* FindCharTitle(uint32 uiID) const;
    const WeaponTitleRow*    FindWeapTitle(uint32 uiID) const;
private:
    std::vector<HairInfoRow>          m_kHair;        std::map<uint32, size_t> m_kHairById;
    std::vector<FaceInfoRow>          m_kFace;        std::map<uint32, size_t> m_kFaceById;
    std::vector<ClassNameRow>         m_kClass;       std::map<uint32, size_t> m_kClassById;
    std::vector<CharacterTitleRow>    m_kCharTitle;   std::map<uint32, size_t> m_kCharTitleById;
    std::vector<WeaponTitleRow>       m_kWeapTitle;   std::map<uint32, size_t> m_kWeapTitleById;
};

class PupTables {
public:
    static PupTables& Get();
    void Bind();
    const PupMainRow*   FindMain  (uint32 uiPupID) const;
    const PupServerRow* FindServer(uint32 uiPupID) const;
    const PupCaseRow*   FindCase  (uint32 uiCaseID) const;
private:
    std::vector<PupMainRow>           m_kMain;        std::map<uint32, size_t> m_kMainById;
    std::vector<PupServerRow>         m_kServer;      std::map<uint32, size_t> m_kServerById;
    std::vector<PupCaseRow>           m_kCase;        std::map<uint32, size_t> m_kCaseById;
};

class MountTables {
public:
    static MountTables& Get();
    void Bind();
    const MoverMainRow*  FindMain (uint32 uiMoverID) const;
    const MoverItemRow*  FindItem (uint32 uiID) const;
private:
    std::vector<MoverMainRow>         m_kMain;       std::map<uint32, size_t> m_kMainById;
    std::vector<MoverItemRow>         m_kItem;       std::map<uint32, size_t> m_kItemById;
};

class MiniHouseTables {
public:
    static MiniHouseTables& Get();
    void Bind();
    const MiniHouseRow*     Find    (uint32 uiID) const;
    const MiniHouseFurnRow* FindFurn(uint32 uiID) const;
private:
    std::vector<MiniHouseRow>         m_kHouse;      std::map<uint32, size_t> m_kHouseById;
    std::vector<MiniHouseFurnRow>     m_kFurn;       std::map<uint32, size_t> m_kFurnById;
};

class GuildTables2 {  // distinct from GuildSystem.h; data tables only
public:
    static GuildTables2& Get();
    void Bind();
    const GuildGradeDataRow*        FindGrade (uint32 uiGrade) const;
    const GuildTournamentRow*       FindGT    (uint32 uiGTNo) const;
    const GuildTournamentRewardRow* FindReward(uint32 uiRank) const;
    const GuildAcademyRow*          GetAcademy() const { return m_kAcademy.empty() ? NULL : &m_kAcademy[0]; }
private:
    std::vector<GuildAcademyRow>            m_kAcademy;
    std::vector<GuildGradeDataRow>          m_kGrade;       std::map<uint32, size_t> m_kGradeById;
    std::vector<GuildTournamentRow>         m_kGT;          std::map<uint32, size_t> m_kGTById;
    std::vector<GuildTournamentRewardRow>   m_kReward;      std::map<uint32, size_t> m_kRewardByRank;
};

class CollectTables {
public:
    static CollectTables& Get();
    void Bind();
    const CollectCardRow*       FindCard(uint32 uiCardID) const;
    const CollectCardRewardRow* FindReward(uint32 uiID)   const;
    size_t CardCount() const { return m_kCards.size(); }
private:
    std::vector<CollectCardRow>             m_kCards;       std::map<uint32, size_t> m_kCardById;
    std::vector<CollectCardRewardRow>       m_kReward;      std::map<uint32, size_t> m_kRewardById;
};

class GradeRandomTables {
public:
    static GradeRandomTables& Get();
    void Bind();
    const GradeItemOptionRow* FindGrade(uint32 uiID) const;
    const SetItemRow*         FindSet  (uint32 uiIndex) const;
    size_t RandomCount() const { return m_kRandom.size(); }
private:
    std::vector<GradeItemOptionRow>         m_kGrade;       std::map<uint32, size_t> m_kGradeById;
    std::vector<SetItemRow>                 m_kSet;         std::map<uint32, size_t> m_kSetById;
    std::vector<RandomOptionRow2>           m_kRandom;
};

class KQTables {
public:
    static KQTables& Get();
    void Bind();
    const KingdomQuestRow*    FindKQ   (uint32 uiID) const;
    const KingdomQuestRewRow* FindRew  (uint32 uiID, uint32 uiRank) const;
    const KingdomQuestMapRow* FindMap  (uint32 uiID) const;
    size_t Count() const { return m_kKQ.size(); }
private:
    std::vector<KingdomQuestRow>            m_kKQ;          std::map<uint32, size_t> m_kKQById;
    std::vector<KingdomQuestRewRow>         m_kRew;
    std::vector<KingdomQuestMapRow>         m_kMap;         std::map<uint32, size_t> m_kMapById;
};

// One-call initializer: Binds every group above. Safe to call multiple times.
void BindAllGroupTables();

} // namespace fiesta
#endif
