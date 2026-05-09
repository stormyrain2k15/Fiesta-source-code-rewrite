// Server/Zone/SoulStoneSystem.h
// 14 -- HP and SP soul stones.
//
// FINAL MODEL (all values data-driven from ParamXxxServer.txt):
//
//   * Two flat counters per character row: `uiHpCount` and `uiSpCount`.
//     NOT inventory items, NOT per-tier counters -- just two integers.
//
//   * Tier is implicit: heal amount + cap + vendor price are looked up
//     per (class, level) from `ClassParamTable` columns
//     `SoulHP / MAXSoulHP / PriceHPStone` and `SoulSP / MAXSoulSP /
//     PriceSPStone`. The character file already has 145 rows per class.
//
//   * HP and SP cooldowns are INDEPENDENT.
//
//   * Vendors: NPCs with `Role=Merchant RoleArg0=SoulStone` in
//     `World/NPC.txt`. `SoulStoneVendor::Buy` debits Vis + grants stones.
//
// EVIDENCE: DATA_CONFIRMED   World/Param*Server.txt SoulHP/MAXSoulHP/...
//           PDB_CONFIRMED   SoulStoneCount, SoulStoneUse_Req.
#ifndef FIESTA_ZONE_SOULSTONESYSTEM_H
#define FIESTA_ZONE_SOULSTONESYSTEM_H
#include "../Shared/ShineTypes.h"
#include "ClassParamTable.h"

namespace fiesta {

class ShinePlayer;

// =============================================================================
// EDITABLE TUNABLES (only the cooldown lives here; everything else is data)
// =============================================================================

// Cooldown per stone use, ms. HP and SP have independent clocks.
const uint32 kSoulStoneCooldownMs    = 1500;

// =============================================================================
//  RUNTIME STATE  (lives on the character DB row, NOT inventory)
// =============================================================================

struct SoulStoneCounts {
    uint16 uiHpCount;
    uint16 uiSpCount;
    uint64 uiNextHpUseTickMs;
    uint64 uiNextSpUseTickMs;

    SoulStoneCounts()
        : uiHpCount(0), uiSpCount(0), uiNextHpUseTickMs(0), uiNextSpUseTickMs(0) {}
};

enum eSoulStoneUseResult {
    SS_USE_OK             = 0,
    SS_USE_NONE_LEFT      = 1,
    SS_USE_ON_COOLDOWN    = 2,
    SS_USE_AT_MAX_HP      = 3,
    SS_USE_AT_MAX_SP      = 4,
    SS_USE_DEAD           = 5,
    SS_USE_NO_PARAM       = 6           // ClassParam not loaded for (class, level)
};

class SoulStoneSystem {
public:
    static eSoulStoneUseResult Use(ShinePlayer*      pkP,
                                    SoulStoneCounts& rC,
                                    bool             bIsHp);

    // Add stones (drop / quest / vendor). Clamped at the per-(class,level) cap.
    // Returns the number actually granted.
    static uint16 Grant(SoulStoneCounts& rC,
                        eShineClass      eClass,
                        uint16           uiLevel,
                        bool             bIsHp,
                        uint16           uiQty);

    // Time until the next HP / SP soul stone may be used (ms; 0 = ready).
    static uint32 RemainingHpCooldownMs(const SoulStoneCounts& rC);
    static uint32 RemainingSpCooldownMs(const SoulStoneCounts& rC);
};

} // namespace fiesta
#endif
