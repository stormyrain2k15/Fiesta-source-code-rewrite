// Server/Zone/Battle.cpp
// Per spec rule 04 / 08, formula constants are local and editable in-place.
#include "Battle.h"
#include "../Shared/well512.h"
#include "../Shared/ShineLogSystem.h"

namespace fiesta {

// Local provisional damage constants (EV_VERIFY).
static const float kPhysBaseScale   = 1.20f;
static const float kCritMul         = 1.75f;
static const int32 kBaseAtkPerLevel = 4;
static const int32 kBaseDefPerLevel = 3;
static const float kHitChance       = 0.92f;
static const float kCritChance      = 0.10f;
static const float kBlockChance     = 0.05f;

static well512 s_kRng;

static int32 LevelStat(uint16 uiLv, int32 perLv) { return (int32)uiLv * perLv; }

DamageInfo RuleOfEngagement::NormalAttack(ShineObject* pkA, ShineObject* pkT) {
    DamageInfo d; d.iPhys = 0; d.iMagic = 0; d.bMiss = false; d.bCrit = false; d.bBlock = false;
    if (!pkA || !pkT) { d.bMiss = true; return d; }
    uint16 uiLvA = 1, uiLvT = 1;
    ShinePlayer* pa = (pkA->GetType() == OT_PLAYER) ? (ShinePlayer*)pkA : NULL;
    ShinePlayer* pt = (pkT->GetType() == OT_PLAYER) ? (ShinePlayer*)pkT : NULL;
    if (pa) uiLvA = pa->GetLevel();
    if (pt) uiLvT = pt->GetLevel();
    int32 atk = LevelStat(uiLvA, kBaseAtkPerLevel);
    int32 def = LevelStat(uiLvT, kBaseDefPerLevel);
    if (s_kRng.NextDouble() > kHitChance) { d.bMiss = true; return d; }
    if (s_kRng.NextDouble() < kBlockChance) d.bBlock = true;
    int32 raw = (int32)(((float)atk - (float)def) * kPhysBaseScale);
    if (raw < 1) raw = 1;
    if (s_kRng.NextDouble() < kCritChance) { raw = (int32)((float)raw * kCritMul); d.bCrit = true; }
    if (d.bBlock) raw /= 2;
    d.iPhys = raw + LevelGapDamageTable(uiLvA, uiLvT);
    if (d.iPhys < 1) d.iPhys = 1;
    return d;
}

DamageInfo RuleOfEngagement::SkillAttack(ShineObject* pkA, ShineObject* pkT, SkillID) {
    DamageInfo d = NormalAttack(pkA, pkT);
    // Local provisional skill multiplier (EV_VERIFY) -- replace with table lookup later.
    if (!d.bMiss) d.iMagic = (int32)((float)d.iPhys * 0.85f);
    return d;
}

DamageInfo RuleOfEngagement::DamageByAngle(ShineObject* pkA, ShineObject* pkT, float fAngle) {
    DamageInfo d = NormalAttack(pkA, pkT);
    if (!d.bMiss) {
        float k = 1.0f;
        if (fAngle > 1.57f)      k = 1.20f; // back hit
        else if (fAngle > 0.78f) k = 1.10f;
        d.iPhys = (int32)((float)d.iPhys * k);
    }
    return d;
}

int32 RuleOfEngagement::DamageBySoul(int32 iBase, uint16 uiLvA, uint16 uiLvT) {
    return iBase + LevelGapDamageTable(uiLvA, uiLvT);
}

int32 RuleOfEngagement::LevelGapDamageTable(uint16 uiLvA, uint16 uiLvT) {
    int32 gap = (int32)uiLvA - (int32)uiLvT;
    if (gap >= 0) {
        if (gap > 10) gap = 10;
        return gap * 2;
    } else {
        if (gap < -10) gap = -10;
        return gap * 3;
    }
}

void Battle::Apply(ShineObject* pkA, ShineObject* pkT, const DamageInfo& d) {
    if (!pkT || d.bMiss) return;
    int32 total = d.iPhys + d.iMagic;
    pkT->SetHP(pkT->GetHP() - total);
    if (pkT->IsDead()) Kill(pkA, pkT);
}

void Battle::Kill(ShineObject* pkA, ShineObject* pkT) {
    if (!pkT) return;
    SHINELOG_INFO("Battle::Kill h=%u by h=%u", pkT->GetHandle(), pkA ? pkA->GetHandle() : 0);
    // Drops / EXP / quest credit fan out from here in pass 2.
}

void Battle::Regen(ShineObject* pk, int32 iHp, int32 iSp) {
    if (!pk) return;
    pk->SetHP(pk->GetHP() + iHp);
    pk->SetSP(pk->GetSP() + iSp);
}

} // namespace fiesta
