// Server/Zone/SoulStoneSystem.cpp
#include "SoulStoneSystem.h"
#include "ShineObject.h"
#include "../Shared/ShineLogSystem.h"
#include <windows.h>

namespace fiesta {

static uint64 NowMs() {
    // GetTickCount64 is XP+SP1 / 2003 onwards; safe on VS2010 / Win32.
    return (uint64)GetTickCount64();
}

eSoulStoneUseResult SoulStoneSystem::Use(ShinePlayer*      pkP,
                                          SoulStoneCounts& rC,
                                          bool             bIsHp,
                                          int              iTier) {
    if (!pkP) return SS_USE_DEAD;
    if (iTier < 0 || iTier >= kSoulStoneTierCount) return SS_USE_INVALID_TIER;

    // Liveness: dead players cannot use soul stones (must use the death
    // revive dialog instead).
    if (pkP->IsDead()) return SS_USE_DEAD;

    // Cooldown gate.
    uint64 now = NowMs();
    if (rC.uiNextUseTickMs > now) return SS_USE_ON_COOLDOWN;

    // Counter gate.
    uint16& rCount = bIsHp ? rC.aHpStones[iTier] : rC.aSpStones[iTier];
    if (rCount == 0) return SS_USE_NONE_LEFT;

    // "Already at full" gate -- prevents wasting a stone.
    if (bIsHp) {
        if (pkP->GetHP() >= pkP->GetMaxHP()) return SS_USE_AT_MAX_HP;
    } else {
        if (pkP->GetSP() >= pkP->GetMaxSP()) return SS_USE_AT_MAX_SP;
    }

    // Apply.
    if (bIsHp) {
        int32 newHp = pkP->GetHP() + kHpSoulHeal[iTier];
        if (newHp > pkP->GetMaxHP()) newHp = pkP->GetMaxHP();
        pkP->SetHP(newHp);
    } else {
        int32 newSp = pkP->GetSP() + kSpSoulHeal[iTier];
        if (newSp > pkP->GetMaxSP()) newSp = pkP->GetMaxSP();
        pkP->SetSP(newSp);
    }
    --rCount;
    rC.uiNextUseTickMs = now + (uint64)kSoulStoneCooldownMs;
    return SS_USE_OK;
}

void SoulStoneSystem::Grant(SoulStoneCounts& rC, bool bIsHp, int iTier, uint16 uiQty) {
    if (iTier < 0 || iTier >= kSoulStoneTierCount) return;
    uint16& rCount = bIsHp ? rC.aHpStones[iTier] : rC.aSpStones[iTier];
    uint32 sum = (uint32)rCount + (uint32)uiQty;
    if (sum > (uint32)kSoulStoneMaxPerTier) sum = kSoulStoneMaxPerTier;
    rCount = (uint16)sum;
}

uint32 SoulStoneSystem::RemainingCooldownMs(const SoulStoneCounts& rC) {
    uint64 now = NowMs();
    if (rC.uiNextUseTickMs <= now) return 0;
    return (uint32)(rC.uiNextUseTickMs - now);
}

uint32 SoulStoneSystem::TotalHp(const SoulStoneCounts& rC) {
    uint32 t = 0;
    for (int i = 0; i < kSoulStoneTierCount; ++i) t += rC.aHpStones[i];
    return t;
}

uint32 SoulStoneSystem::TotalSp(const SoulStoneCounts& rC) {
    uint32 t = 0;
    for (int i = 0; i < kSoulStoneTierCount; ++i) t += rC.aSpStones[i];
    return t;
}

} // namespace fiesta
