// Server/Zone/PowerGuardStoneSystem.h
// Power Stones (offense buff stones) and Guard Stones (defense buff
// stones). Same backing shape as soul stones: two flat counters per
// character row, two independent cooldowns; cap, per-use boost amount, and
// vendor price all sourced from `ClassParamTable` (per-(class, level) row).
// Power Stone: bumps WC + MA for kStoneBuffSec when consumed.
//   ClassParam columns:  MaxPwrStone, NumPwrStone, PricePwrStone,
//                        PwrStoneWC, PwrStoneMA
// Guard Stone: bumps AC + MR for kStoneBuffSec when consumed.
//   ClassParam columns:  MaxGrdStone, NumGrdStone, PriceGrdStone,
//                        GrdStoneAC, GrdStoneMR
#ifndef FIESTA_ZONE_POWERGUARDSTONESYSTEM_H
#define FIESTA_ZONE_POWERGUARDSTONESYSTEM_H
#include "ClassParamTable.h"

namespace fiesta {

class ShinePlayer;
class Inventory;

// Buff duration per stone (seconds). live game uses ~30 min
// per stone consumed; tune in-place if needed.
const uint32 kStoneBuffSec       = 1800;
const uint32 kStoneCooldownMs    = 1500;

struct StoneCounts {
    uint16 uiPwrCount;
    uint16 uiGrdCount;
    uint64 uiNextPwrUseTickMs;
    uint64 uiNextGrdUseTickMs;
    uint64 uiPwrBuffExpiresMs;          // 0 = no buff
    uint64 uiGrdBuffExpiresMs;
    int32  nActivePwrWC, nActivePwrMA;  // active boost amounts (carry-on equip change)
    int32  nActiveGrdAC, nActiveGrdMR;

    StoneCounts() {
        uiPwrCount = uiGrdCount = 0;
        uiNextPwrUseTickMs = uiNextGrdUseTickMs = 0;
        uiPwrBuffExpiresMs = uiGrdBuffExpiresMs = 0;
        nActivePwrWC = nActivePwrMA = nActiveGrdAC = nActiveGrdMR = 0;
    }
};

enum eStoneUseResult {
    STONE_USE_OK             = 0,
    STONE_USE_NONE_LEFT      = 1,
    STONE_USE_ON_COOLDOWN    = 2,
    STONE_USE_DEAD           = 3,
    STONE_USE_NO_PARAM       = 4
};

class PowerGuardStoneSystem {
public:
    // bIsPower = true -> Power Stone (WC/MA), false -> Guard Stone (AC/MR).
    static eStoneUseResult Use(ShinePlayer*  pkP,
                               StoneCounts&  rC,
                               bool          bIsPower);

    // Add stones (drop / quest / vendor). Cap = per-(class,level) MaxPwrStone /
    // MaxGrdStone. Returns the number actually granted.
    static uint16 Grant(StoneCounts& rC,
                        eShineClass  eClass,
                        uint16       uiLevel,
                        bool         bIsPower,
                        uint16       uiQty);

    // Per-tick: check buff expiry; if expired, zero the active boost.
    // Returns true if a buff just ended (caller broadcasts a stat-change).
    static bool TickBuff(StoneCounts& rC, bool bIsPower);

    // Read remaining cooldown (ms; 0 = ready).
    static uint32 RemainingPwrCooldownMs(const StoneCounts& rC);
    static uint32 RemainingGrdCooldownMs(const StoneCounts& rC);
};

class PowerGuardStoneVendor {
public:
    static bool IsVendor(const std::string& rNpcMobName);  // RoleArg0=PowerStone or GuardStone
    static eStoneUseResult Buy(ShinePlayer*       pkP,
                               const std::string& rNpcMobName,
                               class Inventory&   rInv,
                               StoneCounts&       rCounts,
                               bool               bIsPower,
                               uint16             uiQty);
};

} // namespace fiesta
#endif
