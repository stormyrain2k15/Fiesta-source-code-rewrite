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
