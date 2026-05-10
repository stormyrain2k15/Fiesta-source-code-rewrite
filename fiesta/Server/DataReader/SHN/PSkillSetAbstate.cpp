// Server/DataReader/SHN/PSkillSetAbstate.cpp
// Auto-generated: one-file-per-SHN split for PSkillSetAbstate.shn
#include "PSkillSetAbstate.h"
#include "../../Shared/ShineLogSystem.h"

namespace fiesta {

PSkillSetAbstateShn& PSkillSetAbstateShn::Get() { static PSkillSetAbstateShn s; return s; }

void PSkillSetAbstateShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("PSkillSetAbstate");
    if (!t) { SHINELOG_WARN("PSkillSetAbstate.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        PSkillSetAbstateRow rec;
        rec.kPS_InxName = ShnGetStr(*t, _r, "PS_InxName");
        rec.uiPS_Condition = ShnGetU32(*t, _r, "PS_Condition");
        rec.uiPS_ConditioRate = (uint16)ShnGetU32(*t, _r, "PS_ConditioRate");
        rec.kPS_AbStateInx = ShnGetStr(*t, _r, "PS_AbStateInx");
        rec.uiStrength = (uint8)ShnGetU32(*t, _r, "Strength");
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("PSkillSetAbstate.shn: %u rows", (uint32)m_kRows.size());
}

} // namespace fiesta
