// Server/Zone/DropResolver.cpp
#include "DropResolver.h"
#include "WorldTables.h"
#include "BattleTunables.h"
#include "GroupTables.h"
#include "../DataReader/Schemas.h"
#include "../DataReader/ITableBase.h"
#include "../Shared/well512.h"
#include "../Shared/ShineLogSystem.h"
#include <stdlib.h>

namespace fiesta {

static well512 s_kRng;

static int RollPermilleHit(int32 permille, int32 globalRateX1k) {
    int32 effective = (permille * globalRateX1k) / 1000;
    int32 roll = (int32)(s_kRng.NextDouble() * 1000.0);
    return (roll < effective) ? 1 : 0;
}

// Pick an upgrade level from the per-+N chance vector (16 entries, permille).
static uint8 RollUpgrade(const ItemDropGroupRow& g) {
    int32 total = 0;
    for (int i = 0; i < 16; ++i) total += g.aUpgradeChance[i];
    if (total <= 0) return 0;
    int32 roll = (int32)(s_kRng.NextDouble() * (double)total);
    int32 acc = 0;
    for (int i = 0; i < 16; ++i) {
        acc += g.aUpgradeChance[i];
        if (roll < acc) return (uint8)i;
    }
    return 0;
}

static void RollRandomOptions(const std::string& rItem, ShineItem& rOut) {
    const LegacyRandomOptionRow* p = RandomOptionTable::Get().Find(rItem);
    if (!p) return;
    int32 cnt = p->uiMinOpCount;
    if (p->uiMaxOpCount > p->uiMinOpCount)
        cnt += (int32)(s_kRng.NextDouble() * (double)(p->uiMaxOpCount - p->uiMinOpCount + 1));
    if (cnt > 5) cnt = 5;
    bool used[5] = { false, false, false, false, false };
    for (int n = 0; n < cnt; ++n) {
        int slot = (int)(s_kRng.NextDouble() * 5.0);
        if (slot < 0 || slot > 4) slot = 0;
        if (used[slot]) {
            for (int i = 0; i < 5; ++i) if (!used[i]) { slot = i; break; }
        }
        used[slot] = true;
        uint16 lo = p->aMin[slot];
        uint16 hi = p->aMax[slot];
        uint16 v  = lo;
        if (hi > lo) v = (uint16)(lo + (s_kRng.NextDouble() * (double)(hi - lo + 1)));
        rOut.aRandomOption[slot] = v;
    }
}

void DropResolver::Resolve(const DropContext& rCtx, std::vector<ShineItem>& rOut) {
    // -----------------------------------------------------------------
    // Step 1: mob -> drop-table id.
    //
    // The lookup needs `MobInfoServerRow.DropItemListIdx`. That column
    // exists in MobInfoServer.shn and is generated into Schemas.h by
    // SchemaGen. Once Schemas.h declares the field this becomes a direct
    // read; for now we route through the generic ITableBase string
    // export so we can still resolve at runtime even if the typed
    // accessor isn't compiled in.
    // -----------------------------------------------------------------
    uint32 dropTableId = 0;
    {
        const ITableBase<MobInfoServerRow>* pMobTab = ITableBase<MobInfoServerRow>::ms_pkTable;
        if (!pMobTab) return;
        const MobInfoServerRow* pMob = pMobTab->Find(rCtx.uiMobID);
        if (!pMob) return;
        // column name "DropItemListIdx" matches the supplied
        // MobInfoServer.shn header. If the field is named differently in
        // the generated struct, edit this single line.
        dropTableId = (uint32)pMob->DropItemListIdx;
    }
    if (dropTableId == 0) return;

    // -----------------------------------------------------------------
    // Step 2: roll every row of ItemDropTable that targets this group.
    // -----------------------------------------------------------------
    const ITableBase<ItemDropTableRow>* pTab = ITableBase<ItemDropTableRow>::ms_pkTable;
    if (!pTab) return;
    int32 globalRate = rCtx.nGlobalRateX1k > 0 ? rCtx.nGlobalRateX1k : kDropRateGlobalScalerX1k;

    const std::vector<ItemDropTableRow>& rows = pTab->Rows();
    for (size_t i = 0; i < rows.size(); ++i) {
        const ItemDropTableRow& r = rows[i];
        if ((uint32)r.DropTableID != dropTableId) continue;
        if (!RollPermilleHit((int32)r.Permill, globalRate)) continue;

        const ItemDropGroupRow* g = ItemDropGroupTable::Get().Find(r.ItemGroupIdx);
        if (!g) continue;

        ShineItem item;
        item.uiItemId   = 0;
        item.uiInxName  = 0;          // resolved by Inventory::Add when item-name is mapped
        item.uiSlot     = 0xFFFF;
        item.uiEndure   = 0;
        item.uiEnchant  = 0;
        item.bEquipped  = 0;
        item.kItemIndex = g->kItemID;
        item.uiQty      = g->uiMinQty;
        if (g->uiMaxQty > g->uiMinQty) {
            uint16 q = g->uiMinQty + (uint16)(s_kRng.NextDouble() * (double)(g->uiMaxQty - g->uiMinQty + 1));
            item.uiQty = q;
        }
        item.uiEnchant  = RollUpgrade(*g);
        for (int k = 0; k < 5; ++k) item.aRandomOption[k] = 0;
        // Resolve the random-option pool: ItemInfoServer.RandomOptionDropGroup
        // overrides the per-item lookup when present (it lets multiple
        // items share a single roll table, e.g. all "Lv60 Mage Robe" items
        // pull from the same option pool).
        const ItemInfoServerRow* pSrv = ItemTables::Get().FindServerByInx(g->kItemID);
        if (pSrv && !pSrv->kRandomOptionDropGroup.empty())
            RollRandomOptions(pSrv->kRandomOptionDropGroup, item);
        else
            RollRandomOptions(g->kItemID, item);
        rOut.push_back(item);
    }
}

} // namespace fiesta
