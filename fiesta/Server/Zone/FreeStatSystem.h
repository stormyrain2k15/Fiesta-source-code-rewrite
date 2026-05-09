// Server/Zone/FreeStatSystem.h
// "+ STR / + END / + DEX / + INT / + MEN" free-stat allocation,
// per-level point grants, refund tickets, and validation against
// MoverMain.shn's per-level stat cap.
//                                   ChrCommon.txt: 5 sub-tables w/ 181 rows each.
#ifndef FIESTA_ZONE_FREESTATSYSTEM_H
#define FIESTA_ZONE_FREESTATSYSTEM_H
#include "../Shared/ShineTypes.h"

namespace fiesta {

class ShinePlayer;

// Per-character free-stat ledger. Lives inside ShinePlayer (or
// ShinePlayerCharacter once that struct is wired up); this header just
// declares the manipulation surface.
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
    // hits the per-stat cap.
    static bool Allocate(FreeStatLedger& rL, uint8 bClass, uint16 uiLevel,
                         char chWhich, int32 nDelta);

    // Reset all stat allocations and refund all spent points (for a
    // "Stat Reset Ticket"). Returns the number of points refunded.
    static int32 RefundAll(FreeStatLedger& rL);

    // Cap check helper -- returns the remaining headroom from MoverMain
    // for the current (class, level) so callers can avoid Allocate when
    // it would fail. 0 = at cap.
    static int32 RemainingHeadroom(const FreeStatLedger& rL, uint8 bClass, uint16 uiLevel);
};

} // namespace fiesta
#endif
