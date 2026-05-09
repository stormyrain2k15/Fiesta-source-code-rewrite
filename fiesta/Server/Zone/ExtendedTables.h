// Server/Zone/ExtendedTables.h
// 06+ -- The remaining typed SHN accessors.  covered the headline
// data tables (Item / Mob / Skill / Map / AbState / Pup / Mover /
// MiniHouse / Guild / Collect / Quest / Spamer / SetEffect / Upgrade /
// Pup AI / Mob battle); this file mops up the *long tail* (110 SHN
// files). Every loader is a thin static singleton that caches the row
// data into a typed POD and a primary-key index map at boot.
// The original engine had one of these per `*Tab`/`*DataBox` C++ class
// (e.g. `BMPDataBox`, `ItemMixDataBox`, `MapBuffDataBox`, ...). We
// fold them into one TU because they all follow the same pattern --
// `Bind()` walks a single `ShnFile`, projects columns by name into a
// row struct, and pushes onto a `std::vector` while indexing into a
// `std::map`. Column reads via `ShnGetU32/Str/I32` keep us resilient
// to drop-to-drop column reorder (the file headers carry names).
//                                   (2026-02 drop, schema manifest at
//                                   /app/downloads/shn_schema_manifest.csv).
#ifndef FIESTA_ZONE_EXTENDEDTABLES_H
#define FIESTA_ZONE_EXTENDEDTABLES_H
#include "../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace fiesta {

// =============================================================================
//  Account-upgrade ladder (cash-shop accessory enhancement).
//  Region split: Global = AccUpgrade.shn, Brazil = BRAccUpgrade.shn.
// =============================================================================
class AccUpgradeTables {
public:
    static AccUpgradeTables& Get();
    void Bind();
    struct AccRow {
        uint32 uiID;
        uint32 uiCriFail, uiDownFail, uiNormalFail, uiCon, uiLuckySuc;
    };
    struct InfoRow {
        uint32      uiID;
        std::string kInxName;
        uint32      uiUpFactor;
        uint32      uiSuccess;
        uint32      uiMin1, uiMax1, uiMin2, uiMax2;
    };
    const AccRow*  FindAcc   (uint32 uiID) const;
    const InfoRow* FindInfo  (uint32 uiID) const;
    const AccRow*  FindBR    (uint32 uiID) const;
    const InfoRow* FindBRInfo(uint32 uiID) const;
private:
    std::vector<AccRow>   m_kAcc;     std::map<uint32,size_t> m_kAccById;
    std::vector<InfoRow>  m_kInfo;    std::map<uint32,size_t> m_kInfoById;
    std::vector<AccRow>   m_kBR;      std::map<uint32,size_t> m_kBRById;
    std::vector<InfoRow>  m_kBRInfo;  std::map<uint32,size_t> m_kBRInfoById;
};

// =============================================================================
//  Action / ActiveSkill grouping
// =============================================================================
class ActionExtraTables {
public:
    static ActionExtraTables& Get();
    void Bind();
    struct AESRow         { uint32 uiID; uint32 uiAbState; uint32 uiStrength; uint32 uiKeepTimeMs; };
    struct ViewRow        { uint32 uiID; std::string kInxName; uint32 uiAniNo; };
    struct SkillGroupRow  { uint32 uiID; std::string kGroupName; std::string kSkillInx; };
    const AESRow*        FindEffectAbState(uint32 uiID) const;
    const ViewRow*       FindActionView   (uint32 uiID) const;
    void                 SkillsByGroup    (const std::string& rGroup,
                                           std::vector<const SkillGroupRow*>& rOut) const;
private:
    std::vector<AESRow>        m_kAES;      std::map<uint32,size_t> m_kAESById;
    std::vector<ViewRow>       m_kView;     std::map<uint32,size_t> m_kViewById;
    std::vector<SkillGroupRow> m_kGroup;
    std::map<std::string, std::vector<size_t> > m_kByGroup;
};

// =============================================================================
//  Admin-only level-set, announce-feed, BMP (battle-map points)
// =============================================================================
class AdminLvSetTable {
public:
    static AdminLvSetTable& Get();
    void Bind();
    // Return the admin command level required to issue a particular SubLv.
    uint8 RequiredFor(uint32 uiCommandID) const;
private:
    std::map<uint32, uint8> m_kRows;
};
class AnnounceDataTable {
public:
    static AnnounceDataTable& Get();
    void Bind();
    struct Row { uint32 uiHandle; std::string kMessage; uint32 uiColor; };
    const Row* Find(uint32 uiHandle) const;
private:
    std::vector<Row> m_kRows; std::map<uint32, size_t> m_kByHandle;
};
class BMPTable {
public:
    static BMPTable& Get();
    void Bind();
    // Returns true if the [BMPIndex,Step] pair is a valid drawing entry.
    bool Has(uint32 uiBMPIndex, uint32 uiStep) const;
private:
    std::map<uint64, bool> m_kKeys;  // (bmp<<32) | step
};

// =============================================================================
//  Belong (origin/binding) + dice/dividend tables
// =============================================================================
class BelongTables {
public:
    static BelongTables& Get();
    void Bind();
    struct DiceRow      { uint32 uiID; uint32 uiBelongType; uint32 uiRate; };
    struct TypeInfoRow  { uint32 uiBelongType; uint32 uiCanTrade; uint32 uiCanDrop; uint32 uiCanStorage; };
    const DiceRow*     FindDice(uint32 uiID) const;
    const TypeInfoRow* FindType(uint32 uiBelongType) const;
private:
    std::vector<DiceRow>     m_kDice;  std::map<uint32,size_t> m_kDiceById;
    std::vector<TypeInfoRow> m_kType;  std::map<uint32,size_t> m_kTypeById;
};

// =============================================================================
//  Title state-server / chat color / hair color / generic color info
// =============================================================================
class CharTitleStateServerTable {
public:
    static CharTitleStateServerTable& Get();
    void Bind();
    struct Row { uint32 uiTitleID; int32 iStr,iCon,iDex,iInt,iMen; uint32 uiAtkSpd,uiCastSpd,uiCoolDown; };
    const Row* Find(uint32 uiTitleID) const;
private:
    std::vector<Row> m_kRows; std::map<uint32,size_t> m_kById;
};
class ChatColorTable {
public:
    static ChatColorTable& Get();
    void Bind();
    uint32 Color(uint32 uiChannelID) const;   // 0xRRGGBB or 0
private:
    std::map<uint32, uint32> m_kColor;
};
class ColorInfoTables {
public:
    static ColorInfoTables& Get();
    void Bind();
    struct ColorRow     { uint32 uiID; std::string kName; uint32 uiRGBA; };
    struct HairColorRow { uint32 uiID; std::string kName; uint32 uiRGBA; uint32 uiGrade; };
    const ColorRow*     FindColor(uint32 uiID)     const;
    const HairColorRow* FindHair (uint32 uiID)     const;
    size_t              ColorCount() const { return m_kColor.size(); }
    size_t              HairCount()  const { return m_kHair.size();  }
private:
    std::vector<ColorRow>     m_kColor; std::map<uint32,size_t> m_kColorById;
    std::vector<HairColorRow> m_kHair;  std::map<uint32,size_t> m_kHairById;
};

// =============================================================================
//  Collect-card extras (scoring / drop / star rate / titles).
// =============================================================================
class CollectExtrasTables {
public:
    static CollectExtrasTables& Get();
    void Bind();
    struct DropRow   { uint32 uiCardID; uint32 uiPercent; };
    struct GroupRow  { uint32 uiMobGroup; std::string kDescription; };
    struct MobGroupRow { uint32 uiMobGroup; std::string kMobInx; };
    struct StarRow   { uint32 uiCardGradeType; uint32 uiStar; uint32 uiPercent; };
    struct TitleRow  { uint32 uiCardSetID; uint32 uiTitleID; };
    int32 DropPercent(uint32 uiCardID) const;
    const std::string& GroupDesc(uint32 uiMobGroup) const;
    void               MobsInGroup(uint32 uiMobGroup, std::vector<std::string>& rOut) const;
    int32              StarPercent(uint32 uiGradeType, uint32 uiStar) const;
    int32              TitleForSet(uint32 uiCardSetID) const;
private:
    std::map<uint32, uint32>                       m_kDrop;
    std::map<uint32, std::string>                  m_kGroupDesc;
    std::map<uint32, std::vector<std::string> >    m_kGroupMobs;
    std::map<uint64, uint32>                       m_kStar;     // (grade<<32)|star
    std::map<uint32, uint32>                       m_kTitle;
    std::string                                    m_kEmpty;
};

// =============================================================================
//  Dice mini-game (a side activity tied to NPCs in towns)
// =============================================================================
class DiceTables {
public:
    static DiceTables& Get();
    void Bind();
    struct GameRow   { uint32 uiID; std::string kName; uint32 uiBet; uint32 uiTimeMs; };
    struct RateRow   { uint32 uiGameID; uint32 uiOutcome; uint32 uiRate; };
    struct DivRow    { uint32 uiGameID; uint32 uiTier;    uint32 uiDividend; };
    const GameRow*  FindGame    (uint32 uiID)     const;
    int32           Rate        (uint32 uiGameID, uint32 uiOutcome) const;
    int32           Dividend    (uint32 uiGameID, uint32 uiTier)    const;
private:
    std::vector<GameRow> m_kGame; std::map<uint32,size_t> m_kGameById;
    std::map<uint64, uint32> m_kRate; // (game<<32)|outcome
    std::map<uint64, uint32> m_kDiv;  // (game<<32)|tier
};

// =============================================================================
//  Enchant-socket success table + global error code table.
// =============================================================================
class EnchantSocketTable {
public:
    static EnchantSocketTable& Get();
    void Bind();
    int32 RatePerThousand(uint32 uiGrade, uint32 uiSocketSlot) const;
private:
    std::map<uint64, uint32> m_kRate; // (grade<<32)|slot
};
class ErrorCodeTable {
public:
    static ErrorCodeTable& Get();
    void Bind();
    const std::string& Message(uint32 uiCode) const;
    uint32             CodeOf (const std::string& rKey) const;  // reverse
private:
    std::map<uint32, std::string> m_kByCode;
    std::map<std::string, uint32> m_kByKey;
    std::string                   m_kEmpty;
};

// =============================================================================
//  Estate (MiniHouse) extras: dummy attach points, durability tier table,
//  furniture object effects, object-animation rows.
// =============================================================================
class EstateExtraTables {
public:
    static EstateExtraTables& Get();
    void Bind();
    struct DummyRow      { uint32 uiHouseID; uint32 uiSlotID; std::string kBoneName; };
    struct EndureRow     { uint32 uiTier; uint32 uiMaxEndure; uint32 uiHourlyDecay; };
    struct FurnEffRow    { uint32 uiFurnID; uint32 uiAbStateID; uint32 uiRange; };
    struct ObjAniRow     { uint32 uiFurnID; uint32 uiAnimSlot; std::string kAniName; uint32 uiLoopMs; };
    void              DummiesFor (uint32 uiHouseID, std::vector<const DummyRow*>& rOut) const;
    const EndureRow*  FindEndure (uint32 uiTier)         const;
    const FurnEffRow* FindFurnEff(uint32 uiFurnID)       const;
    const ObjAniRow*  FindObjAni (uint32 uiFurnID, uint32 uiAnimSlot) const;
private:
    std::vector<DummyRow>   m_kDummy;
    std::map<uint32, std::vector<size_t> >  m_kDummyByHouse;
    std::vector<EndureRow>  m_kEndure;  std::map<uint32,size_t> m_kEndureByTier;
    std::vector<FurnEffRow> m_kFurnEff; std::map<uint32,size_t> m_kFurnEffById;
    std::vector<ObjAniRow>  m_kObjAni;  std::map<uint64,size_t> m_kObjAniByKey;
};

// =============================================================================
//  Field level-condition gate + Friend-point reward + Set-item effect.
// =============================================================================
class FieldLvConditionTable {
public:
    static FieldLvConditionTable& Get();
    void Bind();
    bool IsAllowed(const std::string& rMapName, uint32 uiCharLevel) const;
private:
    struct Row { uint32 uiMin; uint32 uiMax; };
    std::map<std::string, Row> m_kRow;
};
class FriendPointRewardTable {
public:
    static FriendPointRewardTable& Get();
    void Bind();
    struct Row { uint32 uiThreshold; std::string kRewardItem; uint32 uiQty; };
    const Row* Find(uint32 uiThreshold) const;
    size_t     Count() const { return m_kRows.size(); }
private:
    std::vector<Row> m_kRows; std::map<uint32, size_t> m_kByThreshold;
};
class SetItemEffectTable {
public:
    static SetItemEffectTable& Get();
    void Bind();
    struct Row { uint32 uiEffectID; std::string kEffectName; int32 iValue; };
    const Row* Find(uint32 uiEffectID) const;
private:
    std::vector<Row> m_kRows; std::map<uint32, size_t> m_kById;
};

// =============================================================================
//  Group AbState (party/raid abnormal state propagation).
// =============================================================================
class GroupAbStateTable {
public:
    static GroupAbStateTable& Get();
    void Bind();
    struct Row { uint32 uiSourceAbState; uint32 uiPropagatedAbState; uint32 uiRange; };
    const Row* Find(uint32 uiSourceAbState) const;
private:
    std::vector<Row> m_kRows; std::map<uint32, size_t> m_kBySrc;
};

// =============================================================================
//  Guild Battle (GB) -- in-game casino mini-games hosted in the Guild
//  Battle event hall: dice, slot machine, NPCs, ban window, exchange
//  cap, tax, reward dispenser, join eligibility, event broadcast codes.
// =============================================================================
class GBTables {
public:
    static GBTables& Get();
    void Bind();

    bool IsBanned(uint32 uiHourOfWeek)   const;
    int32 ExchangeMaxCoin(uint32 uiTier) const;
    int32 HouseRent(uint32 uiTier)       const;
    int32 TaxRate(uint32 uiTier)         const;     // x100 (e.g. 250 = 2.50%)
    bool  CanJoin(uint32 uiCharLv)       const;

    struct DiceGame { uint32 uiID; std::string kName; uint32 uiMinBet; uint32 uiMaxBet; };
    const DiceGame* FindDice(uint32 uiID) const;
    int32 DiceRate(uint32 uiGameID, uint32 uiOutcome) const;
    int32 DiceDividend(uint32 uiGameID, uint32 uiTier) const;

    struct SMRow { uint32 uiID; uint32 uiBetCoin; uint32 uiCardRate; uint32 uiJPRate; uint32 uiLine; };
    const SMRow* FindSM(uint32 uiID) const;
    int32        SMAll(uint32 uiID)       const;
    int32        SMCenter(uint32 uiID)    const;
    int32        SMGroup(uint32 uiID)     const;
    int32        SMNPC  (uint32 uiID)     const;
    int32        EventCode(const std::string& rEvent) const;

    int32        Reward(uint32 uiHandle, std::string& rItemOut, uint32& uiQtyOut) const;
private:
    std::map<uint32, bool>          m_kBan;          // hourOfWeek -> banned
    std::map<uint32, int32>         m_kExchange;
    std::map<uint32, int32>         m_kHouse;
    std::map<uint32, int32>         m_kTax;
    std::map<uint32, bool>          m_kJoin;         // CharLv -> ok

    std::vector<DiceGame>           m_kDiceGame;     std::map<uint32,size_t> m_kDiceGameById;
    std::map<uint64, uint32>        m_kDiceRate;
    std::map<uint64, uint32>        m_kDiceDiv;

    std::vector<SMRow>              m_kSM;           std::map<uint32,size_t> m_kSMById;
    std::map<uint32, int32>         m_kSMAll;
    std::map<uint32, int32>         m_kSMCenter;
    std::map<uint32, int32>         m_kSMGroup;
    std::map<uint32, int32>         m_kSMNPC;

    std::map<std::string, int32>    m_kEvent;
    struct RewardRow { std::string kItem; uint32 uiQty; };
    std::map<uint32, RewardRow>     m_kReward;
};

// =============================================================================
//  Guild Tournament Item (GTI) drop subjects + win-score by participation.
// =============================================================================
class GTITables {
public:
    static GTITables& Get();
    void Bind();
    struct BreedRow   { uint32 uiID; std::string kSubjectInx; uint32 uiTier; };
    const BreedRow* FindBreed(uint32 uiID)       const;
    int32           GetRate (uint32 uiID)        const;
    int32           GetRateGap(uint32 uiID, int32 iGap) const;
    int32           ServerCap(uint32 uiID)       const;
    int32           WinScore(uint32 uiPlace)     const;     // GTWinScore.shn
private:
    std::vector<BreedRow>             m_kBreed;  std::map<uint32,size_t> m_kBreedById;
    std::map<uint32, int32>           m_kRate;
    std::map<uint64, int32>           m_kRateGap;
    std::map<uint32, int32>           m_kServer;
    std::map<uint32, int32>           m_kWinScore;
};

// =============================================================================
//  Guild Academy extras (level up + rank).
// =============================================================================
class GuildAcademyExtraTables {
public:
    static GuildAcademyExtraTables& Get();
    void Bind();
    struct LevelUpRow { uint32 uiLevel; uint32 uiExpRequired; uint32 uiBuffID; };
    struct RankRow    { uint32 uiRank;  std::string kInxName; uint32 uiThreshold; };
    struct GradeScoreRow { uint32 uiGrade; uint32 uiScore; };
    struct LevelScoreRow { uint32 uiLevel; uint32 uiScore; };
    const LevelUpRow*    FindLevel(uint32 uiLevel) const;
    const RankRow*       FindRank (uint32 uiRank)  const;
    int32                GradeScore(uint32 uiGrade) const;
    int32                LevelScore(uint32 uiLevel) const;
private:
    std::vector<LevelUpRow> m_kLvl;   std::map<uint32,size_t> m_kLvlById;
    std::vector<RankRow>    m_kRank;  std::map<uint32,size_t> m_kRankById;
    std::map<uint32, int32> m_kGradeScore;
    std::map<uint32, int32> m_kLevelScore;
};

// =============================================================================
//  Guild Tournament extras (LvGap / MasterBuff / Occupy / Require / Score / Skill).
// =============================================================================
class GuildTournamentExtraTables {
public:
    static GuildTournamentExtraTables& Get();
    void Bind();
    int32 LvGapMul(int32 iDiff)                 const;     // x100
    int32 MasterBuffID(uint32 uiTier)           const;
    int32 OccupyPoints(uint32 uiHoldSec)        const;
    bool  Require(uint32 uiGTNo, std::string& rOutNote) const;
    int32 ScoreFor(uint32 uiPlace)              const;
    int32 SkillIDFor(uint32 uiTier, uint32 uiSlot) const;
private:
    std::map<int32, int32>          m_kLvGap;
    std::map<uint32, int32>         m_kMasterBuff;
    std::map<uint32, int32>         m_kOccupy;
    std::map<uint32, std::string>   m_kRequire;
    std::map<uint32, int32>         m_kScore;
    std::map<uint64, int32>         m_kSkill;       // (tier<<32)|slot
};

// =============================================================================
//  Item extras: dismantle ladder, drop-log filter, inventory deletion
//  rules, NPC merchant info, item-mix recipes, item-money quirks,
//  options matrix (in addition to World/ItemOptions.txt the SHN form
//  is keyed by EquipType), package contents, server equip-type matrix,
//  cash shop product table, sort hint, use-effect mapping, job equip.
// =============================================================================
class ItemExtraTables {
public:
    static ItemExtraTables& Get();
    void Bind();

    struct DismantleRow { uint32 uiItemID; std::string kRewardItem; uint32 uiMin; uint32 uiMax; };
    struct DropLogRow   { uint32 uiItemID; uint32 uiAnnounceLevel; };
    struct InvenDelRow  { uint32 uiItemID; uint32 uiAfterDays;     };
    struct MerchantRow  { std::string kNpcInx; uint32 uiSlot; std::string kItemInx; uint32 uiPrice; };
    struct MixRow       { uint32 uiID; std::string kIn1; std::string kIn2; std::string kIn3;
                          std::string kOut; uint32 uiRate; uint32 uiCost; };
    struct MoneyRow     { uint32 uiItemID; int64 iValue; };
    struct OptionsRow   { uint32 uiEquipType; uint32 uiSlot; uint32 uiOptionID; uint32 uiPercent; };
    struct PackageRow   { uint32 uiPackageID; std::string kItem; uint32 uiQty; uint32 uiUpgrade; };
    struct ServerEquipRow { uint32 uiEquipType; uint32 uiSlot; uint32 uiAllowedClass; };
    struct ShopRow      { uint32 uiID; std::string kItemInx; uint32 uiPriceVis; uint32 uiPriceMicro; };
    struct SortRow      { uint32 uiItemID; uint32 uiCategory; uint32 uiSubCategory; };
    struct UseEffectRow { uint32 uiItemID; uint32 uiAbStateID; uint32 uiKeepTimeMs; };
    struct JobEquipRow  { uint32 uiJobID; uint32 uiEquipType; uint32 uiAllowed; };

    const DismantleRow*   FindDismantle(uint32 uiItemID) const;
    int32                 DropAnnounce (uint32 uiItemID) const;
    int32                 InvenDelDays (uint32 uiItemID) const;
    void                  MerchantList(const std::string& rNpc,
                                       std::vector<const MerchantRow*>& rOut) const;
    const MixRow*         FindMix(uint32 uiID) const;
    int64                 MoneyValueOf(uint32 uiItemID) const;
    void                  OptionsFor(uint32 uiEquipType, uint32 uiSlot,
                                     std::vector<const OptionsRow*>& rOut) const;
    void                  PackageContents(uint32 uiPackageID,
                                          std::vector<const PackageRow*>& rOut) const;
    bool                  CanEquip(uint32 uiEquipType, uint32 uiSlot, uint32 uiClassID) const;
    const ShopRow*        FindShop(uint32 uiID) const;
    const SortRow*        FindSort(uint32 uiItemID) const;
    const UseEffectRow*   FindUseEffect(uint32 uiItemID) const;
    bool                  JobCanEquip(uint32 uiJobID, uint32 uiEquipType) const;
private:
    std::vector<DismantleRow>   m_kDis;     std::map<uint32,size_t> m_kDisById;
    std::map<uint32, int32>     m_kDropLog;
    std::map<uint32, int32>     m_kInvenDel;
    std::vector<MerchantRow>    m_kMerch;
    std::map<std::string, std::vector<size_t> > m_kMerchByNpc;
    std::vector<MixRow>         m_kMix;     std::map<uint32,size_t> m_kMixById;
    std::map<uint32, int64>     m_kMoney;
    std::vector<OptionsRow>     m_kOpt;     std::map<uint64, std::vector<size_t> > m_kOptByKey;
    std::vector<PackageRow>     m_kPkg;     std::map<uint32, std::vector<size_t> > m_kPkgById;
    std::map<uint64, uint32>    m_kSrvEq;   // (eqType<<32)|slot -> classMask
    std::vector<ShopRow>        m_kShop;    std::map<uint32,size_t> m_kShopById;
    std::vector<SortRow>        m_kSort;    std::map<uint32,size_t> m_kSortById;
    std::vector<UseEffectRow>   m_kUse;     std::map<uint32,size_t> m_kUseById;
    std::map<uint64, bool>      m_kJob;     // (jobID<<32)|eqType -> allowed
};

// =============================================================================
//  Kingdom Quest vote / item / team / vote majority.
// =============================================================================
class KQExtraTables {
public:
    static KQExtraTables& Get();
    void Bind();
    bool IsVote(uint32 uiKQID)              const;
    int32 VoteMajorityRate(uint32 uiKQID)   const;     // 0..100
    int32 TeamSize(uint32 uiKQID)           const;
    void  Items(uint32 uiKQID, std::vector<std::string>& rOut) const;
private:
    std::map<uint32, bool>                       m_kVote;
    std::map<uint32, uint32>                     m_kRate;
    std::map<uint32, uint32>                     m_kTeam;
    std::map<uint32, std::vector<std::string> >  m_kItems;
};

// =============================================================================
//  Lucky-Coin reward (LCGroupRate / LCReward).
// =============================================================================
class LuckyCoinTables {
public:
    static LuckyCoinTables& Get();
    void Bind();
    int32 GroupRate(uint32 uiGroupID, uint32 uiTier)   const;
    bool  Roll(uint32 uiGroupID, uint32 uiTier,
               std::string& rOutItem, uint32& uiQtyOut) const;
private:
    std::map<uint64, uint32> m_kRate;        // (group<<32)|tier
    struct RewardRow { std::string kItem; uint32 uiQty; };
    std::map<uint64, RewardRow> m_kReward;
};

// =============================================================================
//  Map extras: per-zone buff, link points, way points.
// =============================================================================
class MapExtraTables {
public:
    static MapExtraTables& Get();
    void Bind();
    struct BuffRow      { uint32 uiMapID; uint32 uiAbStateID; uint32 uiCondition; };
    struct LinkPointRow { uint32 uiMapID; uint32 uiLinkID; int32 iX,iY,iZ;
                          uint32 uiToMapID; int32 iToX,iToY,iToZ; };
    struct WayPointRow  { uint32 uiMapID; uint32 uiWayID; int32 iX,iY,iZ; };
    const BuffRow*      FindBuff (uint32 uiMapID)                 const;
    const LinkPointRow* FindLink (uint32 uiMapID, uint32 uiLinkID)const;
    void                WaysFor  (uint32 uiMapID,
                                  std::vector<const WayPointRow*>& rOut) const;
private:
    std::vector<BuffRow>      m_kBuff; std::map<uint32,size_t> m_kBuffById;
    std::vector<LinkPointRow> m_kLink; std::map<uint64,size_t> m_kLinkByKey;
    std::vector<WayPointRow>  m_kWay;
    std::map<uint32, std::vector<size_t> > m_kWayByMap;
};

// =============================================================================
//  MarketSearchInfo (auction & trade-board search keywords).
// =============================================================================
class MarketSearchInfoTable {
public:
    static MarketSearchInfoTable& Get();
    void Bind();
    int32 KeywordIdOf(const std::string& rKeyword) const;
private:
    std::map<std::string, uint32> m_kRow;
};

// =============================================================================
//  Minimon (battle-pet companion) info + auto-use consumable mapping.
// =============================================================================
class MinimonTables {
public:
    static MinimonTables& Get();
    void Bind();
    struct InfoRow { uint32 uiID; std::string kInxName; uint32 uiHP; uint32 uiAtk;
                     uint32 uiDef; uint32 uiCDms; uint32 uiSpawnSec; };
    struct AutoRow { uint32 uiMinimonID; std::string kItemInx; uint32 uiThresholdPct; };
    const InfoRow* Find(uint32 uiID)              const;
    const AutoRow* AutoFor(uint32 uiMinimonID)    const;
private:
    std::vector<InfoRow> m_kInfo; std::map<uint32,size_t> m_kInfoById;
    std::vector<AutoRow> m_kAuto; std::map<uint32,size_t> m_kAutoById;
};

// =============================================================================
//  Mob extras: ability-state drop bias, condition server, kill-able
//  rules, kill-announce messages, kill log filter, no-fade-in flag,
//  regen animation map.
// =============================================================================
class MobExtraTables {
public:
    static MobExtraTables& Get();
    void Bind();
    struct DropSetRow   { uint32 uiAbState; uint32 uiPercent; };
    struct CondRow      { std::string kMobInx; uint32 uiCondID; std::string kArg; };
    struct KillAbleRow  { std::string kMobInx; uint32 uiMinLv; uint32 uiMaxLv; };
    struct AnnounceRow  { std::string kMobInx; uint32 uiColor; std::string kMessage; };
    int32              DropPctForAbState(uint32 uiAbState) const;
    void               CondsFor(const std::string& rMob,
                                std::vector<const CondRow*>& rOut) const;
    const KillAbleRow* FindKillAble(const std::string& rMob) const;
    const AnnounceRow* FindAnnounce(const std::string& rMob) const;
    bool               IsLogged(const std::string& rMob)    const;
    bool               NoFadeIn(const std::string& rMob)    const;
    int32              RegenAniIDFor(const std::string& rMob) const;
private:
    std::map<uint32, uint32>             m_kDropSet;
    std::vector<CondRow>                 m_kCond;
    std::map<std::string, std::vector<size_t> > m_kCondByMob;
    std::vector<KillAbleRow>             m_kKill;
    std::map<std::string, size_t>        m_kKillByMob;
    std::vector<AnnounceRow>             m_kAnn;
    std::map<std::string, size_t>        m_kAnnByMob;
    std::map<std::string, bool>          m_kLog;
    std::map<std::string, bool>          m_kFade;
    std::map<std::string, uint32>        m_kRegenAni;
};

// =============================================================================
//  Mover (mount) extras: HG (mount evolution rank), upgrade effects,
//  per-mover usable skill slots, rare-mover rate gates.
// =============================================================================
class MoverExtraTables {
public:
    static MoverExtraTables& Get();
    void Bind();
    struct HGRow         { uint32 uiID; std::string kInxName; uint32 uiGrade; };
    struct UpEffRow      { uint32 uiID; uint32 uiEffectID;  int32 iValue;     };
    struct UseSkillRow   { uint32 uiMoverID; uint32 uiSlot; std::string kSkillInx; };
    struct RareEachRow   { uint32 uiMoverID; uint32 uiRate; };
    struct RareSubRow    { uint32 uiMoverID; uint32 uiSubID; uint32 uiRate; };
    const HGRow*       FindHG    (uint32 uiID)         const;
    const UpEffRow*    FindUpEff (uint32 uiID)         const;
    void               SkillsFor (uint32 uiMoverID,
                                  std::vector<const UseSkillRow*>& rOut) const;
    int32              RareRate  (uint32 uiMoverID)    const;
    int32              SubRate   (uint32 uiMoverID, uint32 uiSubID) const;
private:
    std::vector<HGRow>      m_kHG;     std::map<uint32,size_t> m_kHGById;
    std::vector<UpEffRow>   m_kUp;     std::map<uint32,size_t> m_kUpById;
    std::vector<UseSkillRow>m_kSkill;
    std::map<uint32, std::vector<size_t> > m_kSkillByMover;
    std::map<uint32, uint32>m_kEach;
    std::map<uint64, uint32>m_kSub;       // (mover<<32)|sub
};

// =============================================================================
//  Misc one-off singletons.
// =============================================================================
class MsgWorldManagerTable {
public:
    static MsgWorldManagerTable& Get();
    void Bind();
    const std::string& Get(uint32 uiID) const;
private:
    std::map<uint32, std::string> m_kRow;
    std::string m_kEmpty;
};
class MultiHitTypeTable {
public:
    static MultiHitTypeTable& Get();
    void Bind();
    struct Row { uint32 uiTypeID; uint32 uiTickMs; uint32 uiCount; uint32 uiAttenuationPct; };
    const Row* Find(uint32 uiTypeID) const;
private:
    std::vector<Row> m_kRows; std::map<uint32, size_t> m_kById;
};
class MysteryVaultTable {
public:
    static MysteryVaultTable& Get();
    void Bind();
    struct Row { uint32 uiVaultID; std::string kRewardItem; uint32 uiQty; uint32 uiPercent; };
    void RowsFor(uint32 uiVaultID, std::vector<const Row*>& rOut) const;
    bool Roll   (uint32 uiVaultID, std::string& rOutItem, uint32& uiQtyOut) const;
private:
    std::vector<Row> m_kRows;
    std::map<uint32, std::vector<size_t> > m_kByVault;
};
class NpcScheduleTable {
public:
    static NpcScheduleTable& Get();
    void Bind();
    struct Row { std::string kNpcInx; uint32 uiStartHour; uint32 uiEndHour; uint32 uiDayMask; };
    bool IsActive(const std::string& rNpcInx, uint32 uiHourOfWeek) const;
private:
    std::vector<Row> m_kRows;
    std::map<std::string, std::vector<size_t> > m_kByNpc;
};

// =============================================================================
//  PassiveSkill set abstate + QuestData (additional quest header fields).
// =============================================================================
class PSkillSetAbstateTable {
public:
    static PSkillSetAbstateTable& Get();
    void Bind();
    int32 AbStateFor(uint32 uiPSkillID) const;
private:
    std::map<uint32, uint32> m_kRow;
};
class QuestDataTable {
public:
    static QuestDataTable& Get();
    void Bind();
    // Extra quest-side state not in QuestSpecies/QuestDialog/QuestScript.
    struct Row { uint32 uiID; std::string kInxName; uint32 uiMinLv; uint32 uiMaxLv;
                 uint32 uiTimeLimitSec; uint32 uiMaxRepeat; };
    const Row* Find(uint32 uiID) const;
    size_t Count() const { return m_kRows.size(); }
private:
    std::vector<Row> m_kRows; std::map<uint32, size_t> m_kById;
};

// =============================================================================
//  CharacterTitleStateServer / TownPortal + AttendReward / AttendSchedule.
// =============================================================================
class AttendanceTables {
public:
    static AttendanceTables& Get();
    void Bind();
    struct ScheduleRow { uint32 uiDay; std::string kRewardItem; uint32 uiQty; };
    const ScheduleRow* OnDay(uint32 uiDayIdx) const;
    int32              RewardItemQty(const std::string& rItemInx) const;
    size_t             ScheduleSize() const { return m_kSched.size(); }
private:
    std::vector<ScheduleRow> m_kSched; std::map<uint32, size_t> m_kSchedByDay;
    std::map<std::string, uint32> m_kRewardItemQty;
};

// =============================================================================
//  NPC dialog UI data: bridges the click-on-NPC server flow into the
//  client-rendered dialog box.
//   * `NpcDialogData.shn` -- per-DialogID rows holding the spoken-text key,
//     the list of available reply / button keys, and any conditional
//     branching logic. The server forwards a DialogID to the client which
//     then renders the box from this table.
//   * `NPCViewInfo.shn`   -- per-ViewInfoID rows holding the actual button
//     label string id, icon id, tooltip id, and the action tag the client
//     must echo back when the player presses it.
// Both files live in the client-side `ressystem/` drop and are loaded into
// the universal `ShnRegistry` at boot. The schema is column-name-driven so
// we tolerate drop-to-drop reorder.
// =============================================================================
class NPCDialogTables {
public:
    static NPCDialogTables& Get();
    void Bind();
    struct DialogRow {
        uint32      uiDialogID;
        std::string kTextKey;        // "Dialog" -- localisation key
        std::string kButtonKeys;     // CSV of NPCViewInfo IDs to render
        uint32      uiNextDialogID;  // 0 if leaf
    };
    struct ViewRow {
        uint32      uiViewInfoID;
        std::string kLabelKey;       // "Label" -- localisation key
        uint32      uiIconID;
        std::string kActionTag;      // echoed back when pressed
        std::string kArg0;
        std::string kArg1;
    };
    const DialogRow* FindDialog (uint32 uiDialogID)   const;
    const ViewRow*   FindView   (uint32 uiViewInfoID) const;
    void  ButtonsFor(uint32 uiDialogID, std::vector<const ViewRow*>& rOut) const;
    size_t DialogCount() const { return m_kDialog.size(); }
    size_t ViewCount()   const { return m_kView.size();   }
private:
    std::vector<DialogRow>  m_kDialog;
    std::map<uint32, size_t> m_kDialogById;
    std::vector<ViewRow>    m_kView;
    std::map<uint32, size_t> m_kViewById;
};

// =============================================================================
//  One-call binder for everything in this file.
// =============================================================================
void BindAllExtendedTables();

} // namespace fiesta
#endif
