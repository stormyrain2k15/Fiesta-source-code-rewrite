// Server/Zone/Battle.cpp
// the damage pipeline, written against the BATTLESTAT model.
// All numeric tunables live in BattleTunables.h; level-gap and mob-resist
// lookups live in LevelGapTable / MobResistTable.
#include "Battle.h"
#include "BattleStat.h"
#include "BattleTunables.h"
#include "LevelGapTable.h"
#include "MobResistTable.h"
#include "MobSpawnSystem.h"
#include "WorldTables.h"
#include "GroupTables.h"
#include "ExtendedTables.h"
#include "ItemSystems.h"
#include "QuestSystem.h"
#include "GameLogClient.h"
#include "LiveOpsBoosts.h"
#include "AbStateRuntime.h"
#include "../Shared/well512.h"
#include "../Shared/ShineLogSystem.h"

namespace shine {

static well512 s_kRng;

// Choose the offensive value to use given dmg flags + skill base override.
static int32 PickATK(const BATTLESTAT* a, int32 nFlags, int32 nSkillBase) {
    if (nSkillBase > 0) return nSkillBase;
    if (nFlags & DMG_TYPE_MAGIC)  return a->nMATK;
    if (nFlags & DMG_TYPE_RANGED) return a->nRATK ? a->nRATK : a->nATK;
    return a->nATK;
}
static int32 PickDEF(const BATTLESTAT* t, int32 nFlags) {
    if (nFlags & DMG_TYPE_TRUE)   return 0;
    if (nFlags & DMG_TYPE_MAGIC)  return t->nMDEF;
    if (nFlags & DMG_TYPE_RANGED) return t->nRDEF ? t->nRDEF : t->nDEF;
    return t->nDEF;
}

// =============================================================================
// Full pipeline.
// =============================================================================
void RuleOfEngagement::CalcDamage(DAMAGERESULT*    pOut,
                                  const BATTLESTAT* a, eElement eAtkElement,
                                  const BATTLESTAT* t, uint16    uiTgtMobID,
                                  int32 nDmgFlags,
                                  int32 nSkillBaseATK,
                                  float fAngleRad,
                                  bool  bAtkIsPlayer,
                                  bool  bTgtIsPlayer) {
    if (!pOut) return;
    pOut->Clear();
    pOut->eElement = eAtkElement;
    if (!a || !t) { pOut->bMissed = true; return; }

    // 1. Hit roll.
    {
        int32 hit   = a->nAccuracy;
        int32 dodge = t->nDodge;
        int32 chance = 9000 + hit - dodge;          // 90% baseline
        if (chance < 500) chance = 500;             // floor 5%
        if (chance > 9900) chance = 9900;           // cap 99%
        int32 roll = (int32)(s_kRng.NextDouble() * 10000.0);
        if (roll > chance) { pOut->bMissed = true; pOut->bDodged = true; return; }
    }

    // 2. Raw damage formula. Formula selected by kRawDmgMode in BattleTunables.h.
    int32 atk = PickATK(a, nDmgFlags, nSkillBaseATK);
    int32 def = PickDEF(t, nDmgFlags);
    int64 dmg;
    if (kRawDmgMode == 0) {
        // Quadratic (Shine-style): (ATK+bias)^2 / (ATK+DEF+2*bias)
        // At equal ATK/DEF: ~50% of ATK. Higher ATK advantage = better scaling.
        // TUNE: kQuadraticBias, kRawDmgScalerX1k in BattleTunables.h.
        int64 a2 = (int64)(atk + kQuadraticBias);
        int64 d2 = (int64)(atk + def + kQuadraticBias * 2);
        if (d2 < 1) d2 = 1;
        dmg = (a2 * a2 * (int64)kRawDmgScalerX1k) / (d2 * 1000LL);
    } else {
        // Linear fallback: ATK - DEF, capped so DEF reduces at most kLinearDefCapX1k/1000 of ATK.
        int32 defCap = (atk * kLinearDefCapX1k) / 1000;
        int32 effDef = (def < defCap) ? def : defCap;
        dmg = (int64)(atk - effDef);
    }
    if (dmg < (int64)kDamageFloor) dmg = (int64)kDamageFloor;

    // 3. Level-gap (x1000).
    {
        eLvGapType ty = LVGAP_PVE;
        if (bAtkIsPlayer && bTgtIsPlayer)   ty = LVGAP_PVP;
        else if (!bAtkIsPlayer && bTgtIsPlayer) ty = LVGAP_EVP;
        int32 mul = LevelGapTable::Get().GetMultiplier(a->nLevel, t->nLevel, ty);
        dmg = (dmg * mul) / 1000;
    }

    // 4. Element.
    if (eAtkElement > ELEMENT_NONE && eAtkElement < ELEMENT_MAX) {
        int32 ele = a->nEleATK[eAtkElement];
        int32 res = t->nEleResist[eAtkElement];
        // Add MobResist row if target is a mob.
        if (!bTgtIsPlayer)
            res += MobResistTable::Get().GetElementResist(uiTgtMobID, eAtkElement);
        // Each net 100 ele = +100% damage; each 100 resist = -50% damage (clamp 0).
        int32 net = ele - res;
        // x10000 fixed-point multiplier; baseline 10000 = 1.0x.
        int32 m = 10000 + net * 100;
        if (m < 2000) m = 2000;     // floor 0.20x
        dmg = (dmg * m) / 10000;
    }

    // 5. Angle.
    if (fAngleRad > 1.5707963f) {
        dmg = (dmg * kBackHitMultiplierX1k) / 1000;
    } else if (fAngleRad > 0.7853981f) {
        dmg = (dmg * kFlankHitMultiplierX1k) / 1000;
    }

    // 6. Critical.
    {
        int32 critRoll = (int32)(s_kRng.NextDouble() * (double)kCritRollMax);
        if (critRoll < a->nCritical) {
            pOut->bCritical = true;
            int32 m = kCritDamageScaler + a->nCriticalDmg;
            dmg = (dmg * m) / 10000;
            pOut->nDamageType |= DMG_TYPE_CRITICAL;
        }
    }

    // 7. Block (melee, non-critical only).
    if (!pOut->bCritical && !(nDmgFlags & DMG_TYPE_RANGED) && !(nDmgFlags & DMG_TYPE_MAGIC)) {
        int32 blockRoll = (int32)(s_kRng.NextDouble() * 10000.0);
        if (blockRoll < t->nBlock) {
            pOut->bBlocked = true;
            int32 reduce = t->nBlockDef ? t->nBlockDef : 5000;   // default 50%
            dmg = (dmg * (10000 - reduce)) / 10000;
        }
    }

    // 8. Damage-type resist (mob phys/magic/ranged columns).
    if (!bTgtIsPlayer) {
        int32 r = MobResistTable::Get().GetDamageTypeResist(uiTgtMobID, nDmgFlags);
        if (r > 0) {
            if (r >= 100) r = 95;       // never full immune
            dmg = (dmg * (100 - r)) / 100;
        }
    }

    // 9. Variance.
    if (kDamageVarianceX10k > 0) {
        int32 vmax = kDamageVarianceX10k;
        int32 v = (int32)(s_kRng.NextDouble() * (double)(vmax * 2)) - vmax;
        dmg = (dmg * (10000 + v)) / 10000;
    }

    // 10. Absorb shield.
    if (t->nAbsorbDmg > 0) {
        int32 absorbed = t->nAbsorbDmg;
        if ((int64)absorbed > dmg) absorbed = (int32)dmg;
        dmg -= absorbed;
        pOut->nAbsorbed = absorbed;
    }

    // 11. PvP scaler.
    if (bAtkIsPlayer && bTgtIsPlayer) {
        dmg = (dmg * kPvPDamageScalerX1k) / 1000;
    }

    // 12. Floor.
    if (dmg < (int64)kDamageFloor) dmg = kDamageFloor;
    pOut->nDamage     = (int32)dmg;
    pOut->nDamageType |= nDmgFlags;
}

// -----------------------------------------------------------------------------
// Backwards-compat helpers (used by existing callers that still hold
// raw ShineObject pointers and don't yet have a BATTLESTAT).
// -----------------------------------------------------------------------------
static void StubStat(BATTLESTAT& s, ShineObject* pk) {
    s.Clear();
    if (!pk) return;
    s.nLevel = (pk->GetType() == OT_PLAYER) ? ((ShinePlayer*)pk)->GetLevel() : 1;
    s.nATK   = 5  + s.nLevel * 4;
    s.nDEF   = 2  + s.nLevel * 3;
    s.nMATK  = 3  + s.nLevel * 4;
    s.nMDEF  = 2  + s.nLevel * 3;
    s.nMaxHP = 100 + s.nLevel * 50;
    s.nAccuracy = 0;
    s.nDodge    = 0;
    s.nCritical = 1000;     // 10% baseline
}

DamageInfo RuleOfEngagement::NormalAttack(ShineObject* pkA, ShineObject* pkT) {
    DamageInfo d; d.iPhys = 0; d.iMagic = 0; d.bMiss = false; d.bCrit = false; d.bBlock = false;
    BATTLESTAT a, t; StubStat(a, pkA); StubStat(t, pkT);
    DAMAGERESULT r;
    bool aPC = pkA && pkA->GetType() == OT_PLAYER;
    bool tPC = pkT && pkT->GetType() == OT_PLAYER;
    CalcDamage(&r, &a, ELEMENT_NONE, &t, 0, DMG_TYPE_NORMAL, 0, 0.0f, aPC, tPC);
    d.iPhys = r.nDamage; d.bMiss = r.bMissed; d.bCrit = r.bCritical; d.bBlock = r.bBlocked;
    return d;
}

DamageInfo RuleOfEngagement::SkillAttack(ShineObject* pkA, ShineObject* pkT, SkillID) {
    DamageInfo d = NormalAttack(pkA, pkT);
    if (!d.bMiss) d.iMagic = (int32)((float)d.iPhys * 0.85f);
    return d;
}

DamageInfo RuleOfEngagement::DamageByAngle(ShineObject* pkA, ShineObject* pkT, float fAngle) {
    DamageInfo d; d.iPhys = 0; d.iMagic = 0; d.bMiss = false; d.bCrit = false; d.bBlock = false;
    BATTLESTAT a, t; StubStat(a, pkA); StubStat(t, pkT);
    DAMAGERESULT r;
    bool aPC = pkA && pkA->GetType() == OT_PLAYER;
    bool tPC = pkT && pkT->GetType() == OT_PLAYER;
    CalcDamage(&r, &a, ELEMENT_NONE, &t, 0, DMG_TYPE_NORMAL, 0, fAngle, aPC, tPC);
    d.iPhys = r.nDamage; d.bMiss = r.bMissed; d.bCrit = r.bCritical; d.bBlock = r.bBlocked;
    return d;
}

int32 RuleOfEngagement::DamageBySoul(int32 iBase, uint16 uiLvA, uint16 uiLvT) {
    int32 mul = LevelGapTable::Get().GetMultiplier((int32)uiLvA, (int32)uiLvT, LVGAP_PVE);
    return (iBase * mul) / 1000;
}

int32 RuleOfEngagement::LevelGapDamageTable(uint16 uiLvA, uint16 uiLvT) {
    return LevelGapTable::Get().GetMultiplier((int32)uiLvA, (int32)uiLvT, LVGAP_PVE);
}

// -----------------------------------------------------------------------------
// Battle (mutators).
// -----------------------------------------------------------------------------
void Battle::Apply(ShineObject* pkA, ShineObject* pkT, const DamageInfo& d) {
    if (!pkT || d.bMiss) return;
    int32 total = d.iPhys + d.iMagic;
    // Run incoming damage through any active absorb-shield buffs on
    // the target (AbStateRuntime action 60). The shield rows decrement
    // their remaining capacity in place; total here is the residual.
    total = AbStateRuntime::AbsorbIncoming(pkT, total);
    pkT->SetHP(pkT->GetHP() - total);
    // Reflect-damage hook (action 70). Mirror a fraction back to the
    // attacker if it's still alive and not the same object as target.
    int32 reflect = AbStateRuntime::ReflectIncoming(pkT, total);
    if (reflect > 0 && pkA && pkA != pkT && !pkA->IsDead())
        pkA->SetHP(pkA->GetHP() - reflect);
    if (pkA && pkT->GetType() == OT_MOB && pkA->GetType() == OT_PLAYER) {
        ShineMob* pkM = (ShineMob*)pkT;
        ShinePlayer* pkP = (ShinePlayer*)pkA;
        // 20-level-span scaler: a much higher-level player pulls less aggro
        // than a same/lower-level player dealing the same damage.
        pkM->m_kAggro.AddScaled(pkP->GetCharID(),
                                total > 0 ? total : 1,
                                pkP->GetLevel(), pkM->m_uiLevel);
        // MobChat[DAMAGED] -- per-species response line, sampled by Rate.
        const MobInfoRow* pkInfo = MobTables::Get().Find((uint32)pkM->m_uiSpecies);
        if (pkInfo) {
            std::string line = MobChatTable::Get().Pick(
                MobChatTable::MC_DAMAGED, pkInfo->kInxName);
            if (!line.empty())
                SHINELOG_DEBUG("MobChat[DAMAGED] %s: %s",
                               pkInfo->kInxName.c_str(), line.c_str());
        }
    }
    if (pkT->IsDead()) Kill(pkA, pkT);
}

void Battle::Apply(ShineObject* pkA, ShineObject* pkT, const DAMAGERESULT& r) {
    if (!pkT || r.bMissed) return;
    int32 net = AbStateRuntime::AbsorbIncoming(pkT, r.nDamage);
    pkT->SetHP(pkT->GetHP() - net);
    int32 reflect = AbStateRuntime::ReflectIncoming(pkT, net);
    if (reflect > 0 && pkA && pkA != pkT && !pkA->IsDead())
        pkA->SetHP(pkA->GetHP() - reflect);
    if (pkA && pkT->GetType() == OT_MOB && pkA->GetType() == OT_PLAYER) {
        ShineMob* pkM = (ShineMob*)pkT;
        ShinePlayer* pkP = (ShinePlayer*)pkA;
        pkM->m_kAggro.AddScaled(pkP->GetCharID(),
                                net > 0 ? net : 1,
                                pkP->GetLevel(), pkM->m_uiLevel);
        const MobInfoRow* pkInfo = MobTables::Get().Find((uint32)pkM->m_uiSpecies);
        if (pkInfo) {
            std::string line = MobChatTable::Get().Pick(
                MobChatTable::MC_DAMAGED, pkInfo->kInxName);
            if (!line.empty())
                SHINELOG_DEBUG("MobChat[DAMAGED] %s: %s",
                               pkInfo->kInxName.c_str(), line.c_str());
        }
    }
    if (pkT->IsDead()) Kill(pkA, pkT);
}

void Battle::Kill(ShineObject* pkA, ShineObject* pkT) {
    if (!pkT) return;
    SHINELOG_INFO("Battle::Kill h=%u by h=%u", pkT->GetHandle(), pkA ? pkA->GetHandle() : 0);

    // Mob death: spawn slot frees, killer (if a player) gets EXP, drops,
    // quest credit, kill-counter title bump. Death-emote chat for nearby
    // players via MobChatTable.
    if (pkT->GetType() == OT_MOB) {
        MobSpawnSystem::Get().OnMobDied(pkT->GetHandle());
        ShineMob* pkMob = (ShineMob*)pkT;
        ShinePlayer* pkKiller = NULL;
        if (pkA && pkA->GetType() == OT_PLAYER) pkKiller = (ShinePlayer*)pkA;

        // Look up MobInfo for EXP and species name.
        const MobInfoRow* pkInfo =
            MobTables::Get().Find((uint32)pkMob->m_uiSpecies);

        if (pkKiller) {
            // 1) EXP: base value scaled by ExpRecalc (level-diff and party
            //    factors). The party split / leech rules live in
            //    Party::DistributeExp -- this path is solo.
            uint64 base = pkInfo ? (uint64)pkInfo->uiExp : 0;
            int32  scaler = ExpRecalcTable::Get().Scaler(
                                pkKiller->GetLevel(), pkMob->m_uiLevel);
            uint64 grant = (base * (uint64)scaler) / 100ULL;
            // Live-ops EXP boost (Lucky Hour / Double EXP): ratio is
            // x1000 with 1000 == stock. Skipped when scaler is stock so
            // we don't pay the divide on the hot path.
            const int32 expBoost = LiveOpsBoosts::Get().ExpRateX1k();
            if (expBoost != 1000) {
                grant = (grant * (uint64)expBoost) / 1000ULL;
            }
            pkKiller->AddExp(grant);

            // 1b) Money: base from MobInfo.shn (Money column); zero is a
            // valid "no gold" outcome. Live-ops Money boost (Golden
            // Hour) stacks on top of the base and is applied last so
            // 0 base remains 0 after boost.
            //
            // VERIFY: `MobInfoRow.uiMoney` is read from a "Money" column
            // in MobInfo.shn (see GroupTables.cpp parse). I have not
            // confirmed that column actually exists in the NA2016
            // MobInfo.shn header -- ShnGetU32 returns 0 for a missing
            // column, so the visible symptom of a wrong column name is
            // "mobs never drop gold". Reconcile against the SHN dump.
            if (pkInfo && pkInfo->uiMoney > 0) {
                int64 gold = (int64)pkInfo->uiMoney;
                const int32 moneyBoost = LiveOpsBoosts::Get().MoneyRateX1k();
                if (moneyBoost != 1000) {
                    gold = (gold * (int64)moneyBoost) / 1000LL;
                }
                pkKiller->AddMoney(gold);
            }

            // 2) Drops from data-driven DropResolver.
            ItemDropFromMob::Trigger(pkMob, pkKiller);

            // 3) Quest credit: kill-target requirement.
            QuestEventDispatcher::Get().OnMobKilled(pkKiller->GetCharID(),
                                             pkMob->m_uiSpecies);

            // 4) Friend point: aggregate from FriendPointReward when the
            //    bonus-kill threshold matches (the bookkeeping table
            //    handles dedupe).
            const FriendPointRewardTable::Row* fr =
                FriendPointRewardTable::Get().Find(1);
            (void)fr;

            // 5) Append the kill to World00_GameLog. The GameLog exe pipes
            //    the row through SQLP_GameLog::LogKill -> p_Log_Combat.
            GameLogClient::Get().LogKill(
                pkKiller->GetCharID(),
                (MobID)pkMob->m_uiSpecies,
                (uint16)pkKiller->GetMapID());
        }

        // 5) MobChat death emote: pick a "DEAD" line and broadcast to
        //    nearby players (visibility radius is owned by the Field).
        if (pkInfo) {
            std::string line = MobChatTable::Get().Pick(
                MobChatTable::MC_DEAD, pkInfo->kInxName);
            if (!line.empty())
                SHINELOG_DEBUG("MobChat[DEAD] %s: %s",
                               pkInfo->kInxName.c_str(), line.c_str());
        }

        // 6) Kill-announce (if the mob is in MobKillAnnounce.shn).
        if (pkInfo) {
            const MobExtraTables::AnnounceRow* an =
                MobExtraTables::Get().FindAnnounce(pkInfo->kInxName);
            if (an && pkKiller)
                SHINELOG_INFO("MobKillAnnounce: %s killed %s",
                              pkKiller->GetName().c_str(),
                              pkInfo->kInxName.c_str());
        }

        // 7) Aggro list cleanup.
        pkMob->m_kAggro.Clear();
        return;
    }

    // Player death: EXP loss per ExpRecalc, return-to-town queued via the
    // recall coord.
    if (pkT->GetType() == OT_PLAYER) {
        ShinePlayer* pkP = (ShinePlayer*)pkT;
        uint64 cur = pkP->GetExp();
        // Flat 1% on death; the original game scales by level via
        // ChrCommonTable.ExpLossPct -- consult once that column is wired.
        uint64 loss = cur / 100ULL;
        if (loss > cur) loss = cur;
        pkP->AddExp((uint64)0 - loss);     // SetExp not exposed; AddExp wraps
    }
}

void Battle::Regen(ShineObject* pk, int32 iHp, int32 iSp) {
    if (!pk) return;
    pk->SetHP(pk->GetHP() + iHp);
    pk->SetSP(pk->GetSP() + iSp);
}

} // namespace shine
