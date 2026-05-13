// Server/DataReader/SHN/MoverUseSkill.cpp
// Auto-generated: one-file-per-SHN split for MoverUseSkill.shn
#include "MoverUseSkill.h"
#include "../../Shared/ShineLogSystem.h"

namespace shine {

MoverUseSkillShn& MoverUseSkillShn::Get() { static MoverUseSkillShn s; return s; }

void MoverUseSkillShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("MoverUseSkill");
    if (!t) { SHINELOG_WARN("MoverUseSkill.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        MoverUseSkillRow rec;
        rec.kMoverIDX = ShnGetStr(*t, _r, "MoverIDX");
        rec.kActiveSkillIDX = ShnGetStr(*t, _r, "ActiveSkillIDX");
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("MoverUseSkill.shn: %u rows", (uint32)m_kRows.size());
}

} // namespace shine
