// Server/Zone/DeathReviveSystem.cpp
#include "DeathReviveSystem.h"
#include "ShineObject.h"
#include "../Shared/ShineLogSystem.h"

namespace fiesta {

// EV_VERIFY: live-game tunes this curve every few patches; reasonable default.
int32 DeathReviveSystem::ExpLossOnTownRevive(int32 nLevel) {
    if (nLevel < 10)  return 0;
    if (nLevel < 30)  return 10;     // 1.0% per-mille
    if (nLevel < 60)  return 20;
    if (nLevel < 100) return 30;
    return 40;                        // 4.0%
}

void DeathReviveSystem::GrantShineSoul(ShineSoulPouch& rPouch, uint16 uiQty) {
    uint32 sum = (uint32)rPouch.uiCount + (uint32)uiQty;
    if (sum > 0xFFFF) sum = 0xFFFF;
    rPouch.uiCount = (uint16)sum;
}

eReviveResult DeathReviveSystem::Choose(ShinePlayer*    pkP,
                                         ShineSoulPouch& rPouch,
                                         eReviveChoice   eChoice) {
    if (!pkP) return REV_INVALID;
    if (!pkP->IsDead()) return REV_NOT_DEAD;

    switch (eChoice) {
        case REVIVE_SHINE: {
            if (rPouch.uiCount == 0) return REV_NO_SHINE;
            --rPouch.uiCount;
            pkP->SetHP(pkP->GetMaxHP());
            pkP->SetSP(pkP->GetMaxSP());
            // (Caller un-flags death + broadcasts NC_BAT_RESURRECT-equivalent.)
            return REV_OK;
        }
        case REVIVE_TOWN: {
            pkP->SetHP(pkP->GetMaxHP());
            pkP->SetSP(pkP->GetMaxSP());
            // XP penalty applied by caller using ExpLossOnTownRevive(level).
            // Town warp performed by caller using FieldTable bind row.
            return REV_OK;
        }
        case REVIVE_RESURRECT: {
            // The caller looks up the queued resurrect-skill effect (if any).
            // This branch only validates the choice; the apply/restore path
            // happens in the skill resolver because it needs the caster
            // strength / class for the % HP restored.
            return REV_OK;
        }
        default:
            return REV_INVALID;
    }
}

} // namespace fiesta
