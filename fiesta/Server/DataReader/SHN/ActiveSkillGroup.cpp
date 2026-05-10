// Server/DataReader/SHN/ActiveSkillGroup.cpp
// Auto-generated: one-file-per-SHN split for ActiveSkillGroup.shn
#include "ActiveSkillGroup.h"
#include "../../Shared/ShineLogSystem.h"

namespace fiesta {

ActiveSkillGroupShn& ActiveSkillGroupShn::Get() { static ActiveSkillGroupShn s; return s; }

void ActiveSkillGroupShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("ActiveSkillGroup");
    if (!t) { SHINELOG_WARN("ActiveSkillGroup.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        ActiveSkillGroupRow rec;
        rec.kInxName = ShnGetStr(*t, _r, "InxName");
        rec.uiActiveSkillGroupIndex = ShnGetU32(*t, _r, "ActiveSkillGroupIndex");
        m_kByInx[rec.kInxName] = m_kRows.size();
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("ActiveSkillGroup.shn: %u rows", (uint32)m_kRows.size());
}

const ActiveSkillGroupRow* ActiveSkillGroupShn::FindByInx(const std::string& rInx) const {
    std::map<std::string, size_t>::const_iterator it = m_kByInx.find(rInx);
    return (it == m_kByInx.end()) ? NULL : &m_kRows[it->second];
}

} // namespace fiesta
