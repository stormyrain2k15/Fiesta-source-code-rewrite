// Server/Zone/PowerGuardStoneSystem.cpp
#include "PowerGuardStoneSystem.h"
#include "ShineObject.h"
#include "ShineNPCTable.h"
#include "Inventory.h"
#include "../Shared/ShineLogSystem.h"
#include "../Shared/GTimer.h"
#include <windows.h>

namespace shine {

static uint64 NowMs() { return (uint64)GTimer::NowMillis(); }
static eShineClass ClassOf(ShinePlayer* /*pk*/) { return SC_FIGHTER; }   // until ShinePlayerCharacter::m_eClass exists

eStoneUseResult PowerGuardStoneSystem::Use(ShinePlayer* pkP, StoneCounts& rC, bool bIsPower) {
    if (!pkP || pkP->IsDead()) return STONE_USE_DEAD;
    uint64 now = NowMs();
    uint64& rNext = bIsPower ? rC.uiNextPwrUseTickMs : rC.uiNextGrdUseTickMs;
    if (rNext > now) return STONE_USE_ON_COOLDOWN;

    uint16& rCount = bIsPower ? rC.uiPwrCount : rC.uiGrdCount;
    if (rCount == 0) return STONE_USE_NONE_LEFT;

    eShineClass eC = ClassOf(pkP);
    uint16 uiL = pkP->GetLevel();

    int32 a = 0, b = 0;
    if (bIsPower) {
        a = ClassParamTable::Get().PwrStoneWcForLevel(eC, uiL);
        b = ClassParamTable::Get().PwrStoneMaForLevel(eC, uiL);
        if (a <= 0 && b <= 0) return STONE_USE_NO_PARAM;
        rC.nActivePwrWC = a; rC.nActivePwrMA = b;
        rC.uiPwrBuffExpiresMs = now + (uint64)kStoneBuffSec * 1000ULL;
    } else {
        a = ClassParamTable::Get().GrdStoneAcForLevel(eC, uiL);
        b = ClassParamTable::Get().GrdStoneMrForLevel(eC, uiL);
        if (a <= 0 && b <= 0) return STONE_USE_NO_PARAM;
        rC.nActiveGrdAC = a; rC.nActiveGrdMR = b;
        rC.uiGrdBuffExpiresMs = now + (uint64)kStoneBuffSec * 1000ULL;
    }
    --rCount;
    rNext = now + (uint64)kStoneCooldownMs;
    return STONE_USE_OK;
}

uint16 PowerGuardStoneSystem::Grant(StoneCounts& rC, eShineClass eClass, uint16 uiLevel,
                                     bool bIsPower, uint16 uiQty) {
    int32 cap = bIsPower
        ? ClassParamTable::Get().PwrStoneCapForLevel(eClass, uiLevel)
        : ClassParamTable::Get().GrdStoneCapForLevel(eClass, uiLevel);
    if (cap <= 0) cap = 9999;
    uint16& rCount = bIsPower ? rC.uiPwrCount : rC.uiGrdCount;
    int32 sum = (int32)rCount + (int32)uiQty;
    if (sum > cap) sum = cap;
    int32 added = sum - (int32)rCount;
    rCount = (uint16)sum;
    return (uint16)(added < 0 ? 0 : added);
}

bool PowerGuardStoneSystem::TickBuff(StoneCounts& rC, bool bIsPower) {
    uint64 now = NowMs();
    if (bIsPower) {
        if (rC.uiPwrBuffExpiresMs == 0) return false;
        if (rC.uiPwrBuffExpiresMs > now) return false;
        rC.uiPwrBuffExpiresMs = 0;
        rC.nActivePwrWC = 0;
        rC.nActivePwrMA = 0;
        return true;
    } else {
        if (rC.uiGrdBuffExpiresMs == 0) return false;
        if (rC.uiGrdBuffExpiresMs > now) return false;
        rC.uiGrdBuffExpiresMs = 0;
        rC.nActiveGrdAC = 0;
        rC.nActiveGrdMR = 0;
        return true;
    }
}

uint32 PowerGuardStoneSystem::RemainingPwrCooldownMs(const StoneCounts& rC) {
    uint64 now = NowMs();
    return (rC.uiNextPwrUseTickMs <= now) ? 0u : (uint32)(rC.uiNextPwrUseTickMs - now);
}
uint32 PowerGuardStoneSystem::RemainingGrdCooldownMs(const StoneCounts& rC) {
    uint64 now = NowMs();
    return (rC.uiNextGrdUseTickMs <= now) ? 0u : (uint32)(rC.uiNextGrdUseTickMs - now);
}

// ---- Vendor ----
bool PowerGuardStoneVendor::IsVendor(const std::string& rNpcMobName) {
    const ShineNPCRow* p = ShineNPCTable::Get().FindNPC(rNpcMobName);
    if (!p) return false;
    if (_stricmp(p->kRole.c_str(), "Merchant") != 0) return false;
    if (_stricmp(p->kRoleArg0.c_str(), "PowerStone") == 0) return true;
    if (_stricmp(p->kRoleArg0.c_str(), "GuardStone") == 0) return true;
    if (_stricmp(p->kRoleArg0.c_str(), "Stone")      == 0) return true;
    return false;
}

eStoneUseResult PowerGuardStoneVendor::Buy(ShinePlayer*       pkP,
                                            const std::string& rNpcMobName,
                                            Inventory&         rInv,
                                            StoneCounts&       rCounts,
                                            bool               bIsPower,
                                            uint16             uiQty) {
    if (!pkP) return STONE_USE_DEAD;
    if (uiQty == 0) return STONE_USE_NO_PARAM;
    if (!IsVendor(rNpcMobName)) return STONE_USE_NO_PARAM;

    eShineClass eC = ClassOf(pkP);
    uint16 uiL = pkP->GetLevel();
    int32 unit = bIsPower
        ? ClassParamTable::Get().PwrStonePriceForLevel(eC, uiL)
        : ClassParamTable::Get().GrdStonePriceForLevel(eC, uiL);
    if (unit <= 0) return STONE_USE_NO_PARAM;
    int64 total = (int64)unit * (int64)uiQty;
    if (rInv.Money() < total) return STONE_USE_NONE_LEFT;
    rInv.AddMoney(-total);
    Grant(rCounts, eC, uiL, bIsPower, uiQty);
    return STONE_USE_OK;
}

} // namespace shine
