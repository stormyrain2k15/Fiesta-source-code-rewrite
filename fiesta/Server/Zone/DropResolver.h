// Server/Zone/DropResolver.h
// 15 -- sidecar that turns a "this mob just died" event into a list of
// concrete `ShineItem` instances spawned on the floor.
//
// Inputs (already loaded by other systems):
//   * MobInfoServer.shn          -> drop table id by mob species
//   * World/ItemDropTable.txt    -> per-table list of (group, permill, qty)
//   * World/ItemDropGroup.txt    -> group -> item id + per-+N upgrade chance
//   * World/RandomOptionTable.txt-> per-item random-option ranges
//   * ItemInfo.shn               -> item type / max-stack / default upgrade
//
// EVIDENCE: PDB_CONFIRMED   symbols: cDropTable, cItemDropGroup,
//                                    cRandomOption, ItemDropResolver.
#ifndef FIESTA_ZONE_DROPRESOLVER_H
#define FIESTA_ZONE_DROPRESOLVER_H
#include "../Shared/ShineTypes.h"
#include "Inventory.h"             // ShineItem
#include <vector>
#include <string>

namespace fiesta {

struct DropContext {
    uint16      uiMobID;
    uint16      uiMobLevel;
    int32       nKillerLevel;       // for level-gap drop scaling
    bool        bIsParty;
    bool        bIsKQ;
    int32       nGlobalRateX1k;     // edit point: kDropRateGlobalScalerX1k
};

class DropResolver {
public:
    // Roll the drop table for a dead mob; append concrete `ShineItem`
    // results to `rOut`. Each result is populated with item id, qty,
    // upgrade level, and (when applicable) random-option blob.
    static void Resolve(const DropContext& rCtx, std::vector<ShineItem>& rOut);
};

} // namespace fiesta
#endif
