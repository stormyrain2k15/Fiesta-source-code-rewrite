// Server/DataReader/SHN/GTIBreedSubject.cpp
// Auto-generated: one-file-per-SHN split for GTIBreedSubject.shn
#include "GTIBreedSubject.h"
#include "../../Shared/ShineLogSystem.h"

namespace fiesta {

GTIBreedSubjectShn& GTIBreedSubjectShn::Get() { static GTIBreedSubjectShn s; return s; }

void GTIBreedSubjectShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("GTIBreedSubject");
    if (!t) { SHINELOG_WARN("GTIBreedSubject.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        GTIBreedSubjectRow rec;
        rec.kInxName = ShnGetStr(*t, _r, "InxName");
        rec.kMobIndex = ShnGetStr(*t, _r, "MobIndex");
        m_kByInx[rec.kInxName] = m_kRows.size();
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("GTIBreedSubject.shn: %u rows", (uint32)m_kRows.size());
}

const GTIBreedSubjectRow* GTIBreedSubjectShn::FindByInx(const std::string& rInx) const {
    std::map<std::string, size_t>::const_iterator it = m_kByInx.find(rInx);
    return (it == m_kByInx.end()) ? NULL : &m_kRows[it->second];
}

} // namespace fiesta
