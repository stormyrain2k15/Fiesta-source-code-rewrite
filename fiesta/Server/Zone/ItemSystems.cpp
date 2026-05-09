// Server/Zone/ItemSystems.cpp
#include "ItemSystems.h"
#include "../Shared/well512.h"
#include "../Shared/ShineLogSystem.h"
#include "../DataReader/Schemas.h"
#include "../DataReader/Tables.h"  // back-compat aliases

namespace fiesta {

static well512 s_kRng;

// 16
bool ShineItemUpgrade::TryUpgrade(ShineItem& r) {
    int32 pct = AccUpGradeDataBox::SuccessRatePct(r.uiEnchant);
    if ((int32)(s_kRng.NextRange(100)) < pct) { r.uiEnchant += 1; return true; }
    if ((int32)(s_kRng.NextRange(100)) < BRAccUpgradeDataBox::BreakRatePct(r.uiEnchant)) {
        r.uiEnchant = 0; r.uiEndure = 0;
    }
    return false;
}
int32 AccUpGradeDataBox  ::SuccessRatePct(uint16 lvl) { static const int32 t[16] = {100,100,90,80,70,60,50,40,30,25,20,15,10,8,6,4}; return lvl < 16 ? t[lvl] : 2; }
int32 BRAccUpgradeDataBox::BreakRatePct  (uint16 lvl) { static const int32 t[16] = {  0,  0, 0, 0, 5,10,15,20,25,30,35,40,45,50,55,60}; return lvl < 16 ? t[lvl] : 70; }
void  ItemRandomOption::Roll(ShineItem&) {}
int32 SetItemData::SetEffectStat(uint32, uint16 pieces) { return (int32)pieces * 5; }
uint32 WeaponTitle::ResolveTitle(uint32 kills) {
    if (kills > 100000) return 5; if (kills > 10000) return 4;
    if (kills >  1000)  return 3; if (kills >   100) return 2;
    if (kills >    10)  return 1; return 0;
}

// 17
bool ShineItemUse::TryUse(ShinePlayer* pk, ShineItem& r) {
    if (!pk) return false;
    if (!ItemAuthority::CanUse(pk, r)) return false;
    if (r.uiQty > 1) r.uiQty -= 1;
    SHINELOG_DEBUG("ShineItemUse cid=%u inx=%u", pk->GetCharID(), r.uiInxName);
    return true;
}
void ChargedItem::Tick(ShinePlayer*) {}
const ChargedEffectRow* ChargedItemEffectDataBox::Find(uint32 inx) {
    return ITableBase<ChargedEffectRow>::ms_pkTable
           ? ITableBase<ChargedEffectRow>::ms_pkTable->Find(inx) : NULL;
}
bool ItemMall::BuyById(ShinePlayer* pk, uint32, uint16 qty) { return pk && qty > 0; }

// 18
void ItemDropTable::Get(MobID, std::vector<DropEntry>& r) { r.clear(); } // EV_VERIFY

void ItemDropFromMob::Trigger(ShineMob* pkMob, ShinePlayer* pkKiller) {
    if (!pkMob) return;
    std::vector<DropEntry> kDrops; ItemDropTable::Get(pkMob->m_uiSpecies, kDrops);
    for (size_t i = 0; i < kDrops.size(); ++i) {
        if (s_kRng.NextRange(10000) < kDrops[i].uiWeight) {
            if (pkKiller) RewardInven::Push(pkKiller, (ItemID)kDrops[i].uiInxName, kDrops[i].uiQty);
        }
    }
}
void DropItemAnalyzer::Analyze(MobID s, std::vector<DropEntry>& r) { ItemDropTable::Get(s, r); }
Vec3 DropLocationManager::ScatterAround(const Vec3& c, uint32) {
    float a = (float)(s_kRng.NextRange(360)) * 0.01745329f;
    float r = (float)(s_kRng.NextRange(200)) * 0.01f;
    return Vec3(c.x + (float)cos(a) * r, c.y + (float)sin(a) * r, c.z);
}
CharID BelongDiceTable::PickWinner(const std::vector<CharID>& g) {
    if (g.empty()) return INVALID_CHARID;
    return g[s_kRng.NextRange((uint32)g.size())];
}
void RewardInven::Push(ShinePlayer*, ItemID, uint16) {}

} // namespace fiesta
