// Server/Zone/MobAIRunner.h
// 19+ -- Per-tick mob AI driver that ties together MobAISystem (Lua),
// MobBehaviorBox (decision trees), MobAttackSequenceBox (attack patterns),
// MobChatTable (taunts / death lines), and AggroList.
//
// Lifecycle per mob, per tick:
//   1. Decay aggro (every 5s).
//   2. If aggro Top() == 0: detect nearby players with `DetectCha` radius.
//      First detection -> MobChat[PIECE] line.
//   3. If aggro Top() != 0: chase with `FollowCha`.
//   4. When in melee range: attack (driven by MobAttackSequenceBox).
//      First attack of the engagement -> MobChat[ATTACK] line.
//   5. On HP-low threshold: HELPMAIN/HELPSUB calls (broadcast to allies).
//
// EVIDENCE: PDB_CONFIRMED  symbol: MobAI, MobAIRunner, MobBehavior, MobAttackSequence
// EVIDENCE: DATA_CONFIRMED  source: MobBehaviorDescript/, MobAttackSequence/, World/MobChat.txt
#ifndef FIESTA_ZONE_MOBAIRUNNER_H
#define FIESTA_ZONE_MOBAIRUNNER_H
#include "../Shared/ShineTypes.h"

namespace fiesta {

class MobAIRunner {
public:
    static MobAIRunner& Get();
    // Called every Zone tick from Main.cpp.
    void Tick(uint64 uiNowMs);
private:
    MobAIRunner() : m_uiLastDecayMs(0) {}
    uint64 m_uiLastDecayMs;
};

} // namespace fiesta
#endif
