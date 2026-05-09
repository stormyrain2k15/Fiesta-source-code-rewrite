// Server/Zone/PotSystem.cpp
#include "PotSystem.h"
#include "AbState.h"
#include "../Shared/GTimer.h"
#include "../Shared/ShineLogSystem.h"

namespace fiesta {

PotSystem& PotSystem::Get() { static PotSystem s; return s; }
PotSystem::PotSystem() {}

void PotSystem::Register(uint32 uiItemId, const PotDef& rDef) {
    for (size_t i = 0; i < m_kKnown.size(); ++i) {
        if (m_kKnown[i].uiItemId == uiItemId) { m_kKnown[i].kDef = rDef; return; }
    }
    Slot s; s.uiItemId = uiItemId; s.kDef = rDef;
    m_kKnown.push_back(s);
}

const PotDef* PotSystem::FindDef(uint32 uiItemId) const {
    for (size_t i = 0; i < m_kKnown.size(); ++i) {
        if (m_kKnown[i].uiItemId == uiItemId) return &m_kKnown[i].kDef;
    }
    return NULL;
}

bool PotSystem::IsKnown(uint32 uiItemId) const {
    return FindDef(uiItemId) != NULL;
}

bool PotSystem::OnCooldown(CharID c, ePotKind eKind, uint64 uiNowMs) const {
    for (size_t i = 0; i < m_kCooldowns.size(); ++i) {
        if (m_kCooldowns[i].c == c && m_kCooldowns[i].eKind == eKind) {
            return uiNowMs < m_kCooldowns[i].uiReadyAtMs;
        }
    }
    return false;
}

void PotSystem::ArmCooldown(CharID c, ePotKind eKind, uint64 uiUntilMs) {
    for (size_t i = 0; i < m_kCooldowns.size(); ++i) {
        if (m_kCooldowns[i].c == c && m_kCooldowns[i].eKind == eKind) {
            m_kCooldowns[i].uiReadyAtMs = uiUntilMs; return;
        }
    }
    Cooldown cd; cd.c = c; cd.eKind = eKind; cd.uiReadyAtMs = uiUntilMs;
    m_kCooldowns.push_back(cd);
}

bool PotSystem::Use(ShinePlayer* pk, uint32 uiItemId) {
    if (!pk) return false;
    const PotDef* pkDef = FindDef(uiItemId);
    if (!pkDef) return false;
    uint64 now = GTimer::NowMillis();
    if (OnCooldown(pk->GetCharID(), pkDef->eKind, now)) return false;

    int32 iApplied = 0;
    switch (pkDef->eKind) {
        case POT_HP_FLAT:
            iApplied = pkDef->iAmount;
            pk->SetHP(pk->GetHP() + iApplied);
            break;
        case POT_SP_FLAT:
            iApplied = pkDef->iAmount;
            pk->SetSP(pk->GetSP() + iApplied);
            break;
        case POT_HP_PCT: {
            int32 amt = (int32)((int64)pk->GetMaxHP() * (int64)pkDef->iAmount / 100);
            iApplied = amt;
            pk->SetHP(pk->GetHP() + amt);
            break;
        }
        case POT_SP_PCT: {
            int32 amt = (int32)((int64)pk->GetMaxSP() * (int64)pkDef->iAmount / 100);
            iApplied = amt;
            pk->SetSP(pk->GetSP() + amt);
            break;
        }
        case POT_EXP_PCT:
        case POT_REST_EXP:
            // Rested-EXP and EXP-pct bonuses are applied at next mob-kill in
            // QuestRuntime / Battle::Kill. We just record the multiplier on
            // the player at consume time -- pass 2 carries it through.
            iApplied = pkDef->iAmount;
            break;
        case POT_ABSTATE:
            pk->AbState().Apply(pkDef->uiAbStateInxName,
                                (int32)pkDef->uiCooldownMs > 0 ? (int32)pkDef->uiCooldownMs : 60000,
                                /*stk*/1);
            iApplied = (int32)pkDef->uiAbStateInxName;
            break;
        default:
            return false;
    }

    ArmCooldown(pk->GetCharID(), pkDef->eKind, now + (uint64)pkDef->uiCooldownMs);
    SHINELOG_INFO("Pot: cid=%u item=%u kind=%d applied=%d",
                  pk->GetCharID(), uiItemId, (int)pkDef->eKind, iApplied);
    return true;
}

} // namespace fiesta
