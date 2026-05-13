// Server/DataReader/SHN/ToggleSkill.cpp
// Auto-generated: one-file-per-SHN split for ToggleSkill.shn
#include "ToggleSkill.h"
#include "../../Shared/ShineLogSystem.h"

namespace shine {

ToggleSkillShn& ToggleSkillShn::Get() { static ToggleSkillShn s; return s; }

void ToggleSkillShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("ToggleSkill");
    if (!t) { SHINELOG_WARN("ToggleSkill.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        ToggleSkillRow rec;
        rec.kTS_SkillInx = ShnGetStr(*t, _r, "TS_SkillInx");
        rec.uiTS_Condition = ShnGetU32(*t, _r, "TS_Condition");
        rec.uiTS_Value = (uint16)ShnGetU32(*t, _r, "TS_Value");
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("ToggleSkill.shn: %u rows", (uint32)m_kRows.size());
}

} // namespace shine
