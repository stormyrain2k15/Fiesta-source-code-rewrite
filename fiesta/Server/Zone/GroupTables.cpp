// Server/Zone/GroupTables.cpp
//
// Each Bind() walks the corresponding ShnFile's rows once and builds a
// typed vector + index map. Column reads are by NAME (`ShnGetU32(t, i,
// "ColName")`) so a future drop that renames or reorders columns lights up
// the missing-column path (zero-fill / empty-string) instead of silently
// reading the wrong field. The trade-off vs index reads is one O(N) name
// scan per accessor binding -- negligible at boot.
#include "GroupTables.h"
#include "../Shared/ShineLogSystem.h"

namespace fiesta {

#define BIND_BEGIN(VAR, NAME) \
    const ShnFile* VAR = ShnRegistry::Get().GetTable(NAME); \
    if (!VAR) { SHINELOG_WARN("%s.shn missing -- group accessor disabled", NAME); return; }

#define ITER_ROWS(VAR) for (size_t _r = 0; _r < (VAR)->Rows().size(); ++_r)

// =============================================================================
//  ItemTables
// =============================================================================
ItemTables& ItemTables::Get() { static ItemTables s; return s; }

void ItemTables::Bind() {
    {
        BIND_BEGIN(t, "ItemInfo")
        m_kItems.reserve(t->Rows().size());
        ITER_ROWS(t) {
            ItemInfoRow rec;
            rec.uiID            = ShnGetU32(*t, _r, "ID");
            rec.kInxName        = ShnGetStr(*t, _r, "InxName");
            rec.kName           = ShnGetStr(*t, _r, "Name");
            rec.uiType          = ShnGetU32(*t, _r, "Type");
            rec.uiClass         = ShnGetU32(*t, _r, "Class");
            rec.uiMaxLot        = ShnGetU32(*t, _r, "MaxLot");
            rec.uiEquip         = ShnGetU32(*t, _r, "Equip");
            rec.uiItemAuctionSubGroup = ShnGetU32(*t, _r, "ItemAuctionSubGroup");
            rec.uiItemGradeType = ShnGetU32(*t, _r, "ItemGradeType");
            rec.uiTwoHand       = ShnGetU32(*t, _r, "TwoHand");
            rec.uiAtkSpeed      = ShnGetU32(*t, _r, "AtkSpeed");
            rec.uiDemandLv      = ShnGetU32(*t, _r, "DemandLv");
            rec.uiGrade         = ShnGetU32(*t, _r, "Grade");
            rec.uiMinWC         = ShnGetU32(*t, _r, "MinWC");
            rec.uiMaxWC         = ShnGetU32(*t, _r, "MaxWC");
            rec.uiAC            = ShnGetU32(*t, _r, "AC");
            rec.uiMinMA         = ShnGetU32(*t, _r, "MinMA");
            rec.uiMaxMA         = ShnGetU32(*t, _r, "MaxMA");
            rec.uiMR            = ShnGetU32(*t, _r, "MR");
            rec.uiWCRate        = ShnGetU32(*t, _r, "WCRate");
            rec.uiMARate        = ShnGetU32(*t, _r, "MARate");
            rec.uiACRate        = ShnGetU32(*t, _r, "ACRate");
            rec.uiMRRate        = ShnGetU32(*t, _r, "MRRate");
            rec.uiCriRate       = ShnGetU32(*t, _r, "CriRate");
            rec.uiCriMinWc      = ShnGetU32(*t, _r, "CriMinWc");
            rec.uiCriMaxWc      = ShnGetU32(*t, _r, "CriMaxWc");
            rec.uiCriMinMa      = ShnGetU32(*t, _r, "CriMinMa");
            rec.uiCriMaxMa      = ShnGetU32(*t, _r, "CriMaxMa");
            rec.uiCrlTB         = ShnGetU32(*t, _r, "CrlTB");
            rec.uiMaxHP         = ShnGetU32(*t, _r, "MaxHP");
            rec.uiMaxSP         = ShnGetU32(*t, _r, "MaxSP");
            rec.uiMaxAP         = ShnGetU32(*t, _r, "MaxAP");
            // Pack the per-class permission flags (Fig..Ass) into a 32-bit mask.
            // Order mirrors Schemas.h::ItemInfoRow column ordinals.
            rec.uiClassMask = 0;
            static const char* kClassCols[25] = {
                "Fig","Cfig","War","Gla","Kni","Cle","Hcle","Pal","Hol","Gua",
                "Arc","Harc","Sco","Sha","Ran","Mag","Wmag","Enc","Warl","Wiz",
                "Jok","Chs","Cru","Cls","Ass"
            };
            for (uint32 cc = 0; cc < 25; ++cc) {
                if (ShnGetU32(*t, _r, kClassCols[cc])) rec.uiClassMask |= (1u << cc);
            }
            rec.uiBuyPrice      = ShnGetU32(*t, _r, "BuyPrice");
            rec.uiSellPrice     = ShnGetU32(*t, _r, "SellPrice");
            rec.uiBuyFame       = ShnGetU32(*t, _r, "BuyFame");
            rec.uiBuyGToken     = ShnGetU32(*t, _r, "BuyGToken");
            rec.uiBuyGBCoin     = ShnGetU32(*t, _r, "BuyGBCoin");
            rec.uiWeaponType    = ShnGetU32(*t, _r, "WeaponType");
            rec.uiArmorType     = ShnGetU32(*t, _r, "ArmorType");
            rec.uiUpLimit       = ShnGetU32(*t, _r, "UpLimit");
            rec.uiUpSucRatio    = ShnGetU32(*t, _r, "UpSucRatio");
            rec.uiUpLuckRatio   = ShnGetU32(*t, _r, "UpLuckRatio");
            rec.uiUpResource    = ShnGetU32(*t, _r, "UpResource");
            rec.uiBasicUpInx    = ShnGetU32(*t, _r, "BasicUpInx");
            rec.uiAddUpInx      = ShnGetU32(*t, _r, "AddUpInx");
            rec.uiTH            = ShnGetU32(*t, _r, "TH");
            rec.uiTB            = ShnGetU32(*t, _r, "TB");
            rec.uiShieldAC      = ShnGetU32(*t, _r, "ShieldAC");
            rec.uiHitRatePlus   = ShnGetU32(*t, _r, "HitRatePlus");
            rec.uiEvaRatePlus   = ShnGetU32(*t, _r, "EvaRatePlus");
            rec.uiMACriPlus     = ShnGetU32(*t, _r, "MACriPlus");
            rec.uiCriDamPlus    = ShnGetU32(*t, _r, "CriDamPlus");
            rec.uiMagCriDamPlus = ShnGetU32(*t, _r, "MagCriDamPlus");
            rec.bPutOnBelonged  = (uint8)ShnGetU32(*t, _r, "PutOnBelonged");
            rec.bBelonged       = (uint8)ShnGetU32(*t, _r, "Belonged");
            rec.bNoDrop         = (uint8)ShnGetU32(*t, _r, "NoDrop");
            rec.bNoSell         = (uint8)ShnGetU32(*t, _r, "NoSell");
            rec.bNoStorage      = (uint8)ShnGetU32(*t, _r, "NoStorage");
            rec.bNoTrade        = (uint8)ShnGetU32(*t, _r, "NoTrade");
            rec.bNoDelete       = (uint8)ShnGetU32(*t, _r, "NoDelete");
            rec.kTitleName      = ShnGetStr(*t, _r, "TitleName");
            rec.kItemUseSkill   = ShnGetStr(*t, _r, "ItemUseSkill");
            rec.kSetItemIndex   = ShnGetStr(*t, _r, "SetItemIndex");
            rec.uiItemFunc      = ShnGetU32(*t, _r, "ItemFunc");
            rec.bAutoMon        = (uint8)ShnGetU32(*t, _r, "AutoMon");
            m_kItemById[rec.uiID]      = m_kItems.size();
            m_kItemByInx[rec.kInxName] = m_kItems.size();
            m_kItems.push_back(rec);
        }
        SHINELOG_INFO("ItemInfo.shn: %u rows (91-col)", (uint32)m_kItems.size());
    }
    {
        BIND_BEGIN(t, "ItemInfoServer")
        m_kServer.reserve(t->Rows().size());
        ITER_ROWS(t) {
            ItemInfoServerRow rec;
            rec.uiID         = ShnGetU32(*t, _r, "ID");
            rec.kInxName     = ShnGetStr(*t, _r, "InxName");
            rec.kMarketIndex = ShnGetStr(*t, _r, "MarketIndex");
            rec.uiRou = ShnGetU32(*t, _r, "Rou");
            rec.uiEld = ShnGetU32(*t, _r, "Eld");
            rec.uiUrg = ShnGetU32(*t, _r, "Urg");
            rec.uiAll = ShnGetU32(*t, _r, "All");
            rec.uiFer = ShnGetU32(*t, _r, "Fer");
            rec.uiKas = ShnGetU32(*t, _r, "Kas");
            rec.uiSad = ShnGetU32(*t, _r, "Sad");
            rec.uiKor = ShnGetU32(*t, _r, "Kor");
            rec.uiVer = ShnGetU32(*t, _r, "Ver");
            rec.uiMys = ShnGetU32(*t, _r, "Mys");
            rec.kDropGroupA            = ShnGetStr(*t, _r, "DropGroupA");
            rec.kDropGroupB            = ShnGetStr(*t, _r, "DropGroupB");
            rec.kRandomOptionDropGroup = ShnGetStr(*t, _r, "RandomOptionDropGroup");
            rec.uiVanishSecs           = ShnGetU32(*t, _r, "Vanish");
            rec.uiLootingMode          = ShnGetU32(*t, _r, "looting");
            rec.uiDropRateKilledByMob    = (uint16)ShnGetU32(*t, _r, "DropRateKilledByMob");
            rec.uiDropRateKilledByPlayer = (uint16)ShnGetU32(*t, _r, "DropRateKilledByPlayer");
            rec.uiISETIndex            = ShnGetU32(*t, _r, "ISET_Index");
            rec.bKQItem                = (uint8)ShnGetU32(*t, _r, "KQItem");
            rec.bPK_KQ_USE             = (uint8)ShnGetU32(*t, _r, "PK_KQ_USE");
            rec.bKQ_Item_Drop          = (uint8)ShnGetU32(*t, _r, "KQ_Item_Drop");
            rec.bPreventAttack         = (uint8)ShnGetU32(*t, _r, "PreventAttack");
            m_kServerById[rec.uiID] = m_kServer.size();
            if (!rec.kInxName.empty()) m_kServerByInx[rec.kInxName] = m_kServer.size();
            m_kServer.push_back(rec);
        }
        SHINELOG_INFO("ItemInfoServer.shn: %u rows (25-col)", (uint32)m_kServer.size());
    }
    {
        BIND_BEGIN(t, "ItemUpgrade")
        m_kUpgrade.reserve(t->Rows().size());
        ITER_ROWS(t) {
            ItemUpgradeRow rec;
            rec.uiID        = ShnGetU32(*t, _r, "ID");
            rec.kUpgrade    = ShnGetStr(*t, _r, "Upgrade");
            rec.uiSucRate   = ShnGetU32(*t, _r, "SucRate");
            rec.uiFailGrade = ShnGetU32(*t, _r, "FailGrade");
            rec.uiResource  = ShnGetU32(*t, _r, "Resource");
            rec.uiQty       = ShnGetU32(*t, _r, "Qty");
            m_kUpgradeById[rec.uiID] = m_kUpgrade.size();
            m_kUpgrade.push_back(rec);
        }
    }
    {
        BIND_BEGIN(t, "ItemAction")
        m_kActions.reserve(t->Rows().size());
        ITER_ROWS(t) {
            ItemActionRow rec;
            rec.uiID        = ShnGetU32(*t, _r, "ID");
            rec.kAction     = ShnGetStr(*t, _r, "Action");
            rec.uiCondition = ShnGetU32(*t, _r, "Condition");
            rec.uiEffect    = ShnGetU32(*t, _r, "Effect");
            rec.uiCoolTime  = ShnGetU32(*t, _r, "CoolTime");
            m_kActionsById[rec.uiID] = m_kActions.size();
            m_kActions.push_back(rec);
        }
    }
}
const ItemInfoRow* ItemTables::FindItem(uint32 uiID) const {
    std::map<uint32, size_t>::const_iterator it = m_kItemById.find(uiID);
    return (it == m_kItemById.end()) ? NULL : &m_kItems[it->second];
}
const ItemInfoRow* ItemTables::FindByInx(const std::string& rInx) const {
    std::map<std::string, size_t>::const_iterator it = m_kItemByInx.find(rInx);
    return (it == m_kItemByInx.end()) ? NULL : &m_kItems[it->second];
}
const ItemInfoServerRow* ItemTables::FindServer(uint32 uiID) const {
    std::map<uint32, size_t>::const_iterator it = m_kServerById.find(uiID);
    return (it == m_kServerById.end()) ? NULL : &m_kServer[it->second];
}
const ItemInfoServerRow* ItemTables::FindServerByInx(const std::string& rInx) const {
    std::map<std::string, size_t>::const_iterator it = m_kServerByInx.find(rInx);
    return (it == m_kServerByInx.end()) ? NULL : &m_kServer[it->second];
}
uint32 ItemTables::KingdomWeight(uint32 uiID, const std::string& rTag) const {
    const ItemInfoServerRow* p = FindServer(uiID);
    if (!p) return 0;
    if (rTag == "Rou") return p->uiRou;
    if (rTag == "Eld") return p->uiEld;
    if (rTag == "Urg") return p->uiUrg;
    if (rTag == "Fer") return p->uiFer;
    if (rTag == "Kas") return p->uiKas;
    if (rTag == "Sad") return p->uiSad;
    if (rTag == "Kor") return p->uiKor;
    if (rTag == "Ver") return p->uiVer;
    if (rTag == "Mys") return p->uiMys;
    return p->uiAll;
}
uint32 ItemTables::VanishSecs(uint32 uiID) const {
    const ItemInfoServerRow* p = FindServer(uiID);
    return p ? p->uiVanishSecs : 0;
}
uint32 ItemTables::LootingMode(uint32 uiID) const {
    const ItemInfoServerRow* p = FindServer(uiID);
    return p ? p->uiLootingMode : 0;
}
const ItemUpgradeRow* ItemTables::FindUpgrade(uint32 uiID) const {
    std::map<uint32, size_t>::const_iterator it = m_kUpgradeById.find(uiID);
    return (it == m_kUpgradeById.end()) ? NULL : &m_kUpgrade[it->second];
}
const ItemActionRow* ItemTables::FindAction(uint32 uiID) const {
    std::map<uint32, size_t>::const_iterator it = m_kActionsById.find(uiID);
    return (it == m_kActionsById.end()) ? NULL : &m_kActions[it->second];
}

// =============================================================================
//  MobTables
// =============================================================================
MobTables& MobTables::Get() { static MobTables s; return s; }

void MobTables::Bind() {
    {
        BIND_BEGIN(t, "MobInfo")
        m_kMobs.reserve(t->Rows().size());
        ITER_ROWS(t) {
            MobInfoRow rec;
            rec.uiID            = ShnGetU32(*t, _r, "ID");
            rec.kInxName        = ShnGetStr(*t, _r, "InxName");
            rec.kName           = ShnGetStr(*t, _r, "Name");
            rec.uiLevel         = ShnGetU32(*t, _r, "Level");
            rec.uiMaxHP         = ShnGetU32(*t, _r, "MaxHP");
            rec.uiWalkSpeed     = ShnGetU32(*t, _r, "WalkSpeed");
            rec.uiRunSpeed      = ShnGetU32(*t, _r, "RunSpeed");
            rec.uiIsNPC         = ShnGetU32(*t, _r, "IsNPC");
            rec.uiSize          = ShnGetU32(*t, _r, "Size");
            rec.uiWeaponType    = ShnGetU32(*t, _r, "WeaponType");
            rec.uiArmorType     = ShnGetU32(*t, _r, "ArmorType");
            rec.uiGradeType     = ShnGetU32(*t, _r, "GradeType");
            rec.uiType          = ShnGetU32(*t, _r, "Type");
            rec.uiIsPlayerSide  = ShnGetU32(*t, _r, "IsPlayerSide");
            rec.uiAbsoluteSize  = ShnGetU32(*t, _r, "AbsoluteSize");
            rec.uiExp           = ShnGetU32(*t, _r, "Exp");
            m_kMobById[rec.uiID]      = m_kMobs.size();
            m_kMobByInx[rec.kInxName] = m_kMobs.size();
            m_kMobs.push_back(rec);
        }
        SHINELOG_INFO("MobInfo.shn: %u rows", (uint32)m_kMobs.size());
    }
    {
        BIND_BEGIN(t, "MobSpecies")
        m_kSpec.reserve(t->Rows().size());
        ITER_ROWS(t) {
            MobSpeciesRow rec;
            rec.uiID     = ShnGetU32(*t, _r, "ID");
            rec.kMobName = ShnGetStr(*t, _r, "MobName");
            m_kSpecById[rec.uiID] = m_kSpec.size();
            m_kSpec.push_back(rec);
        }
    }
    {
        BIND_BEGIN(t, "MobLifeTime")
        m_kLife.reserve(t->Rows().size());
        ITER_ROWS(t) {
            MobLifeTimeRow rec;
            rec.uiID         = ShnGetU32(*t, _r, "ID");
            rec.uiLifeTimeMs = ShnGetU32(*t, _r, "LifeTime");
            m_kLifeById[rec.uiID] = m_kLife.size();
            m_kLife.push_back(rec);
        }
    }
}
const MobInfoRow* MobTables::FindMob(uint32 uiID) const {
    std::map<uint32, size_t>::const_iterator it = m_kMobById.find(uiID);
    return (it == m_kMobById.end()) ? NULL : &m_kMobs[it->second];
}
const MobInfoRow* MobTables::FindByInx(const std::string& rInx) const {
    std::map<std::string, size_t>::const_iterator it = m_kMobByInx.find(rInx);
    return (it == m_kMobByInx.end()) ? NULL : &m_kMobs[it->second];
}
const MobSpeciesRow* MobTables::FindSpec(uint32 uiID) const {
    std::map<uint32, size_t>::const_iterator it = m_kSpecById.find(uiID);
    return (it == m_kSpecById.end()) ? NULL : &m_kSpec[it->second];
}
const MobLifeTimeRow* MobTables::FindLife(uint32 uiID) const {
    std::map<uint32, size_t>::const_iterator it = m_kLifeById.find(uiID);
    return (it == m_kLifeById.end()) ? NULL : &m_kLife[it->second];
}

// =============================================================================
//  SkillTables
// =============================================================================
SkillTables& SkillTables::Get() { static SkillTables s; return s; }

void SkillTables::Bind() {
    {
        BIND_BEGIN(t, "ActiveSkill")
        m_kActive.reserve(t->Rows().size());
        ITER_ROWS(t) {
            ActiveSkillRow rec;
            rec.uiID       = ShnGetU32(*t, _r, "ID");
            rec.kInxName   = ShnGetStr(*t, _r, "InxName");
            rec.uiClass    = ShnGetU32(*t, _r, "Class");
            rec.uiMaxLevel = ShnGetU32(*t, _r, "MaxLevel");
            rec.uiSP       = ShnGetU32(*t, _r, "SP");
            rec.uiCoolMs   = ShnGetU32(*t, _r, "CoolTime");
            rec.uiCastMs   = ShnGetU32(*t, _r, "CastingTime");
            m_kActiveById[rec.uiID] = m_kActive.size();
            m_kActive.push_back(rec);
        }
        SHINELOG_INFO("ActiveSkill.shn: %u rows", (uint32)m_kActive.size());
    }
    {
        BIND_BEGIN(t, "ActiveSkillInfoServer")
        m_kActiveS.reserve(t->Rows().size());
        ITER_ROWS(t) {
            ActiveSkillInfoServerRow rec;
            rec.uiID         = ShnGetU32(*t, _r, "ID");
            rec.uiBaseDamage = ShnGetU32(*t, _r, "BaseDamage");
            rec.uiTargetType = ShnGetU32(*t, _r, "TargetType");
            rec.uiRange      = ShnGetU32(*t, _r, "Range");
            rec.uiAggroBonus = ShnGetU32(*t, _r, "AggroBonus");
            m_kActiveSById[rec.uiID] = m_kActiveS.size();
            m_kActiveS.push_back(rec);
        }
    }
    {
        BIND_BEGIN(t, "PassiveSkill")
        m_kPassive.reserve(t->Rows().size());
        ITER_ROWS(t) {
            PassiveSkillRow rec;
            rec.uiID       = ShnGetU32(*t, _r, "ID");
            rec.kInxName   = ShnGetStr(*t, _r, "InxName");
            rec.uiClass    = ShnGetU32(*t, _r, "Class");
            rec.uiMaxLevel = ShnGetU32(*t, _r, "MaxLevel");
            m_kPassiveById[rec.uiID] = m_kPassive.size();
            m_kPassive.push_back(rec);
        }
    }
    {
        BIND_BEGIN(t, "AreaSkill")
        m_kArea.reserve(t->Rows().size());
        ITER_ROWS(t) {
            AreaSkillRow rec;
            rec.uiID       = ShnGetU32(*t, _r, "ID");
            rec.uiRange    = ShnGetU32(*t, _r, "Range");
            rec.uiDuration = ShnGetU32(*t, _r, "Duration");
            rec.uiTickMs   = ShnGetU32(*t, _r, "TickMs");
            m_kAreaById[rec.uiID] = m_kArea.size();
            m_kArea.push_back(rec);
        }
    }
}
const ActiveSkillRow* SkillTables::FindActive(uint32 uiID) const {
    std::map<uint32, size_t>::const_iterator it = m_kActiveById.find(uiID);
    return (it == m_kActiveById.end()) ? NULL : &m_kActive[it->second];
}
const ActiveSkillInfoServerRow* SkillTables::FindActiveS(uint32 uiID) const {
    std::map<uint32, size_t>::const_iterator it = m_kActiveSById.find(uiID);
    return (it == m_kActiveSById.end()) ? NULL : &m_kActiveS[it->second];
}
const PassiveSkillRow* SkillTables::FindPassive(uint32 uiID) const {
    std::map<uint32, size_t>::const_iterator it = m_kPassiveById.find(uiID);
    return (it == m_kPassiveById.end()) ? NULL : &m_kPassive[it->second];
}
const AreaSkillRow* SkillTables::FindArea(uint32 uiID) const {
    std::map<uint32, size_t>::const_iterator it = m_kAreaById.find(uiID);
    return (it == m_kAreaById.end()) ? NULL : &m_kArea[it->second];
}

// =============================================================================
//  MapTables / AbStateTables / Presentation / Pup / Mount / MiniHouse / Guild2
//  / Collect / GradeRandom / KQ -- compact bind+find pairs.
// =============================================================================
MapTables& MapTables::Get() { static MapTables s; return s; }
void MapTables::Bind() {
    BIND_BEGIN(t, "MapInfo")
    m_kMaps.reserve(t->Rows().size());
    ITER_ROWS(t) {
        MapInfoRow rec;
        rec.uiID         = ShnGetU32(*t, _r, "ID");
        rec.kMapName     = ShnGetStr(*t, _r, "MapName");
        rec.kName        = ShnGetStr(*t, _r, "Name");
        rec.uiIsWMLink   = ShnGetU32(*t, _r, "IsWMLink");
        rec.iRegenX      = ShnGetI32(*t, _r, "RegenX");
        rec.iRegenY      = ShnGetI32(*t, _r, "RegenY");
        rec.uiKingdomMap = ShnGetU32(*t, _r, "KingdomMap");
        rec.kFolder      = ShnGetStr(*t, _r, "MapFolderName");
        rec.uiInSide     = ShnGetU32(*t, _r, "InSide");
        rec.uiSight      = ShnGetU32(*t, _r, "Sight");
        m_kById[rec.uiID]       = m_kMaps.size();
        m_kByName[rec.kMapName] = m_kMaps.size();
        m_kMaps.push_back(rec);
    }
    SHINELOG_INFO("MapInfo.shn: %u rows", (uint32)m_kMaps.size());
}
const MapInfoRow* MapTables::Find(uint32 uiID) const {
    std::map<uint32, size_t>::const_iterator it = m_kById.find(uiID);
    return (it == m_kById.end()) ? NULL : &m_kMaps[it->second];
}
const MapInfoRow* MapTables::FindByName(const std::string& rN) const {
    std::map<std::string, size_t>::const_iterator it = m_kByName.find(rN);
    return (it == m_kByName.end()) ? NULL : &m_kMaps[it->second];
}

AbStateTables& AbStateTables::Get() { static AbStateTables s; return s; }
void AbStateTables::Bind() {
    BIND_BEGIN(t, "AbState")
    m_kRows.reserve(t->Rows().size());
    ITER_ROWS(t) {
        AbStateRow rec;
        rec.uiID              = ShnGetU32(*t, _r, "ID");
        rec.kInxName          = ShnGetStr(*t, _r, "InxName");
        rec.uiAbStataIndex    = ShnGetU32(*t, _r, "AbStataIndex");
        rec.uiKeepTimeRatio   = ShnGetU32(*t, _r, "KeepTimeRatio");
        rec.uiKeepTimePower   = ShnGetU32(*t, _r, "KeepTimePower");
        rec.uiStateGrade      = ShnGetU32(*t, _r, "StateGrade");
        rec.uiAbStateSaveType = ShnGetU32(*t, _r, "AbStateSaveType");
        m_kById[rec.uiID]       = m_kRows.size();
        m_kByInx[rec.kInxName]  = m_kRows.size();
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("AbState.shn: %u rows", (uint32)m_kRows.size());
}
const AbStateRow* AbStateTables::Find(uint32 uiID) const {
    std::map<uint32, size_t>::const_iterator it = m_kById.find(uiID);
    return (it == m_kById.end()) ? NULL : &m_kRows[it->second];
}
const AbStateRow* AbStateTables::FindByInx(const std::string& rN) const {
    std::map<std::string, size_t>::const_iterator it = m_kByInx.find(rN);
    return (it == m_kByInx.end()) ? NULL : &m_kRows[it->second];
}

PresentationTables& PresentationTables::Get() { static PresentationTables s; return s; }
void PresentationTables::Bind() {
    if (const ShnFile* t = ShnRegistry::Get().GetTable("HairInfo")) {
        ITER_ROWS(t) {
            HairInfoRow rec;
            rec.uiID       = ShnGetU32(*t, _r, "ID");
            rec.kIndexName = ShnGetStr(*t, _r, "IndexName");
            rec.kName      = ShnGetStr(*t, _r, "HairName");
            rec.uiGrade    = ShnGetU32(*t, _r, "Grade");
            rec.uiFighter  = ShnGetU32(*t, _r, "fighter");
            rec.uiArcher   = ShnGetU32(*t, _r, "archer");
            rec.uiCleric   = ShnGetU32(*t, _r, "cleric");
            rec.uiMage     = ShnGetU32(*t, _r, "mage");
            m_kHairById[rec.uiID] = m_kHair.size();
            m_kHair.push_back(rec);
        }
    }
    if (const ShnFile* t = ShnRegistry::Get().GetTable("FaceInfo")) {
        ITER_ROWS(t) {
            FaceInfoRow rec;
            rec.uiID      = ShnGetU32(*t, _r, "ID");
            rec.kFaceName = ShnGetStr(*t, _r, "FaceName");
            rec.uiGrade   = ShnGetU32(*t, _r, "Grade");
            m_kFaceById[rec.uiID] = m_kFace.size();
            m_kFace.push_back(rec);
        }
    }
    if (const ShnFile* t = ShnRegistry::Get().GetTable("ClassName")) {
        ITER_ROWS(t) {
            ClassNameRow rec;
            rec.uiClassID   = ShnGetU32(*t, _r, "ClassID");
            rec.kPrefix     = ShnGetStr(*t, _r, "acPrefix");
            rec.kEngName    = ShnGetStr(*t, _r, "acEngName");
            rec.kLocalName  = ShnGetStr(*t, _r, "acLocalName");
            m_kClassById[rec.uiClassID] = m_kClass.size();
            m_kClass.push_back(rec);
        }
    }
    if (const ShnFile* t = ShnRegistry::Get().GetTable("CharacterTitleData")) {
        ITER_ROWS(t) {
            CharacterTitleRow rec;
            rec.uiID     = ShnGetU32(*t, _r, "ID");
            rec.kInxName = ShnGetStr(*t, _r, "InxName");
            rec.uiGrade  = ShnGetU32(*t, _r, "Grade");
            m_kCharTitleById[rec.uiID] = m_kCharTitle.size();
            m_kCharTitle.push_back(rec);
        }
    }
    if (const ShnFile* t = ShnRegistry::Get().GetTable("WeaponTitleData")) {
        ITER_ROWS(t) {
            WeaponTitleRow rec;
            rec.uiID     = ShnGetU32(*t, _r, "ID");
            rec.kInxName = ShnGetStr(*t, _r, "InxName");
            m_kWeapTitleById[rec.uiID] = m_kWeapTitle.size();
            m_kWeapTitle.push_back(rec);
        }
    }
}
const HairInfoRow*       PresentationTables::FindHair (uint32 uiID) const {
    std::map<uint32, size_t>::const_iterator it = m_kHairById.find(uiID);
    return (it == m_kHairById.end()) ? NULL : &m_kHair[it->second]; }
const FaceInfoRow*       PresentationTables::FindFace (uint32 uiID) const {
    std::map<uint32, size_t>::const_iterator it = m_kFaceById.find(uiID);
    return (it == m_kFaceById.end()) ? NULL : &m_kFace[it->second]; }
const ClassNameRow*      PresentationTables::FindClass(uint32 uiID) const {
    std::map<uint32, size_t>::const_iterator it = m_kClassById.find(uiID);
    return (it == m_kClassById.end()) ? NULL : &m_kClass[it->second]; }
const CharacterTitleRow* PresentationTables::FindCharTitle(uint32 uiID) const {
    std::map<uint32, size_t>::const_iterator it = m_kCharTitleById.find(uiID);
    return (it == m_kCharTitleById.end()) ? NULL : &m_kCharTitle[it->second]; }
const WeaponTitleRow*    PresentationTables::FindWeapTitle(uint32 uiID) const {
    std::map<uint32, size_t>::const_iterator it = m_kWeapTitleById.find(uiID);
    return (it == m_kWeapTitleById.end()) ? NULL : &m_kWeapTitle[it->second]; }

PupTables& PupTables::Get() { static PupTables s; return s; }
void PupTables::Bind() {
    if (const ShnFile* t = ShnRegistry::Get().GetTable("PupMain")) {
        ITER_ROWS(t) {
            PupMainRow rec;
            rec.uiPupID    = ShnGetU32(*t, _r, "PupID");
            rec.kPupIDX    = ShnGetStr(*t, _r, "PupIDX");
            rec.kItemIDX   = ShnGetStr(*t, _r, "ItemIDX");
            rec.uiPupSpeed = ShnGetU32(*t, _r, "PupSpeed");
            m_kMainById[rec.uiPupID] = m_kMain.size();
            m_kMain.push_back(rec);
        }
    }
    if (const ShnFile* t = ShnRegistry::Get().GetTable("PupServer")) {
        ITER_ROWS(t) {
            PupServerRow rec;
            rec.uiPupID    = ShnGetU32(*t, _r, "PupID");
            rec.uiSpawnHP  = ShnGetU32(*t, _r, "SpawnHP");
            rec.uiSpawnSP  = ShnGetU32(*t, _r, "SpawnSP");
            rec.uiCooldown = ShnGetU32(*t, _r, "Cooldown");
            rec.uiAggro    = ShnGetU32(*t, _r, "Aggro");
            m_kServerById[rec.uiPupID] = m_kServer.size();
            m_kServer.push_back(rec);
        }
    }
    if (const ShnFile* t = ShnRegistry::Get().GetTable("PupCase")) {
        ITER_ROWS(t) {
            PupCaseRow rec;
            rec.uiCaseID   = ShnGetU32(*t, _r, "CaseID");
            rec.uiPriority = ShnGetU32(*t, _r, "Priority");
            rec.uiAction   = ShnGetU32(*t, _r, "Action");
            m_kCaseById[rec.uiCaseID] = m_kCase.size();
            m_kCase.push_back(rec);
        }
    }
}
const PupMainRow*   PupTables::FindMain  (uint32 uiID) const {
    std::map<uint32, size_t>::const_iterator it = m_kMainById.find(uiID);
    return (it == m_kMainById.end()) ? NULL : &m_kMain[it->second]; }
const PupServerRow* PupTables::FindServer(uint32 uiID) const {
    std::map<uint32, size_t>::const_iterator it = m_kServerById.find(uiID);
    return (it == m_kServerById.end()) ? NULL : &m_kServer[it->second]; }
const PupCaseRow*   PupTables::FindCase  (uint32 uiID) const {
    std::map<uint32, size_t>::const_iterator it = m_kCaseById.find(uiID);
    return (it == m_kCaseById.end()) ? NULL : &m_kCase[it->second]; }

MountTables& MountTables::Get() { static MountTables s; return s; }
void MountTables::Bind() {
    if (const ShnFile* t = ShnRegistry::Get().GetTable("MoverMain")) {
        ITER_ROWS(t) {
            MoverMainRow rec;
            rec.uiMoverID = ShnGetU32(*t, _r, "MoverID");
            rec.kMoverIDX = ShnGetStr(*t, _r, "MoverIDX");
            rec.uiCastMs  = ShnGetU32(*t, _r, "CastingTime");
            rec.uiCoolMs  = ShnGetU32(*t, _r, "CoolTime");
            rec.uiRun     = ShnGetU32(*t, _r, "RunSpeed");
            rec.uiWalk    = ShnGetU32(*t, _r, "WalkSpeed");
            rec.uiHours   = ShnGetU32(*t, _r, "DurationHour");
            rec.uiMaxSlot = ShnGetU32(*t, _r, "MaxCharSlot");
            m_kMainById[rec.uiMoverID] = m_kMain.size();
            m_kMain.push_back(rec);
        }
    }
    if (const ShnFile* t = ShnRegistry::Get().GetTable("MoverItem")) {
        ITER_ROWS(t) {
            MoverItemRow rec;
            rec.uiID     = ShnGetU32(*t, _r, "ID");
            rec.kItemIDX = ShnGetStr(*t, _r, "ItemIDX");
            m_kItemById[rec.uiID] = m_kItem.size();
            m_kItem.push_back(rec);
        }
    }
}
const MoverMainRow* MountTables::FindMain(uint32 uiID) const {
    std::map<uint32, size_t>::const_iterator it = m_kMainById.find(uiID);
    return (it == m_kMainById.end()) ? NULL : &m_kMain[it->second]; }
const MoverItemRow* MountTables::FindItem(uint32 uiID) const {
    std::map<uint32, size_t>::const_iterator it = m_kItemById.find(uiID);
    return (it == m_kItemById.end()) ? NULL : &m_kItem[it->second]; }

MiniHouseTables& MiniHouseTables::Get() { static MiniHouseTables s; return s; }
void MiniHouseTables::Bind() {
    if (const ShnFile* t = ShnRegistry::Get().GetTable("MiniHouse")) {
        ITER_ROWS(t) {
            MiniHouseRow rec;
            rec.uiID            = ShnGetU32(*t, _r, "ID");
            rec.kInxName        = ShnGetStr(*t, _r, "InxName");
            rec.kName           = ShnGetStr(*t, _r, "Name");
            rec.uiGrade         = ShnGetU32(*t, _r, "Grade");
            rec.uiDurationHour  = ShnGetU32(*t, _r, "DurationHour");
            m_kHouseById[rec.uiID] = m_kHouse.size();
            m_kHouse.push_back(rec);
        }
    }
    if (const ShnFile* t = ShnRegistry::Get().GetTable("MiniHouseFurniture")) {
        ITER_ROWS(t) {
            MiniHouseFurnRow rec;
            rec.uiID       = ShnGetU32(*t, _r, "ID");
            rec.kInxName   = ShnGetStr(*t, _r, "InxName");
            rec.uiCategory = ShnGetU32(*t, _r, "Category");
            rec.uiHP       = ShnGetU32(*t, _r, "HP");
            m_kFurnById[rec.uiID] = m_kFurn.size();
            m_kFurn.push_back(rec);
        }
    }
}
const MiniHouseRow*     MiniHouseTables::Find    (uint32 uiID) const {
    std::map<uint32, size_t>::const_iterator it = m_kHouseById.find(uiID);
    return (it == m_kHouseById.end()) ? NULL : &m_kHouse[it->second]; }
const MiniHouseFurnRow* MiniHouseTables::FindFurn(uint32 uiID) const {
    std::map<uint32, size_t>::const_iterator it = m_kFurnById.find(uiID);
    return (it == m_kFurnById.end()) ? NULL : &m_kFurn[it->second]; }

GuildTables2& GuildTables2::Get() { static GuildTables2 s; return s; }
void GuildTables2::Bind() {
    if (const ShnFile* t = ShnRegistry::Get().GetTable("GuildAcademy")) {
        ITER_ROWS(t) {
            GuildAcademyRow rec;
            rec.kBuffName        = ShnGetStr(*t, _r, "BuffName");
            rec.uiLeastJoinTime  = ShnGetU32(*t, _r, "LeastJoinTime");
            rec.uiRankAggregation= ShnGetU32(*t, _r, "RankAggregationTime");
            m_kAcademy.push_back(rec);
        }
    }
    if (const ShnFile* t = ShnRegistry::Get().GetTable("GuildGradeData")) {
        ITER_ROWS(t) {
            GuildGradeDataRow rec;
            rec.uiGrade       = ShnGetU32(*t, _r, "Grade");
            rec.uiMaxMember   = ShnGetU32(*t, _r, "MaxMember");
            rec.uiStorageSlot = ShnGetU32(*t, _r, "StorageSlot");
            m_kGradeById[rec.uiGrade] = m_kGrade.size();
            m_kGrade.push_back(rec);
        }
    }
    if (const ShnFile* t = ShnRegistry::Get().GetTable("GuildTournament")) {
        ITER_ROWS(t) {
            GuildTournamentRow rec;
            rec.uiGTNo        = ShnGetU32(*t, _r, "GTNo");
            rec.uiMatchNumber = ShnGetU32(*t, _r, "MatchNumber");
            rec.uiPrizeCoin   = ShnGetU32(*t, _r, "PrizeCoin");
            m_kGTById[rec.uiGTNo] = m_kGT.size();
            m_kGT.push_back(rec);
        }
    }
    if (const ShnFile* t = ShnRegistry::Get().GetTable("GuildTournamentReward")) {
        ITER_ROWS(t) {
            GuildTournamentRewardRow rec;
            rec.uiRank = ShnGetU32(*t, _r, "Rank");
            rec.uiCoin = ShnGetU32(*t, _r, "Coin");
            rec.uiFame = ShnGetU32(*t, _r, "Fame");
            m_kRewardByRank[rec.uiRank] = m_kReward.size();
            m_kReward.push_back(rec);
        }
    }
}
const GuildGradeDataRow*        GuildTables2::FindGrade (uint32 uiG) const {
    std::map<uint32, size_t>::const_iterator it = m_kGradeById.find(uiG);
    return (it == m_kGradeById.end()) ? NULL : &m_kGrade[it->second]; }
const GuildTournamentRow*       GuildTables2::FindGT    (uint32 uiN) const {
    std::map<uint32, size_t>::const_iterator it = m_kGTById.find(uiN);
    return (it == m_kGTById.end()) ? NULL : &m_kGT[it->second]; }
const GuildTournamentRewardRow* GuildTables2::FindReward(uint32 uiR) const {
    std::map<uint32, size_t>::const_iterator it = m_kRewardByRank.find(uiR);
    return (it == m_kRewardByRank.end()) ? NULL : &m_kReward[it->second]; }

CollectTables& CollectTables::Get() { static CollectTables s; return s; }
void CollectTables::Bind() {
    if (const ShnFile* t = ShnRegistry::Get().GetTable("CollectCard")) {
        ITER_ROWS(t) {
            CollectCardRow rec;
            rec.uiCardID        = ShnGetU32(*t, _r, "CC_CardID");
            rec.kItemInx        = ShnGetStr(*t, _r, "CC_ItemInx");
            rec.uiCardGradeType = ShnGetU32(*t, _r, "CC_CardGradeType");
            rec.uiMobGroup      = ShnGetU32(*t, _r, "CC_CardMobGroup");
            m_kCardById[rec.uiCardID] = m_kCards.size();
            m_kCards.push_back(rec);
        }
    }
    if (const ShnFile* t = ShnRegistry::Get().GetTable("CollectCardReward")) {
        ITER_ROWS(t) {
            CollectCardRewardRow rec;
            rec.uiID      = ShnGetU32(*t, _r, "ID");
            rec.uiPercent = ShnGetU32(*t, _r, "Percent");
            rec.kReward   = ShnGetStr(*t, _r, "Reward");
            rec.uiQty     = ShnGetU32(*t, _r, "Qty");
            rec.uiBonus   = ShnGetU32(*t, _r, "Bonus");
            m_kRewardById[rec.uiID] = m_kReward.size();
            m_kReward.push_back(rec);
        }
    }
}
const CollectCardRow*       CollectTables::FindCard  (uint32 uiID) const {
    std::map<uint32, size_t>::const_iterator it = m_kCardById.find(uiID);
    return (it == m_kCardById.end()) ? NULL : &m_kCards[it->second]; }
const CollectCardRewardRow* CollectTables::FindReward(uint32 uiID) const {
    std::map<uint32, size_t>::const_iterator it = m_kRewardById.find(uiID);
    return (it == m_kRewardById.end()) ? NULL : &m_kReward[it->second]; }

GradeRandomTables& GradeRandomTables::Get() { static GradeRandomTables s; return s; }
void GradeRandomTables::Bind() {
    if (const ShnFile* t = ShnRegistry::Get().GetTable("GradeItemOption")) {
        ITER_ROWS(t) {
            GradeItemOptionRow rec;
            rec.uiItemIndex         = ShnGetU32(*t, _r, "ItemIndex");
            rec.uiSTR               = (uint16)ShnGetU32(*t, _r, "STR");
            rec.uiCON               = (uint16)ShnGetU32(*t, _r, "CON");
            rec.uiDEX               = (uint16)ShnGetU32(*t, _r, "DEX");
            rec.uiINT               = (uint16)ShnGetU32(*t, _r, "INT");
            rec.uiMEN               = (uint16)ShnGetU32(*t, _r, "MEN");
            rec.uiResistPoison      = (uint16)ShnGetU32(*t, _r, "ResistPoison");
            rec.uiResistDeaseas     = (uint16)ShnGetU32(*t, _r, "ResistDeaseas");
            rec.uiResistCurse       = (uint16)ShnGetU32(*t, _r, "ResistCurse");
            rec.uiResistMoveSpdDown = (uint16)ShnGetU32(*t, _r, "ResistMoveSpdDown");
            rec.uiCritical          = (uint16)ShnGetU32(*t, _r, "Critical");
            rec.uiToHitRate         = (uint16)ShnGetU32(*t, _r, "ToHitRate");
            rec.uiToHitPlus         = (uint16)ShnGetU32(*t, _r, "ToHitPlus");
            rec.uiToBlockRate       = (uint16)ShnGetU32(*t, _r, "ToBlockRate");
            rec.uiToBlockPlus       = (uint16)ShnGetU32(*t, _r, "ToBlockPlus");
            rec.uiMaxHP             = (uint16)ShnGetU32(*t, _r, "MaxHP");
            rec.uiMaxSP             = (uint16)ShnGetU32(*t, _r, "MaxSP");
            rec.uiMoveSpdRate       = (uint16)ShnGetU32(*t, _r, "MoveSpdRate");
            rec.uiAbsoluteAttack    = (uint16)ShnGetU32(*t, _r, "AbsoluteAttack");
            rec.uiPickupLimit       = (uint16)ShnGetU32(*t, _r, "PickupLimit");
            m_kGradeById[rec.uiItemIndex] = m_kGrade.size();
            m_kGrade.push_back(rec);
        }
    }
    if (const ShnFile* t = ShnRegistry::Get().GetTable("SetItem")) {
        ITER_ROWS(t) {
            SetItemRow rec;
            rec.uiIndex  = ShnGetU32(*t, _r, "Index");
            rec.uiPiece  = ShnGetU32(*t, _r, "Piece");
            rec.uiEffect = ShnGetU32(*t, _r, "Effect");
            m_kSetById[rec.uiIndex] = m_kSet.size();
            m_kSet.push_back(rec);
        }
    }
    if (const ShnFile* t = ShnRegistry::Get().GetTable("RandomOption")) {
        ITER_ROWS(t) {
            RandomOptionRow2 rec;
            rec.kDropItemIndex    = ShnGetStr(*t, _r, "DropItemIndex");
            rec.uiRandomOptionType= ShnGetU32(*t, _r, "RandomOptionType");
            rec.iMin              = ShnGetI32(*t, _r, "Min");
            rec.iMax              = ShnGetI32(*t, _r, "Max");
            rec.uiTypeDropRate    = ShnGetU32(*t, _r, "TypeDropRate");
            m_kRandom.push_back(rec);
        }
    }
}
const GradeItemOptionRow* GradeRandomTables::FindGrade(uint32 uiID) const {
    std::map<uint32, size_t>::const_iterator it = m_kGradeById.find(uiID);
    return (it == m_kGradeById.end()) ? NULL : &m_kGrade[it->second]; }
const SetItemRow*         GradeRandomTables::FindSet  (uint32 uiI) const {
    std::map<uint32, size_t>::const_iterator it = m_kSetById.find(uiI);
    return (it == m_kSetById.end()) ? NULL : &m_kSet[it->second]; }

KQTables& KQTables::Get() { static KQTables s; return s; }
void KQTables::Bind() {
    if (const ShnFile* t = ShnRegistry::Get().GetTable("KingdomQuest")) {
        ITER_ROWS(t) {
            KingdomQuestRow rec;
            rec.uiID       = ShnGetU32(*t, _r, "ID");
            rec.kName      = ShnGetStr(*t, _r, "Name");
            rec.uiMinLevel = ShnGetU32(*t, _r, "MinLevel");
            rec.uiMaxLevel = ShnGetU32(*t, _r, "MaxLevel");
            rec.uiTeamSize = ShnGetU32(*t, _r, "TeamSize");
            m_kKQById[rec.uiID] = m_kKQ.size();
            m_kKQ.push_back(rec);
        }
    }
    if (const ShnFile* t = ShnRegistry::Get().GetTable("KingdomQuestRew")) {
        ITER_ROWS(t) {
            KingdomQuestRewRow rec;
            rec.uiID    = ShnGetU32(*t, _r, "ID");
            rec.uiRank  = ShnGetU32(*t, _r, "Rank");
            rec.kReward = ShnGetStr(*t, _r, "Reward");
            rec.uiQty   = ShnGetU32(*t, _r, "Qty");
            m_kRew.push_back(rec);
        }
    }
    if (const ShnFile* t = ShnRegistry::Get().GetTable("KingdomQuestMap")) {
        ITER_ROWS(t) {
            KingdomQuestMapRow rec;
            rec.uiID     = ShnGetU32(*t, _r, "ID");
            rec.kMapName = ShnGetStr(*t, _r, "MapName");
            rec.uiX      = ShnGetU32(*t, _r, "X");
            rec.uiY      = ShnGetU32(*t, _r, "Y");
            m_kMapById[rec.uiID] = m_kMap.size();
            m_kMap.push_back(rec);
        }
    }
}
const KingdomQuestRow*    KQTables::FindKQ (uint32 uiID) const {
    std::map<uint32, size_t>::const_iterator it = m_kKQById.find(uiID);
    return (it == m_kKQById.end()) ? NULL : &m_kKQ[it->second]; }
const KingdomQuestRewRow* KQTables::FindRew(uint32 uiID, uint32 uiRank) const {
    for (size_t i = 0; i < m_kRew.size(); ++i)
        if (m_kRew[i].uiID == uiID && m_kRew[i].uiRank == uiRank)
            return &m_kRew[i];
    return NULL;
}
const KingdomQuestMapRow* KQTables::FindMap(uint32 uiID) const {
    std::map<uint32, size_t>::const_iterator it = m_kMapById.find(uiID);
    return (it == m_kMapById.end()) ? NULL : &m_kMap[it->second]; }

// =============================================================================
//  One-call binder
// =============================================================================
void BindAllGroupTables() {
    ItemTables::Get()        .Bind();
    MobTables::Get()         .Bind();
    SkillTables::Get()       .Bind();
    MapTables::Get()         .Bind();
    AbStateTables::Get()     .Bind();
    PresentationTables::Get().Bind();
    PupTables::Get()         .Bind();
    MountTables::Get()       .Bind();
    MiniHouseTables::Get()   .Bind();
    GuildTables2::Get()      .Bind();
    CollectTables::Get()     .Bind();
    GradeRandomTables::Get() .Bind();
    KQTables::Get()          .Bind();
    SHINELOG_INFO("BindAllGroupTables: done; ShnRegistry has %u tables",
                  (uint32)ShnRegistry::Get().size());
}

#undef BIND_BEGIN
#undef ITER_ROWS

} // namespace fiesta
