// Server/Zone/Battle.h
// normal/skill attacks, RuleOfEngagement dispatch, damage pipeline host.
//                                   DamageByAngle, DamageBySoul,
//                                   LevelGapDamageTable, CalcDamage.
#ifndef FIESTA_ZONE_BATTLE_H
#define FIESTA_ZONE_BATTLE_H
#include "ShineObject.h"
#include "BattleStat.h"

namespace fiesta {

// Backwards-compatible thin damage-summary type used by older call sites.
struct DamageInfo {
    int32  iPhys;
    int32  iMagic;
    bool   bMiss;
    bool   bCrit;
    bool   bBlock;
};

// ----------------------------------------------------------------------------
// RuleOfEngagement -- the damage pipeline.
// Stages (each stage may be skipped via flags / data):
//   1. Hit roll      (accuracy vs dodge)
//   2. Block roll    (target block stat, melee only)
//   3. Raw value     (ATK or MATK or RATK - target's matching DEF)
//   4. Level-gap     (table lookup, multiplier x1000)
//   5. Element       (attacker EleATK[e] vs target EleResist[e])
//   6. Angle         (back/flank multiplier)
//   7. Critical      (crit roll, crit dmg multiplier)
//   8. Block apply   (if blocked, multiply by (100 - BlockDef)%)
//   9. Damage type   (ranged / magic resists from MobResist)
//  10. Variance      (+/- kDamageVarianceX10k)
//  11. Absorb        (target absorb shield)
//  12. PvP scaler    (if both are players)
//  13. Floor to kDamageFloor.
// ----------------------------------------------------------------------------
class RuleOfEngagement {
public:
    // Full pipeline. eFlags is a bitmask of eDamageType.
    static void  CalcDamage(DAMAGERESULT*    pOut,
                            const BATTLESTAT* pkAtk, eElement eAtkElement,
                            const BATTLESTAT* pkTgt, uint16   uiTgtMobID,
                            int32 nDmgFlags,
                            int32 nSkillBaseATK,
                            float fAngleRad,
                            bool  bAtkIsPlayer,
                            bool  bTgtIsPlayer);

    // Convenience wrappers used by older call sites (still valid).
    static DamageInfo NormalAttack (ShineObject* pkA, ShineObject* pkT);
    static DamageInfo SkillAttack  (ShineObject* pkA, ShineObject* pkT, SkillID sid);
    static DamageInfo DamageByAngle(ShineObject* pkA, ShineObject* pkT, float fAngle);
    static int32      DamageBySoul (int32 iBase, uint16 uiLvA, uint16 uiLvT);
    static int32      LevelGapDamageTable(uint16 uiLvA, uint16 uiLvT);
};

// ----------------------------------------------------------------------------
//  Original NA2016 PDB function surface.
//
//  These free-function wrappers preserve the original-server function
//  shape (Roe_*, normalpyRoe_*, normalmaRoe_*) that PineScript and
//  client-emitted scripts expect. They forward into the current
//  RuleOfEngagement pipeline where possible. The math inside is still
//  VERIFY/TUNE-tagged -- the goal of these wrappers is the function
//  surface, not the formula. Tunable details live in the wrapped
//  implementations (Battle.cpp).
//
//  Calling convention: every helper takes the same battle stats /
//  ShineObject pointers the canonical handlers use; outputs are plain
//  ints / DamageInfo so PineScript bindings can grab them without
//  pulling Battle.h into the script TU.
// ----------------------------------------------------------------------------
namespace Roe {
    // Generic damage / hit / crit pipeline (mirrors the PDB Roe_*
    // free-function surface). Internally calls RuleOfEngagement.
    int32      Roe_calcdamage      (ShineObject* pkA, ShineObject* pkT, int32 nSkillBaseATK);
    int32      Roe_getattack       (const BATTLESTAT* pkA, eElement eAtkElement, int32 nSkillBaseATK);
    int32      Roe_defendpower     (const BATTLESTAT* pkT, eElement eAtkElement);
    int32      Roe_hitrate         (const BATTLESTAT* pkA, const BATTLESTAT* pkT);
    int32      Roe_criticalrate    (const BATTLESTAT* pkA, const BATTLESTAT* pkT);
    bool       Roe_isdamageincrease(const BATTLESTAT* pkA, const BATTLESTAT* pkT, int32 nDmgFlags);
}

namespace normalpyRoe {
    // Physical (PY) normal-attack rules. PDB function family
    // `normalpyRoe_*`. Used by PvP normal attacks and physical-class
    // skills that bypass the elemental table.
    DamageInfo normalpyRoe_calc        (ShineObject* pkA, ShineObject* pkT);
    int32      normalpyRoe_damage      (ShineObject* pkA, ShineObject* pkT);
    int32      normalpyRoe_hitrate     (const BATTLESTAT* pkA, const BATTLESTAT* pkT);
    bool       normalpyRoe_isdamageincrease(const BATTLESTAT* pkA, const BATTLESTAT* pkT);
}

namespace normalmaRoe {
    // Magical (MA) normal-attack rules. PDB function family
    // `normalmaRoe_*`. Used by mage normal attacks and magic-class
    // skills before the SkillDataBox T-matrix scaler.
    DamageInfo normalmaRoe_calc        (ShineObject* pkA, ShineObject* pkT);
    int32      normalmaRoe_damage      (ShineObject* pkA, ShineObject* pkT);
    int32      normalmaRoe_hitrate     (const BATTLESTAT* pkA, const BATTLESTAT* pkT);
    bool       normalmaRoe_isdamageincrease(const BATTLESTAT* pkA, const BATTLESTAT* pkT);
}

class Battle {
public:
    static void Apply(ShineObject* pkA, ShineObject* pkT, const DamageInfo&    d);
    static void Apply(ShineObject* pkA, ShineObject* pkT, const DAMAGERESULT&  r);
    static void Kill (ShineObject* pkA, ShineObject* pkT);
    static void Regen(ShineObject* pk, int32 iHp, int32 iSp);
};

class BattleObject : public ShineObject {};   // PDB symbol parity

} // namespace fiesta
#endif
