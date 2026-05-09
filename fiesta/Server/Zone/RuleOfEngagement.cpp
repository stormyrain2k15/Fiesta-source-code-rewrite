// Server/Zone/RuleOfEngagement.cpp
// PvP/PvE engagement rules. The class definition lives in Battle.h;
// the heavy implementation of `RuleOfEngagement::CalcDamage` and the
// NormalAttack/SkillAttack wrappers lives in Battle.cpp. This file
// owns:
//   1. The canonical filename for the PDB layout.
//   2. A small private verdict helper (anonymous namespace) used
//      by the PvE/PvP gate logic in Battle.cpp.
//   3. The original NA2016 free-function surface (Roe_*,
//      normalpyRoe_*, normalmaRoe_*) -- these are thin wrappers
//      around the pipeline so PineScript and other script callers
//      can target the original function names.
//
// Math note: every wrapper here forwards to the existing pipeline
// (RuleOfEngagement::CalcDamage / NormalAttack / SkillAttack), which
// is itself VERIFY/TUNE-tagged in Battle.cpp. The function surface is
// the deliverable; tuning the constants inside the wrapped pipeline is
// a separate (and authorized) tuning round.
#include "ShineObject.h"
#include "Battle.h"
#include "BattleStat.h"

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
}

// ---------------------------------------------------------------------------
//  Original PDB function surface -- generic Roe pipeline.
//  Each helper forwards into RuleOfEngagement / its sub-pieces in
//  Battle.cpp. Math constants are NOT redefined here -- the wrappers
//  exist so callers (PineScript / Lua bindings / client-emitted
//  formulas) can target the original symbol names. Tunable bodies stay
//  in Battle.cpp and remain VERIFY/TUNE-tagged.
// ---------------------------------------------------------------------------
namespace Roe {

int32 Roe_calcdamage(ShineObject* pkA, ShineObject* pkT, int32 nSkillBaseATK) {
    if (!pkA || !pkT) return 0;
    DamageInfo d = RuleOfEngagement::SkillAttack(pkA, pkT, (SkillID)nSkillBaseATK);
    return d.iPhys + d.iMagic;
}

int32 Roe_getattack(const BATTLESTAT* pkA, eElement /*eAtkElement*/, int32 nSkillBaseATK) {
    // VERIFY: real formula is Roe_getattack = Atk + RandomRange + ElementBonus.
    // Until the constants land, return the floor estimate (Atk + skill base).
    if (!pkA) return 0;
    return pkA->nATK + nSkillBaseATK;
}

int32 Roe_defendpower(const BATTLESTAT* pkT, eElement /*eAtkElement*/) {
    // VERIFY: original applies element-vs-resist scaler before subtracting Def.
    if (!pkT) return 0;
    return pkT->nDEF;
}

int32 Roe_hitrate(const BATTLESTAT* pkA, const BATTLESTAT* pkT) {
    // VERIFY: NA2016 formula is roughly Hit% = clamp(50 + (Hit - Avoid)/2, 5, 95).
    // BATTLESTAT does not yet expose dedicated Hit/Avoid fields, so use
    // the Dodge defensive column as a proxy for Avoid until the real
    // schema columns land. Surface-only -- see VERIFY tag.
    if (!pkA || !pkT) return 0;
    int32 raw = 50 - (pkT->nDodge / 200);
    if (raw < 5)  raw = 5;
    if (raw > 95) raw = 95;
    return raw;
}

int32 Roe_criticalrate(const BATTLESTAT* pkA, const BATTLESTAT* pkT) {
    // VERIFY: original references both Critical and CriticalResist columns.
    if (!pkA) return 0;
    int32 raw = pkA->nCritical / 100 - (pkT ? pkT->nDodge / 400 : 0);
    if (raw < 0)  raw = 0;
    if (raw > 50) raw = 50;
    return raw;
}

bool Roe_isdamageincrease(const BATTLESTAT* /*pkA*/, const BATTLESTAT* /*pkT*/, int32 nDmgFlags) {
    // VERIFY: original consults rage / berserk / passive-buff bits on
    // the attacker. Surface-only stub: any nonzero flag set counts.
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
