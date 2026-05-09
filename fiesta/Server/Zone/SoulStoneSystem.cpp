// Server/Zone/SoulStoneSystem.cpp
#include "SoulStoneSystem.h"
#include "ShineObject.h"
#include "../Shared/ShineLogSystem.h"

namespace fiesta {

// Per-tier restoration fractions (x100 = %).
static const int32 kHpLoPct = 3000;     // 30%
static const int32 kHpHiPct = 6000;     // 60%
static const int32 kSpLoPct = 3000;
static const int32 kSpHiPct = 6000;

// XP penalty on town-respawn (per-mille of current level XP).
static int32 TownXpPenaltyPerMille(int32 nLevel) {
    // EV_VERIFY: live game tunes this curve every few patches; sane default.
    if (nLevel < 10)  return 0;
    if (nLevel < 30)  return 10;     // 1.0%
    if (nLevel < 60)  return 20;
    if (nLevel < 100) return 30;
    return 40;                        // 4.0% at endgame
}

void SoulStoneSystem::RestoreFraction(ShinePlayer* pkP, int32 nHpPctX100, int32 nSpPctX100) {
    if (!pkP) return;
    if (nHpPctX100 > 0) {
        int32 hp = pkP->GetMaxHP() * nHpPctX100 / 10000;
        pkP->SetHP(hp);
    }
    if (nSpPctX100 > 0) {
        int32 sp = pkP->GetMaxSP() * nSpPctX100 / 10000;
        pkP->SetSP(sp);
    }
}

bool SoulStoneSystem::TryRevive(ShinePlayer* pkP, SoulStoneCounts& rC, eSoulRevive eType) {
    if (!pkP) return false;
    switch (eType) {
        case SOUL_REVIVE_HP_LO:
            if (rC.uiHpLo == 0) return false;
            --rC.uiHpLo; RestoreFraction(pkP, kHpLoPct, 0); return true;
        case SOUL_REVIVE_HP_HI:
            if (rC.uiHpHi == 0) return false;
            --rC.uiHpHi; RestoreFraction(pkP, kHpHiPct, 0); return true;
        case SOUL_REVIVE_SP_LO:
            if (rC.uiSpLo == 0) return false;
            --rC.uiSpLo; RestoreFraction(pkP, 0, kSpLoPct); return true;
        case SOUL_REVIVE_SP_HI:
            if (rC.uiSpHi == 0) return false;
            --rC.uiSpHi; RestoreFraction(pkP, 0, kSpHiPct); return true;
        case SOUL_REVIVE_SHINE:
            if (rC.uiShineSoul == 0) return false;
            --rC.uiShineSoul; RestoreFraction(pkP, 10000, 10000); return true;
        case SOUL_REVIVE_TOWN: {
            RestoreFraction(pkP, 10000, 10000);
            // XP penalty handled by caller using ExpLossOnTownRevive().
            return true;
        }
        default: return false;
    }
}

int32 SoulStoneSystem::ExpLossOnTownRevive(int32 nLevel) {
    return TownXpPenaltyPerMille(nLevel);
}

} // namespace fiesta
