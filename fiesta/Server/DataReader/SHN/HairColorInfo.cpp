// Server/DataReader/SHN/HairColorInfo.cpp
// Auto-generated: one-file-per-SHN split for HairColorInfo.shn
#include "HairColorInfo.h"
#include "../../Shared/ShineLogSystem.h"

namespace fiesta {

HairColorInfoShn& HairColorInfoShn::Get() { static HairColorInfoShn s; return s; }

void HairColorInfoShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("HairColorInfo");
    if (!t) { SHINELOG_WARN("HairColorInfo.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        HairColorInfoRow rec;
        rec.uiID = (uint8)ShnGetU32(*t, _r, "ID");
        rec.kIndexName = ShnGetStr(*t, _r, "IndexName");
        rec.kName = ShnGetStr(*t, _r, "Name");
        rec.kColorTextureName = ShnGetStr(*t, _r, "ColorTextureName");
        rec.uiGrade = (uint8)ShnGetU32(*t, _r, "Grade");
        m_kById[rec.uiID] = m_kRows.size();
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("HairColorInfo.shn: %u rows", (uint32)m_kRows.size());
}

const HairColorInfoRow* HairColorInfoShn::FindById(uint32 uiID) const {
    std::map<uint32, size_t>::const_iterator it = m_kById.find(uiID);
    return (it == m_kById.end()) ? NULL : &m_kRows[it->second];
}

} // namespace fiesta
