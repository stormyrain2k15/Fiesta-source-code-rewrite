// Server/DataReader/SHN/MobAutoAction.cpp
// Auto-generated: one-file-per-SHN split for MobAutoAction.shn
#include "MobAutoAction.h"
#include "../../Shared/ShineLogSystem.h"

namespace shine {

MobAutoActionShn& MobAutoActionShn::Get() { static MobAutoActionShn s; return s; }

void MobAutoActionShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("MobAutoAction");
    if (!t) { SHINELOG_WARN("MobAutoAction.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        MobAutoActionRow rec;
        rec.kMobInx = ShnGetStr(*t, _r, "MobInx");
        rec.uiAttack = ShnGetU32(*t, _r, "Attack");
        rec.uiTarget = ShnGetU32(*t, _r, "Target");
        rec.uiActionType = ShnGetU32(*t, _r, "ActionType");
        rec.kStateInx = ShnGetStr(*t, _r, "StateInx");
        rec.uiStrength = (uint8)ShnGetU32(*t, _r, "Strength");
        rec.uiEffectRate = (uint16)ShnGetU32(*t, _r, "EffectRate");
        rec.uiRange = (uint16)ShnGetU32(*t, _r, "Range");
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("MobAutoAction.shn: %u rows", (uint32)m_kRows.size());
}

} // namespace shine
