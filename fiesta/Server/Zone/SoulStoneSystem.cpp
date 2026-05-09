// Server/Zone/SoulStoneSystem.cpp
#include "SoulStoneSystem.h"
#include "ShineObject.h"
#include "../Shared/ShineLogSystem.h"
#include <windows.h>

namespace fiesta {

static uint64 NowMs() { return (uint64)GetTickCount64(); }

uint8 SoulStoneSystem::TierForLevel(int32 nLevel) {
    if (nLevel < 1) nLevel = 1;
    if (nLevel >= kMaxLevelForTierTable) nLevel = kMaxLevelForTierTable - 1;
    return kSoulStoneTierByLevel[nLevel - 1];
}

int32 SoulStoneSystem::HealForLevel(int32 nLevel, bool bIsHp) {
    uint8 t = TierForLevel(nLevel);
    return bIsHp ? kHpSoulHealByTier[t] : kSpSoulHealByTier[t];
}

int32 SoulStoneSystem::PriceForLevel(int32 nLevel) {
    uint8 t = TierForLevel(nLevel);
    return kSoulStonePriceByTier[t];
}

eSoulStoneUseResult SoulStoneSystem::Use(ShinePlayer*      pkP,
                                          SoulStoneCounts& rC,
                                          bool             bIsHp) {
    if (!pkP)            return SS_USE_DEAD;
    if (pkP->IsDead())   return SS_USE_DEAD;

    // Independent cooldowns.
    uint64 now = NowMs();
    uint64& rNext = bIsHp ? rC.uiNextHpUseTickMs : rC.uiNextSpUseTickMs;
    if (rNext > now) return SS_USE_ON_COOLDOWN;

    // Counter gate.
    uint16& rCount = bIsHp ? rC.uiHpCount : rC.uiSpCount;
    if (rCount == 0) return SS_USE_NONE_LEFT;

    // "Already at full" guard.
    if (bIsHp) {
        if (pkP->GetHP() >= pkP->GetMaxHP()) return SS_USE_AT_MAX_HP;
    } else {
        if (pkP->GetSP() >= pkP->GetMaxSP()) return SS_USE_AT_MAX_SP;
    }

    // Apply -- tier is derived from level, not stored.
    int32 heal = HealForLevel((int32)pkP->GetLevel(), bIsHp);
    if (bIsHp) {
        int32 newHp = pkP->GetHP() + heal;
        if (newHp > pkP->GetMaxHP()) newHp = pkP->GetMaxHP();
        pkP->SetHP(newHp);
    } else {
        int32 newSp = pkP->GetSP() + heal;
        if (newSp > pkP->GetMaxSP()) newSp = pkP->GetMaxSP();
        pkP->SetSP(newSp);
    }
    --rCount;
    rNext = now + (uint64)kSoulStoneCooldownMs;
    return SS_USE_OK;
}

void SoulStoneSystem::Grant(SoulStoneCounts& rC, bool bIsHp, uint16 uiQty) {
    uint16& rCount = bIsHp ? rC.uiHpCount : rC.uiSpCount;
    uint32 sum = (uint32)rCount + (uint32)uiQty;
    if (sum > (uint32)kSoulStoneMaxCount) sum = kSoulStoneMaxCount;
    rCount = (uint16)sum;
}

uint32 SoulStoneSystem::RemainingHpCooldownMs(const SoulStoneCounts& rC) {
    uint64 now = NowMs();
    return (rC.uiNextHpUseTickMs <= now) ? 0u : (uint32)(rC.uiNextHpUseTickMs - now);
}

uint32 SoulStoneSystem::RemainingSpCooldownMs(const SoulStoneCounts& rC) {
    uint64 now = NowMs();
    return (rC.uiNextSpUseTickMs <= now) ? 0u : (uint32)(rC.uiNextSpUseTickMs - now);
}

} // namespace fiesta
