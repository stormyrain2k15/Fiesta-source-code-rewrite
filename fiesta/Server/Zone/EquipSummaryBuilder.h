// Server/Zone/EquipSummaryBuilder.h
// 15 -- composes an EQUIPSUMMARY by walking equipped slots, doing ItemInfo.shn
// lookups, and adding upgrade-level bonuses computed from BasicUpInx /
// AddUpInx.
//
// EVIDENCE: PDB_CONFIRMED   symbol: ItemTotalInformation::ToBattleStat,
//                                   EquipEnumChanger.
#ifndef FIESTA_ZONE_EQUIPSUMMARYBUILDER_H
#define FIESTA_ZONE_EQUIPSUMMARYBUILDER_H
#include "StatDistribute.h"
#include "Inventory.h"

namespace fiesta {

// Walks all equipped items in `kInv`, looks up their ItemInfo rows, and
// fills `pOut` with the summed stats (incl. upgrade-level absolutes).
void BuildEquipSummary(EQUIPSUMMARY* pOut, const Inventory& kInv);

// Per-item helper: read one item's contribution into a summary. Handles
// upgrade level via BasicUpInx + AddUpInx * (uiEnchant - 1).
void AddItemContribution(EQUIPSUMMARY* pOut, const ShineItem& kItem);

} // namespace fiesta
#endif
