// Server/DataReader/SHN/QuestSpecies.cpp
// Auto-generated: one-file-per-SHN split for QuestSpecies.shn
#include "QuestSpecies.h"
#include "../../Shared/ShineLogSystem.h"

namespace shine {

QuestSpeciesShn& QuestSpeciesShn::Get() { static QuestSpeciesShn s; return s; }

void QuestSpeciesShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("QuestSpecies");
    if (!t) { SHINELOG_WARN("QuestSpecies.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        QuestSpeciesRow rec;
        rec.uiID = (uint16)ShnGetU32(*t, _r, "ID");
        rec.kMobGroupName = ShnGetStr(*t, _r, "MobGroupName");
        rec.kUnkCol2 = ShnGetStr(*t, _r, "UnkCol2");
        rec.kUnkCol3 = ShnGetStr(*t, _r, "UnkCol3");
        rec.kUnkCol4 = ShnGetStr(*t, _r, "UnkCol4");
        rec.kUnkCol5 = ShnGetStr(*t, _r, "UnkCol5");
        m_kById[rec.uiID] = m_kRows.size();
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("QuestSpecies.shn: %u rows", (uint32)m_kRows.size());
}

const QuestSpeciesRow* QuestSpeciesShn::FindById(uint32 uiID) const {
    std::map<uint32, size_t>::const_iterator it = m_kById.find(uiID);
    return (it == m_kById.end()) ? NULL : &m_kRows[it->second];
}

} // namespace shine
