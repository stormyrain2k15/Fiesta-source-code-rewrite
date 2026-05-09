// Server/Zone/ItemUpgrade.cpp
#include "ItemUpgrade.h"
#include "BattleTunables.h"
#include "../DataReader/Schemas.h"
#include "../DataReader/ITableBase.h"
#include "../Shared/well512.h"
#include "../Shared/ShineLogSystem.h"

namespace fiesta {

static well512 s_kRng;

eUpgradeResult ItemUpgrade::Try(Inventory& kInv, uint32 uiItemId, bool bUseLuckStone) {
    // Find the target item.
    const std::vector<ShineItem>& vAll = kInv.All();
    int idx = -1;
    for (size_t i = 0; i < vAll.size(); ++i)
        if (vAll[i].uiItemId == uiItemId) { idx = (int)i; break; }
    if (idx < 0) return UPGRADE_BLOCKED;

    // Resolve item info row.
    const ShineItem& kItem = vAll[idx];
    const ItemInfoRow* p = ITableBase<ItemInfoRow>::ms_pkTable
        ? ITableBase<ItemInfoRow>::ms_pkTable->Find(kItem.uiInxName) : NULL;
    if (!p) return UPGRADE_BLOCKED;

    // Cap checks.
    if (p->UpLimit == 0) return UPGRADE_BLOCKED;
    if (kItem.uiEnchant >= p->UpLimit) return UPGRADE_BLOCKED;
    if (kItem.uiEnchant >= kMaxUpgradeLevel) return UPGRADE_BLOCKED;

    // Compute success rate (per-mille).
    int32 sucRate = (int32)p->UpSucRatio;
    if (bUseLuckStone) sucRate += (int32)p->UpLuckRatio;
    sucRate = (sucRate * kUpgradeSucScalerX1k) / 1000;
    if (sucRate < 0)    sucRate = 0;
    if (sucRate > 1000) sucRate = 1000;

    // Roll.
    int32 roll = (int32)(s_kRng.NextDouble() * 1000.0);
    bool  bSuccess = (roll < sucRate);

    // Locate a non-const reference to the item to mutate. The Inventory
    // exposes only a const view, so we go through Add/Remove.
    ShineItem mutated = kItem;

    if (bSuccess) {
        mutated.uiEnchant = (uint16)(mutated.uiEnchant + 1);
        kInv.Remove(uiItemId);
        kInv.Add(mutated);
        return UPGRADE_OK;
    }

    // Failure consequences -- table-driven by current +N level.
    uint8 lv = (uint8)mutated.uiEnchant;
    if (lv >= (uint8)(sizeof(kFailPenaltyAtLevel) / sizeof(kFailPenaltyAtLevel[0])))
        lv = (uint8)(sizeof(kFailPenaltyAtLevel) / sizeof(kFailPenaltyAtLevel[0]) - 1);
    uint8 penalty = kFailPenaltyAtLevel[lv];

    if (penalty == 0) {
        return UPGRADE_FAIL;            // safe tier, item untouched
    }
    if (penalty == 1) {
        if (mutated.uiEnchant > 0) mutated.uiEnchant = (uint16)(mutated.uiEnchant - 1);
        kInv.Remove(uiItemId);
        kInv.Add(mutated);
        return UPGRADE_DOWNGRADE;
    }
    // penalty == 2 -> destroy.
    kInv.Remove(uiItemId);
    return UPGRADE_DESTROY;
}

} // namespace fiesta
