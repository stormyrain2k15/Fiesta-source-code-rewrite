// Server/Zone/Tables/ItemTables.cpp
// FEATURE: world-creation -- ItemInfo / ItemInfoServer / ItemUpgrade /
// ItemAction binders. Four SHNs share one .cpp because they form one
// item lookup surface (one Get(), four Find* methods, cross-file
// dependencies on the same in-memory vectors). Splitting them across
// four .cpps would force the singleton into a header which is exactly
// the static-init-order trap we're avoiding.
//
// If a future patch adds a NEW item-domain SHN (e.g. ItemAuction or
// ItemMarket), it MUST land in its own .cpp under /Tables/ -- this
// file stops at the four-row contract baked in at NA2016.
#include "BindMacros.h"
#include "../GroupTables.h"

namespace shine {

ItemTables& ItemTables::Get() { static ItemTables s; return s; }

void ItemTables::Bind() {
    {
        // FEATURE: world-creation -- column read: ID, InxName, Name,
        // Type, Class, MaxLot, Equip, ItemAuctionSubGroup,
        // ItemGradeType, TwoHand, AtkSpeed, DemandLv, Grade, MinWC,
        // MaxWC, AC, MinMA, MaxMA, MR, WCRate, MARate, ACRate, MRRate,
        // CriRate, CriMinWc, CriMaxWc, CriMinMa, CriMaxMa, CrlTB,
        // MaxHP, MaxSP, MaxAP, Fig..Ass (25 class flags), BuyPrice,
        // SellPrice, BuyFame, BuyGToken, BuyGBCoin, WeaponType,
        // ArmorType, UpLimit, UpSucRatio, UpLuckRatio, UpResource,
        // BasicUpInx, AddUpInx, TH, TB, ShieldAC, HitRatePlus,
        // EvaRatePlus, MACriPlus, CriDamPlus, MagCriDamPlus,
        // PutOnBelonged, Belonged, NoDrop, NoSell, NoStorage, NoTrade,
        // NoDelete, TitleName, ItemUseSkill, SetItemIndex, ItemFunc,
        // AutoMon
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
        // FEATURE: world-creation -- column read: ID, InxName,
        // MarketIndex, Rou, Eld, Urg, All, Fer, Kas, Sad, Kor, Ver,
        // Mys, DropGroupA, DropGroupB, RandomOptionDropGroup, Vanish,
        // looting, DropRateKilledByMob, DropRateKilledByPlayer,
        // ISET_Index, KQItem, PK_KQ_USE, KQ_Item_Drop, PreventAttack
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
        // FEATURE: world-creation -- column read: ID, Upgrade, SucRate,
        // FailGrade, Resource, Qty
        BIND_BEGIN(t, "ItemUpgrade")
        m_kUpgrade.reserve(t->Rows().size());
        ITER_ROWS(t) {
            LegacyItemUpgradeRow rec;
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
        // FEATURE: world-creation -- column read: ID, Action, Condition,
        // Effect, CoolTime
        BIND_BEGIN(t, "ItemAction")
        m_kActions.reserve(t->Rows().size());
        ITER_ROWS(t) {
            LegacyItemActionRow rec;
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
const LegacyItemUpgradeRow* ItemTables::FindUpgrade(uint32 uiID) const {
    std::map<uint32, size_t>::const_iterator it = m_kUpgradeById.find(uiID);
    return (it == m_kUpgradeById.end()) ? NULL : &m_kUpgrade[it->second];
}
const LegacyItemActionRow* ItemTables::FindAction(uint32 uiID) const {
    std::map<uint32, size_t>::const_iterator it = m_kActionsById.find(uiID);
    return (it == m_kActionsById.end()) ? NULL : &m_kActions[it->second];
}

} // namespace shine
