// Server/Zone/MobAIRunner.cpp
#include "MobAIRunner.h"
#include "MobAISystem.h"
#include "MobAttackSequence.h"
#include "MobBehaviorScript.h"
#include "WorldTables.h"
#include "GroupTables.h"
#include "MapField.h"
#include "ZoneServer.h"
#include "Battle.h"
#include "AggroList.h"
#include "../Shared/ShineLogSystem.h"
#include <math.h>

namespace fiesta {

MobAIRunner& MobAIRunner::Get() { static MobAIRunner s; return s; }

namespace {
inline float Dist2(const Vec3& a, const Vec3& b) {
    float dx = a.x - b.x, dy = a.y - b.y, dz = a.z - b.z;
    return dx*dx + dy*dy + dz*dz;
}
inline ShinePlayer* PickClosestPlayer(const Field& rF, const ShineMob* pkMob,
                                      float* pOutR2 = NULL) {
    const std::vector<ShineObject*>& objs = rF.Objects();
    ShinePlayer* best = NULL;
    float bestR2 = 1e30f;
    for (size_t i = 0; i < objs.size(); ++i) {
        if (objs[i]->GetType() != OT_PLAYER) continue;
        ShinePlayer* pkP = (ShinePlayer*)objs[i];
        float r2 = Dist2(pkMob->GetPos(), pkP->GetPos());
        if (r2 < bestR2) { bestR2 = r2; best = pkP; }
    }
    if (pOutR2) *pOutR2 = bestR2;
    return best;
}
} // namespace

void MobAIRunner::Tick(uint64 uiNowMs) {
    bool bDecayThisTick = (uiNowMs - m_uiLastDecayMs >= 5000ULL);
    if (bDecayThisTick) m_uiLastDecayMs = uiNowMs;

    // Iterate every Field. The set is small (number of zones this server hosts).
    const std::vector<MapInfoRow>& maps = MapTables::Get().Maps();
    for (size_t mi = 0; mi < maps.size(); ++mi) {
        Field* pkF = MapDataBox::Get().GetField((MapID)maps[mi].uiID);
        if (!pkF) continue;
        const std::vector<ShineObject*>& objs = pkF->Objects();
        for (size_t i = 0; i < objs.size(); ++i) {
            if (objs[i]->GetType() != OT_MOB) continue;
            ShineMob* pkMob = (ShineMob*)objs[i];
            if (pkMob->IsDead()) continue;

            // ---- 1. Aggro decay --------------------------------------
            if (bDecayThisTick) pkMob->m_kAggro.Decay(10);

            // ---- 2. Detection / engagement gate ----------------------
            CharID curTop = pkMob->m_kAggro.Top();
            if (curTop == 0) {
                ShinePlayer* pkP = PickClosestPlayer(*pkF, pkMob);
                if (pkP && MobAISystem::IsInAggroRange(pkMob, pkP, false)) {
                    pkMob->m_kAggro.AddScaled(pkP->GetCharID(), 1,
                                              pkP->GetLevel(), pkMob->m_uiLevel);
                    const MobInfoRow* info =
                        MobTables::Get().Find((uint32)pkMob->m_uiSpecies);
                    if (info) {
                        std::string say = MobChatTable::Get().Pick(
                            MobChatTable::MC_PIECE, info->kInxName);
                        if (!say.empty())
                            SHINELOG_DEBUG("MobChat[PIECE] %s -> %s: %s",
                                           info->kInxName.c_str(),
                                           pkP->GetName().c_str(), say.c_str());
                    }
                }
                continue;
            }

            // ---- 3. Chase ---------------------------------------------
            ShinePlayer* pkTgt = ZoneServer::Get().FindPlayerByCharID(curTop);
            if (!pkTgt || pkTgt->IsDead() ||
                !MobAISystem::IsInAggroRange(pkMob, pkTgt, true)) {
                pkMob->m_kAggro.Drop(curTop);
                continue;
            }

            // ---- 4. Attack -------------------------------------------
            float r2 = Dist2(pkMob->GetPos(), pkTgt->GetPos());
            // Melee threshold: 4 cells^2 (16). Future pass replaces with
            // per-attack range from the matched MobAttackSequence step.
            if (r2 <= 16.0f) {
                // Drive the attack sequence: pick the next step from
                // MobAttackSequenceBox; the matching MobBehaviorBox tree
                // owns mid-fight skill choice.
                const MobInfoRow* info =
                    MobTables::Get().Find((uint32)pkMob->m_uiSpecies);
                if (info) {
                    const MobAttackSequence* pkSeq =
                        MobAttackSequenceBox::Get().Find(info->kInxName);
                    (void)pkSeq;     // step-cursor advancement lands in Pass 2.
                    // First-attack chat hint (only fires when we've just
                    // entered melee, not every tick).
                    if (pkMob->m_uiLastAttackMs + 2000 <= uiNowMs) {
                        std::string say = MobChatTable::Get().Pick(
                            MobChatTable::MC_ATTACK, info->kInxName);
                        if (!say.empty())
                            SHINELOG_DEBUG("MobChat[ATTACK] %s: %s",
                                           info->kInxName.c_str(), say.c_str());
                    }
                }
                // Hand off to Battle::Apply with a flat damage figure.
                DamageInfo di;
                di.iPhys = 10;-- replace with BattleStat curve
                di.iMagic= 0;
                di.bMiss = false;
                Battle::Apply(pkMob, pkTgt, di);
                pkMob->m_uiLastAttackMs = uiNowMs;
            }
        }
    }
}

} // namespace fiesta
