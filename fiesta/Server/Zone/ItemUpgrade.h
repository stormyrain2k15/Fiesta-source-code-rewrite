// Server/Zone/ItemUpgrade.h
// the +N enhancement / upgrade flow.
// Inputs (ItemInfo.shn columns):
//   * UpLimit       -- maximum +N this item type can reach.
//   * UpSucRatio    -- success probability per-mille (1000 = 100.00%).
//   * UpLuckRatio   -- additional probability if a Luck stone is consumed.
//   * UpResource    -- which item class is consumed as the upgrade resource
//                      (refer to docs/SHN Documentation/UpResource.md).
//   * BasicUpInx    -- bonus delta on a successful +N -> +N+1.
//   * AddUpInx      -- additional delta starting from +2 onwards.
// Outputs: success/fail/destroy/downgrade applied to the item in-place; on
// success / downgrade the new enchant level is persisted via
// CharDBClient::ItemSetOption (uiType = 1 = enchant level), and on destroy
// via CharDBClient::ItemDelete.
#ifndef FIESTA_ZONE_ITEMUPGRADE_H
#define FIESTA_ZONE_ITEMUPGRADE_H
#include "Inventory.h"

namespace fiesta {

class ShinePlayer;

enum eUpgradeResult {
    UPGRADE_OK        = 0,    // +N -> +N+1
    UPGRADE_FAIL      = 1,    // no change
    UPGRADE_DOWNGRADE = 2,    // +N -> +N-1
    UPGRADE_DESTROY   = 3,    // item destroyed
    UPGRADE_BLOCKED   = 4     // not allowed (not upgradeable / at cap)
};

class ItemUpgrade {
public:
    // Attempt to upgrade a single item. Consumes one resource from the same
    // inventory if found. bUseLuckStone applies UpLuckRatio bonus.
    // The outNewEnchant/outItemKey are populated with the post-attempt
    // state so the caller can persist (CharDB) and ack the client.
    static eUpgradeResult Try(Inventory& kInv, uint32 uiItemId, bool bUseLuckStone,
                              uint16& uiNewEnchantOut, uint64& uiItemKeyOut);

    // High-level entry: attempt + persist + ack the client. Used by
    // ZoneHandlers when NC_ITEM_UPGRADE_REQ arrives.
    static eUpgradeResult ResolveForPlayer(ShinePlayer* pkPlayer,
                                           uint32 uiItemId, bool bUseLuckStone);
};

} // namespace fiesta
#endif

