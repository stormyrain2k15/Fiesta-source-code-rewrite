// Server/Zone/Battle.h
// 12 -- normal/skill attacks, RuleOfEngagement dispatch, damage formula host.
// EVIDENCE: PDB_CONFIRMED  symbol: Battle, BattleObject, RuleOfEngagement, DamageByAngle,
//                                  DamageBySoul, LevelGapDamageTable, AttackRhythm, HitMeList
#ifndef FIESTA_ZONE_BATTLE_H
#define FIESTA_ZONE_BATTLE_H
#include "ShineObject.h"

namespace fiesta {

struct DamageInfo {
    int32  iPhys;
    int32  iMagic;
    bool   bMiss;
    bool   bCrit;
    bool   bBlock;
};

class RuleOfEngagement {
public:
    // EV_VERIFY -- formula constants are local; tune in-place.
    static DamageInfo NormalAttack(ShineObject* pkA, ShineObject* pkT);
    static DamageInfo SkillAttack (ShineObject* pkA, ShineObject* pkT, SkillID sid);
    static DamageInfo DamageByAngle(ShineObject* pkA, ShineObject* pkT, float fAngle);
    static int32      DamageBySoul (int32 iBase, uint16 uiLvA, uint16 uiLvT);
    static int32      LevelGapDamageTable(uint16 uiLvA, uint16 uiLvT);
};

class Battle {
public:
    static void Apply(ShineObject* pkA, ShineObject* pkT, const DamageInfo& d);
    static void Kill (ShineObject* pkA, ShineObject* pkT);
    static void Regen(ShineObject* pk, int32 iHp, int32 iSp);
};

class BattleObject : public ShineObject {};   // PDB symbol parity

} // namespace fiesta
#endif
