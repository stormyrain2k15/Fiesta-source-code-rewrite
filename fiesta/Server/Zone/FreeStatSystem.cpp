// Server/Zone/FreeStatSystem.cpp
#include "FreeStatSystem.h"
#include "MoverTables.h"
#include "BattleTunables.h"
#include "../Shared/ShineLogSystem.h"

namespace fiesta {

void FreeStatSystem::GrantPointsForLevelUp(FreeStatLedger& rL, int32 nNewLevel) {
    if (nNewLevel < 1) return;
    rL.nUnspentPoints += kFreeStatPointsPerLevel;
}

int32 FreeStatSystem::RemainingHeadroom(const FreeStatLedger& rL, uint8 bClass, uint16 uiLevel) {
    int32 sum = rL.nSTR + rL.nEND + rL.nDEX + rL.nINT + rL.nMEN;
    const MoverMainRow* p = MoverMainTable::Get().Find(bClass, uiLevel);
    int32 cap = p ? p->nMaxStatTotal : (kMaxFreeStatPerStat * 5);
    return cap - sum;
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
    int32 refunded = rL.nSTR + rL.nEND + rL.nDEX + rL.nINT + rL.nMEN;
    rL.nUnspentPoints += refunded;
    rL.nSTR = rL.nEND = rL.nDEX = rL.nINT = rL.nMEN = 0;
    return refunded;
}

} // namespace fiesta
