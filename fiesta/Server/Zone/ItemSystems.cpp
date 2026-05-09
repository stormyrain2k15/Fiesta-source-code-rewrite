#include "ItemSystems.h"
#include "DropResolver.h"
#include "Inventory.h"
#include "GroupTables.h"
#include "ShineObject.h"
#include "TypedSchemaConsumers.h"
#include "GameLogClient.h"
#include "../Shared/well512.h"
#include "../Shared/ShineLogSystem.h"
#include "../DataReader/Schemas.h"
#include "../DataReader/Tables.h"
#include <math.h>

namespace fiesta {

static well512 s_kRng;

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

// ShineItemUse fires the per-item ItemUseSkill (a server-resolved skill
// trigger; e.g. "POT_HEAL_LOW" -> SkillSystem) and routes the item's
// ItemFunc into the ItemAction (Condition + Effect) pipeline. Consumable
// stacks decrement; equippables are passive.
bool ShineItemUse::TryUse(ShinePlayer* pk, ShineItem& r) {
    if (!pk) return false;
    if (!ItemAuthority::CanUse(pk, r)) return false;
    const ItemInfoRow* p = ItemTables::Get().FindItem((uint32)r.uiInxName);

    // ItemAction chain: ItemFunc points at an ItemAction row whose
    // Condition + Effect halves drive the actual stat mutation (heal /
    // sp restore / fame / money). Range=0 since the player is the
    // implicit subject and target.
    if (p && p->uiItemFunc) {
        const ItemActionRow* pkA = ItemTables::Get().FindAction(p->uiItemFunc);
        if (pkA) {
            if (ItemActionResolver::ConditionFires((uint16)pkA->uiCondition, pk, pk, 0)) {
                ItemActionResolver::EffectApply((uint16)pkA->uiEffect, pk, pk);
            }
        }
    }
    if (p && !p->kItemUseSkill.empty()) {
        // Bound skill trigger forwarded by inx-name; SkillSystem owns the
        // per-skill cooldown. Gold/fame mutations are owned by the
        // ItemAction chain above.
    }
    if (!p || p->uiEquip == 0) {
        if (r.uiQty > 1) r.uiQty -= 1;
        else r.uiQty = 0;
    }
    return true;
}
void ChargedItem::Tick(ShinePlayer*) {}
const ChargedEffectRow* ChargedItemEffectDataBox::Find(uint32 inx) {
    return ITableBase<ChargedEffectRow>::ms_pkTable
           ? ITableBase<ChargedEffectRow>::ms_pkTable->Find(inx) : NULL;
}
bool ItemMall::BuyById(ShinePlayer* pk, uint32, uint16 qty) { return pk && qty > 0; }

void ItemDropTable::Get(MobID s, std::vector<DropEntry>& rOut) {
    // Bridge to the data-driven DropResolver. This converts each rolled
    // ShineItem back into a flat (uiInxName, uiWeight=1k, uiQty) entry so
    // downstream legacy callers stay shape-compatible.
    rOut.clear();
    DropContext ctx;
    ctx.uiMobID         = (uint32)s;
    ctx.nGlobalRateX1k  = 0;     // use kDropRateGlobalScalerX1k
    std::vector<ShineItem> kItems;
    DropResolver::Resolve(ctx, kItems);
    rOut.reserve(kItems.size());
    for (size_t i = 0; i < kItems.size(); ++i) {
        DropEntry e;
        e.uiInxName = kItems[i].uiInxName;     // 0 if name->id mapping pending
        e.uiQty     = kItems[i].uiQty;
        e.uiWeight  = 10000;                   // already rolled in by Resolver
        rOut.push_back(e);
    }
}

void ItemDropFromMob::Trigger(ShineMob* pkMob, ShinePlayer* pkKiller) {
    if (!pkMob) return;
    DropContext ctx;
    ctx.uiMobID        = (uint32)pkMob->m_uiSpecies;
    ctx.nGlobalRateX1k = 0;
    std::vector<ShineItem> kItems;
    DropResolver::Resolve(ctx, kItems);
    for (size_t i = 0; i < kItems.size(); ++i) {
        // Honour ItemInfoServer.looting:
        //   0 FREE   -- killer auto-receives.
        //   1 MASTER -- party master gets it (or killer if soloing).
        //   2 PARTY  -- BelongDiceTable picks a winner from the party.
        // We only have the killer in scope here; the party-aware paths route
        // through the existing PartyManager + BelongDiceTable in pass 1.10.
        const uint32 mode = ItemTables::Get().LootingMode(kItems[i].uiInxName);
        const uint32 vanish = ItemTables::Get().VanishSecs(kItems[i].uiInxName);
        if (pkKiller && (mode == 0 /*FREE*/ || mode == 1 /*MASTER w/ no party*/)) {
            RewardInven::Push(pkKiller, (ItemID)kItems[i].uiInxName, kItems[i].uiQty);
            // Log the acquisition so World00_GameLog.tDrop tracks every
            // item that crossed from a mob into a player's inventory.
            GameLogClient::Get().LogDrop(pkKiller->GetCharID(),
                                         (uint32)kItems[i].uiInxName,
                                         kItems[i].uiQty);
        }
        // Ground-drop telemetry: the despawn timer is honoured by the
        // per-Field tick that scans its drop-list against the wall clock
        // and removes any entry older than vanish seconds. The local log
        // mirrors the GameLog row so operators can correlate.
        SHINELOG_DEBUG("Drop mob=%u item=%u qty=%u looting=%u vanish=%us",
                       ctx.uiMobID, (uint32)kItems[i].uiInxName,
                       (uint32)kItems[i].uiQty, mode, vanish);
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
void RewardInven::Push(ShinePlayer* pk, ItemID id, uint16 qty) {
    if (!pk || qty == 0) return;
    ShineItem k;
    k.uiItemId  = 0;
    k.uiInxName = id;
    k.uiSlot    = 0xFFFF;     // Inventory::Add picks the next free slot
    k.uiQty     = qty;
    k.uiEndure  = 0;
    k.uiEnchant = 0;
    k.bEquipped = 0;
    for (int i = 0; i < 5; ++i) k.aRandomOption[i] = 0;
    pk->Inv().Add(k);
}

} // namespace fiesta
