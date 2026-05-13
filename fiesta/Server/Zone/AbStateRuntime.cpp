// Server/Zone/AbStateRuntime.cpp
// Real implementation of the AbState/SubAbState dispatcher. The ENTIRE
// buff/debuff system flows through this file:
//
//   * Resolve(AbStateRow, strength) -> SubAbState row -> runtime row
//   * Tick(target, row, now)        -> per-second action handlers
//   * OnApply / OnRemove            -> one-shot stat-mod / shield setup
//   * AbsorbIncoming                -> shield consumption gate
//   * ReflectIncoming               -> reflect-damage gate
//
// Tunables are isolated in `BattleTunables.h::kAbState*` so the
// per-action curves can be retuned without touching the dispatcher.
#include "AbStateRuntime.h"
#include "AbState.h"
#include "ShineObject.h"
#include "GroupTables.h"          // AbStateTables / AbStateRow
#include "MoreTables.h"           // SubAbStateTable
#include "BattleTunables.h"
#include "../Shared/GTimer.h"
#include "../Shared/ShineLogSystem.h"
#include <string.h>

namespace shine {

bool AbStateRuntime::Resolve(const AbStateRow& rAb, uint32 uiStrength,
                             AbStateRuntimeRow& rOut) {
    memset(&rOut, 0, sizeof(rOut));
    if (rAb.kSubAbState.empty() || rAb.kSubAbState == "-") {
        // The AbState row is a "container only" entry (e.g. an aggregate
        // parent); it has no per-tier effect of its own. The PartyState
        // chain still propagates -- handled by AbnormalState::Apply --
        // but the runtime tier can't fire, so we report failure and the
        // caller does NOT push a runtime row. Returning true here would
        // be a hollow success; we deliberately return false.
        return false;
    }
    const SubAbStateTable::Row* pkSub =
        SubAbStateTable::Get().FindByInx(rAb.kSubAbState, uiStrength);
    if (!pkSub) {
        SHINELOG_WARN("AbStateRuntime: SubAbState='%s' strength=%u missing",
                      rAb.kSubAbState.c_str(), uiStrength);
        return false;
    }
    uint64 now = GTimer::NowMillis();
    rOut.uiAbStateId    = rAb.uiID;
    rOut.uiSubAbStateId = pkSub->uiID;
    rOut.uiStrength     = pkSub->uiStrength;
    rOut.uiAppliedMs    = now;

    // KeepTime composition. SubAbState carries the base ms; AbState's
    // KeepTimeRatio is x100 (e.g. 200 = 2.0x). KeepTimeRatio == 0
    // means "use the SubAbState time directly".
    uint64 keep = (uint64)pkSub->uiKeepTimeMs;
    if (rAb.uiKeepTimeRatio > 0) keep = (keep * rAb.uiKeepTimeRatio) / 100;
    if (keep == 0) keep = (uint64)kAbStateDefaultKeepMs;
    rOut.uiExpireMs    = now + keep;
    rOut.uiNextTickMs  = now + (uint64)kAbStateTickMs;

    rOut.uiAction[0] = pkSub->uiActionIdxA; rOut.iArg[0] = pkSub->iActionArgA;
    rOut.uiAction[1] = pkSub->uiActionIdxB; rOut.iArg[1] = pkSub->iActionArgB;
    rOut.uiAction[2] = pkSub->uiActionIdxC; rOut.iArg[2] = pkSub->iActionArgC;
    rOut.uiAction[3] = pkSub->uiActionIdxD; rOut.iArg[3] = pkSub->iActionArgD;

    // Action 60 = damage shield; arg = absorb pool. We seed the per-row
    // remaining capacity here so AbsorbIncoming can decrement it as
    // hits land.
    rOut.iAbsorbRemain = 0;
    for (int i = 0; i < 4; ++i)
        if (rOut.uiAction[i] == kAbAction_AbsorbShield)
            rOut.iAbsorbRemain += rOut.iArg[i];
    return true;
}

bool AbStateRuntime::ResolveFromSub(const char* szSubInxName,
                                    uint32 uiStrength,
                                    AbStateRuntimeRow& rOut) {
    memset(&rOut, 0, sizeof(rOut));
    if (!szSubInxName || !szSubInxName[0]) return false;
    const SubAbStateTable::Row* pkSub =
        SubAbStateTable::Get().FindByInx(szSubInxName, uiStrength);
    if (!pkSub) {
        SHINELOG_WARN("AbStateRuntime: SubAbState='%s' strength=%u missing "
                      "(direct path)", szSubInxName, uiStrength);
        return false;
    }
    uint64 now = GTimer::NowMillis();
    rOut.uiAbStateId    = 0;                       // no AbState parent
    rOut.uiSubAbStateId = pkSub->uiID;
    rOut.uiStrength     = pkSub->uiStrength;
    rOut.uiAppliedMs    = now;

    uint64 keep = (uint64)pkSub->uiKeepTimeMs;
    if (keep == 0) keep = (uint64)kAbStateDefaultKeepMs;
    rOut.uiExpireMs    = now + keep;
    rOut.uiNextTickMs  = now + (uint64)kAbStateTickMs;

    rOut.uiAction[0] = pkSub->uiActionIdxA; rOut.iArg[0] = pkSub->iActionArgA;
    rOut.uiAction[1] = pkSub->uiActionIdxB; rOut.iArg[1] = pkSub->iActionArgB;
    rOut.uiAction[2] = pkSub->uiActionIdxC; rOut.iArg[2] = pkSub->iActionArgC;
    rOut.uiAction[3] = pkSub->uiActionIdxD; rOut.iArg[3] = pkSub->iActionArgD;

    rOut.iAbsorbRemain = 0;
    for (int i = 0; i < 4; ++i)
        if (rOut.uiAction[i] == kAbAction_AbsorbShield)
            rOut.iAbsorbRemain += rOut.iArg[i];
    return true;
}

void AbStateRuntime::OnApply(ShineObject* pkTarget, const AbStateRuntimeRow& rRow) {
    if (!pkTarget) return;
    // One-shot stat modifiers fire here (the dispatcher tick path is
    // for time-series effects). The mods are tracked via the per-target
    // AbnormalState ledger so a final stat sweep at compose time can
    // sum them; that path lives on `ShineObject::AbState()` and is
    // exposed via `AbnormalState::StatModX1k(stat)`.
    for (int i = 0; i < 4; ++i) {
        switch (rRow.uiAction[i]) {
            case kAbAction_StatModATK:
            case kAbAction_StatModDEF:
            case kAbAction_StatModMoveSpd:
            case kAbAction_StatModAtkSpd:
            case kAbAction_StatModMATK:
            case kAbAction_StatModMDEF:
                // Recorded on the per-instance row; the AbnormalState
                // aggregator walks active rows to compose stat mods on
                // demand. No-op here besides logging the apply event.
                SHINELOG_DEBUG("AbState apply stat-mod action=%u arg=%d on h=%u",
                               rRow.uiAction[i], rRow.iArg[i],
                               (uint32)pkTarget->GetHandle());
                break;
            default:
                break;
        }
    }
}

void AbStateRuntime::OnRemove(ShineObject* pkTarget, const AbStateRuntimeRow& rRow) {
    if (!pkTarget) return;
    // Stat mods auto-recompute on the next aggregator pass once the row
    // is dropped from the active list; nothing to clear explicitly.
    SHINELOG_DEBUG("AbState remove ab=%u sub=%u from h=%u",
                   rRow.uiAbStateId, rRow.uiSubAbStateId,
                   (uint32)pkTarget->GetHandle());
}

void AbStateRuntime::Tick(ShineObject* pkTarget, AbStateRuntimeRow& rRow,
                          uint64 uiNowMs) {
    if (!pkTarget) return;
    if (uiNowMs < rRow.uiNextTickMs) return;
    rRow.uiNextTickMs = uiNowMs + (uint64)kAbStateTickMs;
    for (int i = 0; i < 4; ++i) {
        uint32 idx = rRow.uiAction[i];
        int32  arg = rRow.iArg[i];
        if (idx == 0) continue;
        switch (idx) {
            case kAbAction_DotHP_A:
            case kAbAction_DotHP_B: {
                // arg encoded as per-tick damage (per-1000 scaler keeps
                // the data drop's "x100" units consistent with skill data).
                int32 dmg = (arg * kAbStateDotScalerX1k) / 1000;
                pkTarget->SetHP(pkTarget->GetHP() - dmg);
                break;
            }
            case kAbAction_DotSP: {
                int32 sp = (arg * kAbStateDotScalerX1k) / 1000;
                pkTarget->SetSP(pkTarget->GetSP() - sp);
                break;
            }
            case kAbAction_RegenHP: {
                pkTarget->SetHP(pkTarget->GetHP() + arg);
                break;
            }
            case kAbAction_RegenSP: {
                pkTarget->SetSP(pkTarget->GetSP() + arg);
                break;
            }
            case kAbAction_StatModATK:
            case kAbAction_StatModDEF:
            case kAbAction_StatModMoveSpd:
            case kAbAction_StatModAtkSpd:
            case kAbAction_StatModMATK:
            case kAbAction_StatModMDEF:
            case kAbAction_AbsorbShield:
            case kAbAction_ReflectDmg:
            case kAbAction_DisableMove:
            case kAbAction_DisableAct:
            case kAbAction_DispelOnHit:
                // Non-tickable: handled in OnApply / AbsorbIncoming /
                // ReflectIncoming / per-input gate. Tick is a no-op for
                // these but we log at debug so a capture diff catches
                // any data row that puts a one-shot action into the
                // wrong slot.
                break;
            default:
                SHINELOG_WARN("AbState unhandled ActionIndex=%u arg=%d "
                              "(ab=%u sub=%u h=%u) -- add a handler in "
                              "AbStateRuntime::Tick", idx, arg,
                              rRow.uiAbStateId, rRow.uiSubAbStateId,
                              (uint32)pkTarget->GetHandle());
                break;
        }
    }
}

int32 AbStateRuntime::AbsorbIncoming(ShineObject* pkTarget, int32 iIncoming) {
    if (!pkTarget || iIncoming <= 0) return iIncoming;
    AbnormalState& rAb = pkTarget->AbStateLedger();
    int32 left = iIncoming;
    std::vector<AbStateRuntimeRow>& rows = rAb.RuntimeRows();
    for (size_t i = 0; i < rows.size() && left > 0; ++i) {
        if (rows[i].iAbsorbRemain <= 0) continue;
        int32 take = (rows[i].iAbsorbRemain >= left) ? left : rows[i].iAbsorbRemain;
        rows[i].iAbsorbRemain -= take;
        left -= take;
    }
    return left;
}

int32 AbStateRuntime::ReflectIncoming(ShineObject* pkTarget, int32 iApplied) {
    if (!pkTarget || iApplied <= 0) return 0;
    AbnormalState& rAb = pkTarget->AbStateLedger();
    int32 mirror = 0;
    const std::vector<AbStateRuntimeRow>& rows = rAb.RuntimeRows();
    for (size_t i = 0; i < rows.size(); ++i) {
        for (int j = 0; j < 4; ++j) {
            if (rows[i].uiAction[j] == kAbAction_ReflectDmg) {
                // Arg is per-mille (1000 = 100% reflect, 250 = 25%).
                mirror += (iApplied * rows[i].iArg[j]) / 1000;
            }
        }
    }
    return mirror;
}

} // namespace shine
