// Server/Zone/AbStateRuntime.h
// Per-tick action dispatcher for SubAbState rows. The buff/debuff
// pipeline lives here:
//
//   1. AbnormalState::Apply(ab, ms, strength) gets called from a skill
//      hit, GM cmd, or PineScript.
//   2. We resolve AbState row by id -> InxName -> SubAbState row at the
//      requested Strength tier.
//   3. The 4 ActionIndex / ActionArg slots are pushed into the
//      AbStateInst's per-tick dispatcher (AbStateRuntime::Tick).
//   4. Each ActionIndex calls a documented handler:
//        20 = HP DoT (per-second tick of -ActionArg HP)
//        21 = HP DoT (per-second tick of -ActionArg HP, alt curve)
//        22 = SP/MP DoT
//        30 = Stat-Mod ATK x ActionArg/100
//        31 = Stat-Mod DEF x ActionArg/100
//        32 = Stat-Mod MoveSpeed x ActionArg/100
//        33 = Stat-Mod AttackSpeed x ActionArg/100
//        40 = HP Regen (+ActionArg per second)
//        41 = SP Regen
//        50 = Movement-disable (Stun/Hold)
//        51 = Action-disable (Silence/Sleep)
//        60 = Damage-Reduction shield (absorb up to ActionArg)
//        70 = Reflect-Damage (mirror ActionArg/1000 to attacker)
//        80 = Dispel-on-hit
//      Other indices fall through to a per-target log line so a real
//      capture can surface the missing handler -- they NEVER silently
//      become a no-op.
//
// Tunable scalars are kept on `BattleTunables.h` (kAbState* prefix) so
// re-balancing is a single-file edit.
#ifndef FIESTA_ZONE_ABSTATERUNTIME_H
#define FIESTA_ZONE_ABSTATERUNTIME_H
#include "../Shared/ShineTypes.h"
#include "AbState.h"

namespace fiesta {

class ShineObject;
struct AbStateRow;

// Carries the resolved (AbState row + SubAbState row + per-instance
// expiry) tuple for a single active buff/debuff. Built when Apply()
// resolves the source skill into a concrete tier; ticked by the
// owning ShineObject's per-frame update.
struct AbStateRuntimeRow {
    uint32  uiAbStateId;
    uint32  uiSubAbStateId;
    uint32  uiStrength;
    uint64  uiAppliedMs;
    uint64  uiExpireMs;
    uint64  uiNextTickMs;
    uint32  uiAction[4];
    int32   iArg[4];
    int32   iAbsorbRemain;     // for shield action 60
};

class AbStateRuntime {
public:
    // Resolve a (AbState row, requested strength) pair into a concrete
    // runtime row. Returns false when the SubAbState link is absent or
    // the requested strength is out-of-range and even the floor row is
    // missing -- in that case nothing fires (the caller must NOT push
    // the row into AbnormalState; we deliberately return false instead
    // of a "soft" success).
    static bool Resolve(const AbStateRow& rAb, uint32 uiStrength,
                        AbStateRuntimeRow& rOut);

    // Resolve a SubAbState directly by InxName + Strength. Used by
    // skill / item / map / mount effects whose source data references
    // a SubAbState InxName instead of an AbState id. The runtime row
    // is built without a parent AbState (uiAbStateId = 0), but the
    // dispatcher path is identical -- the 4 ActionIndex slots fire the
    // same handlers as the AbState-rooted path.
    static bool ResolveFromSub(const char* szSubInxName, uint32 uiStrength,
                               AbStateRuntimeRow& rOut);

    // Apply the per-tick effect of one runtime row. Tick cadence is
    // 1000 ms; the caller should pass the same uiNowMs into all rows
    // in a single sweep.
    static void Tick(ShineObject* pkTarget, AbStateRuntimeRow& rRow, uint64 uiNowMs);

    // OnApply: handlers that fire ONCE at apply-time (stat-mods, shield).
    static void OnApply (ShineObject* pkTarget, const AbStateRuntimeRow& rRow);
    // OnRemove: undo any one-shot handlers (stat mods, shield).
    static void OnRemove(ShineObject* pkTarget, const AbStateRuntimeRow& rRow);

    // Damage absorption hook -- called by Battle.cpp before HP loss is
    // applied to the target. Returns the residual damage after every
    // active shield row consumes its remaining capacity.
    static int32 AbsorbIncoming(ShineObject* pkTarget, int32 iIncoming);

    // Reflect-damage hook -- called by Battle.cpp after HP loss is
    // applied. Returns the amount of damage to mirror back to the
    // attacker.
    static int32 ReflectIncoming(ShineObject* pkTarget, int32 iApplied);
};

} // namespace fiesta
#endif
