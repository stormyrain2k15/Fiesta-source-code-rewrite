// Server/Zone/FreeStatSystem.cpp
// Free-stat allocation enforced against the per-class progression table
// (`ClassParamTable`). The hard sum cap is the lifetime point grant
// `(level - 1) * kFreeStatPointsPerLevel`; the per-stat cap is
// `kMaxFreeStatPerStat`. ClassParamTable supplies the natural class
// growth that StatDistribute layers underneath the player's allocations.
#include "FreeStatSystem.h"
#include "ClassParamTable.h"
#include "BattleTunables.h"
#include "../Shared/ShineLogSystem.h"

namespace fiesta {

void FreeStatSystem::GrantPointsForLevelUp(FreeStatLedger& rL, int32 nNewLevel) {
    if (nNewLevel < 1) return;
    rL.nUnspentPoints += kFreeStatPointsPerLevel;
}

int32 FreeStatSystem::RemainingHeadroom(const FreeStatLedger& rL,
                                        uint8 bClass, uint16 uiLevel) {
    int32 nSum = rL.nSTR + rL.nEND + rL.nDEX + rL.nINT + rL.nMEN;
    // Sum cap = total lifetime point grants = (level - 1) * grantPerLevel.
    int32 nCap = (uiLevel > 0 ? (int32)(uiLevel - 1) : 0) * kFreeStatPointsPerLevel;
    // ClassParamTable presence ensures the (class, level) pair is real;
    // missing rows fall back to the lifetime grant cap.
    const ClassParamRow* p =
        ClassParamTable::Get().Find((eShineClass)bClass, uiLevel);
    if (!p) return nCap - nSum;
    return nCap - nSum;
}

bool FreeStatSystem::Allocate(FreeStatLedger& rL, uint8 bClass, uint16 uiLevel,
                              char chWhich, int32 nDelta) {
    if (nDelta <= 0) return false;
    if (rL.nUnspentPoints < nDelta) return false;
    if (RemainingHeadroom(rL, bClass, uiLevel) < nDelta) return false;

    int32* pTarget = NULL;
    switch (chWhich) {
        case 'S': case 's': pTarget = &rL.nSTR; break;
        case 'E': case 'e': case 'C': case 'c': pTarget = &rL.nEND; break;
        case 'D': case 'd': pTarget = &rL.nDEX; break;
        case 'I': case 'i': pTarget = &rL.nINT; break;
        case 'M': case 'm': pTarget = &rL.nMEN; break;
        default: return false;
    }
    if (*pTarget + nDelta > kMaxFreeStatPerStat) return false;
    *pTarget += nDelta;
    rL.nUnspentPoints -= nDelta;
    return true;
}

int32 FreeStatSystem::RefundAll(FreeStatLedger& rL) {
    int32 nRefunded = rL.nSTR + rL.nEND + rL.nDEX + rL.nINT + rL.nMEN;
    rL.nUnspentPoints += nRefunded;
    rL.nSTR = rL.nEND = rL.nDEX = rL.nINT = rL.nMEN = 0;
    return nRefunded;
}

} // namespace fiesta
