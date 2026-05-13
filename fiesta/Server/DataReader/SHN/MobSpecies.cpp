// Server/DataReader/SHN/MobSpecies.cpp
// Auto-generated: one-file-per-SHN split for MobSpecies.shn
#include "MobSpecies.h"
#include "../../Shared/ShineLogSystem.h"

namespace shine {

MobSpeciesShn& MobSpeciesShn::Get() { static MobSpeciesShn s; return s; }

void MobSpeciesShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("MobSpecies");
    if (!t) { SHINELOG_WARN("MobSpecies.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        MobSpeciesRow rec;
        rec.uiID = (uint16)ShnGetU32(*t, _r, "ID");
        rec.kMobName = ShnGetStr(*t, _r, "MobName");
        rec.kUnkCol2 = ShnGetStr(*t, _r, "UnkCol2");
        rec.kUnkCol3 = ShnGetStr(*t, _r, "UnkCol3");
        rec.kUnkCol4 = ShnGetStr(*t, _r, "UnkCol4");
        m_kById[rec.uiID] = m_kRows.size();
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("MobSpecies.shn: %u rows", (uint32)m_kRows.size());
}

const MobSpeciesRow* MobSpeciesShn::FindById(uint32 uiID) const {
    std::map<uint32, size_t>::const_iterator it = m_kById.find(uiID);
    return (it == m_kById.end()) ? NULL : &m_kRows[it->second];
}

} // namespace shine
