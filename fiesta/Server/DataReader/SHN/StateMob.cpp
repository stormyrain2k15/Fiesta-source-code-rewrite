// Server/DataReader/SHN/StateMob.cpp
// Auto-generated: one-file-per-SHN split for StateMob.shn
#include "StateMob.h"
#include "../../Shared/ShineLogSystem.h"

namespace shine {

StateMobShn& StateMobShn::Get() { static StateMobShn s; return s; }

void StateMobShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("StateMob");
    if (!t) { SHINELOG_WARN("StateMob.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        StateMobRow rec;
        rec.kTargetMobInx = ShnGetStr(*t, _r, "TargetMobInx");
        rec.kConditionMobInx = ShnGetStr(*t, _r, "ConditionMobInx");
        rec.kAbStateInx = ShnGetStr(*t, _r, "AbStateInx");
        rec.uiStrength = ShnGetU32(*t, _r, "Strength");
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("StateMob.shn: %u rows", (uint32)m_kRows.size());
}

} // namespace shine
