// Server/DataReader/SHN/AreaSkill.cpp
// Auto-generated: one-file-per-SHN split for AreaSkill.shn
#include "AreaSkill.h"
#include "../../Shared/ShineLogSystem.h"

namespace fiesta {

AreaSkillShn& AreaSkillShn::Get() { static AreaSkillShn s; return s; }

void AreaSkillShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("AreaSkill");
    if (!t) { SHINELOG_WARN("AreaSkill.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        AreaSkillRow rec;
        rec.kAS_SkillInx = ShnGetStr(*t, _r, "AS_SkillInx");
        rec.uiAS_Step = (uint8)ShnGetU32(*t, _r, "AS_Step");
        rec.kAS_BMPIndex = ShnGetStr(*t, _r, "AS_BMPIndex");
        rec.uiAS_ImagePin = ShnGetU32(*t, _r, "AS_ImagePin");
        rec.uiAS_IsDirection = (uint8)ShnGetU32(*t, _r, "AS_IsDirection");
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("AreaSkill.shn: %u rows", (uint32)m_kRows.size());
}

} // namespace fiesta
