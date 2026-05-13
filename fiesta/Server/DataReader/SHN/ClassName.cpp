// Server/DataReader/SHN/ClassName.cpp
// Auto-generated: one-file-per-SHN split for ClassName.shn
#include "ClassName.h"
#include "../../Shared/ShineLogSystem.h"

namespace shine {

ClassNameShn& ClassNameShn::Get() { static ClassNameShn s; return s; }

void ClassNameShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("ClassName");
    if (!t) { SHINELOG_WARN("ClassName.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    m_kById.clear();
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        ClassNameRow rec;
        rec.uiClassID = (uint8)ShnGetU32(*t, _r, "ClassID");
        rec.kAcPrefix = ShnGetStr(*t, _r, "acPrefix");
        rec.kAcEngName = ShnGetStr(*t, _r, "acEngName");
        rec.kAcLocalName = ShnGetStr(*t, _r, "acLocalName");
        m_kById[(uint32)rec.uiClassID] = m_kRows.size();
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("ClassName.shn: %u rows", (uint32)m_kRows.size());
}

const ClassNameRow* ClassNameShn::FindById(uint32 uiID) const {
    std::map<uint32, size_t>::const_iterator it = m_kById.find(uiID);
    return (it == m_kById.end()) ? NULL : &m_kRows[it->second];
}

} // namespace shine
