// Server/Zone/FreeStatSystem.h
// Per-character free-stat allocation. The per-class progression baseline
// comes from `ClassParamTable` (one row per (class, level)); this module
// enforces the sum/per-stat caps on top of that baseline. Lifetime sum
// cap = (level - 1) * kFreeStatPointsPerLevel.
#ifndef SHINE_ZONE_FREESTATSYSTEM_H
#define SHINE_ZONE_FREESTATSYSTEM_H
#include "../Shared/ShineTypes.h"

namespace shine {

class ShinePlayer;

// Per-character free-stat ledger. ShinePlayer::BuildFreeStatLedger /
// ApplyFreeStatLedger marshal between the in-memory player state and
// this view; FreeStatSystem mutates the view.
struct FreeStatLedger {
    int32 nSTR;
    int32 nEND;
    int32 nDEX;
    int32 nINT;
    int32 nMEN;
    int32 nUnspentPoints;       // increments by kFreeStatPointsPerLevel per level
};

class FreeStatSystem {
public:
    // Award the per-level points after a level-up.
    static void GrantPointsForLevelUp(FreeStatLedger& rL, int32 nNewLevel);

    // Increment one stat by `nDelta`. Returns false if not enough points or
    // hits the per-stat / lifetime-sum cap.
    static bool Allocate(FreeStatLedger& rL, uint8 bClass, uint16 uiLevel,
                         char chWhich, int32 nDelta);

    // Reset all stat allocations and refund all spent points (for a
    // "Stat Reset Ticket"). Returns the number of points refunded.
    static int32 RefundAll(FreeStatLedger& rL);

    // Cap check helper -- returns the remaining headroom (`nCap - sum`)
    // for the current (class, level). 0 = at cap.
    static int32 RemainingHeadroom(const FreeStatLedger& rL, uint8 bClass, uint16 uiLevel);
};

} // namespace shine
#endif

