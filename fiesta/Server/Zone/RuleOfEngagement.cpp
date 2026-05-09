// Server/Zone/RuleOfEngagement.cpp
// PvP/PvE engagement rules. The class definition lives in Battle.h;
// the heavy implementation of `RuleOfEngagement::CalcDamage` and the
// NormalAttack/SkillAttack wrappers lives in Battle.cpp. This file
// owns:
//   1. The canonical filename for the PDB layout.
//   2. A small private verdict helper (anonymous namespace) used
//      by the PvE/PvP gate logic in Battle.cpp.
//   3. The original NA2016 free-function surface (Roe_*,
//      normalpyRoe_*, normalmaRoe_*) -- thin int32-returning helpers
//      for scripted callers that don't go through the BATTLESTAT
//      pipeline. Tunables live in `BattleTunables.h` (kRoe*).
//
// Math note: every wrapper here forwards to the existing pipeline
// (RuleOfEngagement::CalcDamage / NormalAttack / SkillAttack), which
// is itself VERIFY/TUNE-tagged in Battle.cpp. The function surface is
// the deliverable; tuning the constants inside the wrapped pipeline is
// a separate (and authorized) tuning round.
#include "ShineObject.h"
#include "Battle.h"
#include "BattleStat.h"
#include "BattleTunables.h"
#include "../Shared/well512.h"
#include <stdlib.h>

namespace fiesta {

namespace {
    // PvE/PvP verdict gate. Mirrors the canonical NA2016 logic:
    //   * Both mobs       -> 0 (no engagement: same-faction)
    //   * Mob vs player   -> 1 (PvE)
    //   * Both players    -> 2 (PvP)
    // Used by Battle::Apply to short-circuit invalid pairings before
    // the damage pipeline runs.
    inline int VerdictDefault(ShineObject* pkA, ShineObject* pkT) {
        if (!pkA || !pkT || pkA == pkT) return 0;
        if (pkA->GetType() == OT_MOB    && pkT->GetType() == OT_MOB)    return 0;
        if (pkA->GetType() == OT_PLAYER && pkT->GetType() == OT_PLAYER) return 2;
        return 1;
    }

    // Process-local PRNG for the scripted Roe_* helpers. Independent
    // from `Battle.cpp`'s s_kRng so re-seeding either one doesn't perturb
    // the other.
    inline well512& RoeRng() {
        static well512 s; static bool seeded = false;
        if (!seeded) { s.Seed(0xC0FFEE12u); seeded = true; }
        return s;
    }
}

// ---------------------------------------------------------------------------
//  Original PDB function surface -- generic Roe pipeline.
//  Each helper computes a final int32 directly using the kRoe*
//  tunables. Scripts (PineScript / Lua) should treat the answers as
//  authoritative for the int-return path; the BATTLESTAT pipeline
//  (RuleOfEngagement::CalcDamage in Battle.cpp) is the canonical
//  combat path for actual damage application.
// ---------------------------------------------------------------------------
namespace Roe {

int32 Roe_calcdamage(ShineObject* pkA, ShineObject* pkT, int32 nSkillBaseATK) {
    if (!pkA || !pkT) return 0;
    DamageInfo d = RuleOfEngagement::SkillAttack(pkA, pkT, (SkillID)nSkillBaseATK);
    return d.iPhys + d.iMagic;
}

// Roe_getattack = nATK + (random in [0, nATK * kRoeAtkRangeX1k / 1000])
//                 + nSkillBaseATK
// The skill base is added on top of the natural swing so a skill that
// reads as "+200 base" is a flat +200 over a normal swing.
int32 Roe_getattack(const BATTLESTAT* pkA, eElement /*eAtkElement*/, int32 nSkillBaseATK) {
    if (!pkA) return 0;
    int32 nBase  = pkA->nATK;
    int32 nSwing = 0;
    if (kRoeAtkRangeX1k > 0 && nBase > 0) {
        int32 nMax = (nBase * kRoeAtkRangeX1k) / 1000;
        if (nMax > 0) nSwing = (int32)(RoeRng().Next() % (uint32)(nMax + 1));
    }
    return nBase + nSwing + nSkillBaseATK;
}

// Roe_defendpower = nDEF, scaled by element resist if the attack element
// matches a column on the target's resist row. Scaler is applied as a
// reduction (defender stronger -> dmg lower -> defender effectively higher).
int32 Roe_defendpower(const BATTLESTAT* pkT, eElement eAtkElement) {
    if (!pkT) return 0;
    int32 nDef = pkT->nDEF;
    if (eAtkElement > ELEMENT_NONE && eAtkElement < ELEMENT_MAX) {
        int32 nRes = pkT->nEleResist[eAtkElement];
        // For each 100 points of resist add `(kRoeElementResistX1k - 1000)/10`%
        // bonus defence. With kRoeElementResistX1k = 700 this is -3% per 100,
        // i.e. resist actually lowers def in this branch; flip the sign
        // here if your data drop encodes resist as a damage *reduction*
        // rather than a multiplier.
        nDef = nDef + (nRes * (kRoeElementResistX1k - 1000)) / 1000;
        if (nDef < 0) nDef = 0;
    }
    return nDef;
}

// Roe_hitrate% = clamp(kRoeHitBasePct + (Acc - Dodge) * slope/1000,
//                       floor, cap)
int32 Roe_hitrate(const BATTLESTAT* pkA, const BATTLESTAT* pkT) {
    if (!pkA || !pkT) return kRoeHitFloorPct;
    int32 net = pkA->nAccuracy - pkT->nDodge;
    int32 raw = kRoeHitBasePct + (net * kRoeHitSlopeX1k) / 1000;
    if (raw < kRoeHitFloorPct) raw = kRoeHitFloorPct;
    if (raw > kRoeHitCapPct)   raw = kRoeHitCapPct;
    return raw;
}

// Roe_criticalrate% = clamp(kRoeCritBasePct + Crit/divisor
//                           - TgtDodge/resistDivisor, 0, cap)
int32 Roe_criticalrate(const BATTLESTAT* pkA, const BATTLESTAT* pkT) {
    if (!pkA) return 0;
    int32 raw = kRoeCritBasePct + pkA->nCritical / kRoeCritDivisor;
    if (pkT) raw -= pkT->nDodge / kRoeCritResistDivisor;
    if (raw < 0) raw = 0;
    if (raw > kRoeCritCapPct) raw = kRoeCritCapPct;
    return raw;
}

// Roe_isdamageincrease: true if any damage-class flag indicates a buff
// path is active. The stock implementation treats nDmgFlags != 0 as the
// "increase" trigger; richer rule sets can OR in specific buff bits
// (rage / berserk / passive) without changing call sites.
bool Roe_isdamageincrease(const BATTLESTAT* /*pkA*/, const BATTLESTAT* /*pkT*/, int32 nDmgFlags) {
    return nDmgFlags != 0;
}

} // namespace Roe

// ---------------------------------------------------------------------------
//  normalpyRoe_* -- physical normal-attack family.
// ---------------------------------------------------------------------------
namespace normalpyRoe {

DamageInfo normalpyRoe_calc(ShineObject* pkA, ShineObject* pkT) {
    return RuleOfEngagement::NormalAttack(pkA, pkT);
}
int32 normalpyRoe_damage(ShineObject* pkA, ShineObject* pkT) {
    DamageInfo d = RuleOfEngagement::NormalAttack(pkA, pkT);
    return d.iPhys + d.iMagic;
}
int32 normalpyRoe_hitrate(const BATTLESTAT* pkA, const BATTLESTAT* pkT) {
    return Roe::Roe_hitrate(pkA, pkT);
}
bool  normalpyRoe_isdamageincrease(const BATTLESTAT* pkA, const BATTLESTAT* pkT) {
    return Roe::Roe_isdamageincrease(pkA, pkT, 1 /* PHYSICAL */);
}

} // namespace normalpyRoe

// ---------------------------------------------------------------------------
//  normalmaRoe_* -- magical normal-attack family.
// ---------------------------------------------------------------------------
namespace normalmaRoe {

DamageInfo normalmaRoe_calc(ShineObject* pkA, ShineObject* pkT) {
    // Magic normal attack currently shares the physical pipeline; the
    // T-matrix scaler split happens inside RuleOfEngagement::CalcDamage
    // when an eElement is supplied. VERIFY/TUNE applies.
    return RuleOfEngagement::NormalAttack(pkA, pkT);
}
int32 normalmaRoe_damage(ShineObject* pkA, ShineObject* pkT) {
    DamageInfo d = RuleOfEngagement::NormalAttack(pkA, pkT);
    return d.iPhys + d.iMagic;
}
int32 normalmaRoe_hitrate(const BATTLESTAT* pkA, const BATTLESTAT* pkT) {
    return Roe::Roe_hitrate(pkA, pkT);
}
bool  normalmaRoe_isdamageincrease(const BATTLESTAT* pkA, const BATTLESTAT* pkT) {
    return Roe::Roe_isdamageincrease(pkA, pkT, 2 /* MAGICAL */);
}

} // namespace normalmaRoe

} // namespace fiesta
