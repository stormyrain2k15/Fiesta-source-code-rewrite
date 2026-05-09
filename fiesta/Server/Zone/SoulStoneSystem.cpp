// Server/Zone/SoulStoneSystem.cpp
#include "SoulStoneSystem.h"
#include "ShineObject.h"
#include "../Shared/ShineLogSystem.h"
#include "../Shared/GTimer.h"
#include <windows.h>

namespace fiesta {

static uint64 NowMs() { return (uint64)GTimer::NowMillis(); }

// Resolve the player's class. ShinePlayer doesn't yet expose a class accessor
// directly so we forward-declare the tiny helper inline; once the
// `ShinePlayerCharacter` struct lands this becomes a member call.
static eShineClass ClassOf(ShinePlayer* /*pk*/) {
    // TODO: pull from ShinePlayerCharacter::m_eClass once that field exists.
    // For now Fighter is a safe default that matches the per-class file
    // shape; misclassification only changes per-stone heal/cap/price
    // until the field is wired.
    return SC_FIGHTER;
}

eSoulStoneUseResult SoulStoneSystem::Use(ShinePlayer*      pkP,
                                          SoulStoneCounts& rC,
                                          bool             bIsHp) {
    if (!pkP)            return SS_USE_DEAD;
    if (pkP->IsDead())   return SS_USE_DEAD;

    uint64 now = NowMs();
    uint64& rNext = bIsHp ? rC.uiNextHpUseTickMs : rC.uiNextSpUseTickMs;
    if (rNext > now) return SS_USE_ON_COOLDOWN;

    uint16& rCount = bIsHp ? rC.uiHpCount : rC.uiSpCount;
    if (rCount == 0) return SS_USE_NONE_LEFT;

    if (bIsHp) {
        if (pkP->GetHP() >= pkP->GetMaxHP()) return SS_USE_AT_MAX_HP;
    } else {
        if (pkP->GetSP() >= pkP->GetMaxSP()) return SS_USE_AT_MAX_SP;
    }

    // Look up the per-level heal amount from ClassParamTable (real data).
    eShineClass eC = ClassOf(pkP);
    uint16      uiL = pkP->GetLevel();
    int32 heal = bIsHp
        ? ClassParamTable::Get().SoulHpHealForLevel(eC, uiL)
        : ClassParamTable::Get().SoulSpHealForLevel(eC, uiL);
    if (heal <= 0) {
        SHINELOG_WARN("SoulStone: no ClassParam row for class=%d level=%u", (int)eC, (uint32)uiL);
        return SS_USE_NO_PARAM;
    }

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

uint16 SoulStoneSystem::Grant(SoulStoneCounts& rC,
                              eShineClass      eClass,
                              uint16           uiLevel,
                              bool             bIsHp,
                              uint16           uiQty) {
    int32 cap = bIsHp
        ? ClassParamTable::Get().SoulHpCapForLevel(eClass, uiLevel)
        : ClassParamTable::Get().SoulSpCapForLevel(eClass, uiLevel);
    if (cap <= 0) cap = 9999;       // safe fallback if class param missing
    uint16& rCount = bIsHp ? rC.uiHpCount : rC.uiSpCount;
    int32 sum = (int32)rCount + (int32)uiQty;
    if (sum > cap) sum = cap;
    int32 added = sum - (int32)rCount;
    rCount = (uint16)sum;
    return (uint16)(added < 0 ? 0 : added);
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
