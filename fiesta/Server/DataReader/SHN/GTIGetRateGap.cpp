// Server/DataReader/SHN/GTIGetRateGap.cpp
// Auto-generated: one-file-per-SHN split for GTIGetRateGap.shn
#include "GTIGetRateGap.h"
#include "../../Shared/ShineLogSystem.h"

namespace shine {

GTIGetRateGapShn& GTIGetRateGapShn::Get() { static GTIGetRateGapShn s; return s; }

void GTIGetRateGapShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("GTIGetRateGap");
    if (!t) { SHINELOG_WARN("GTIGetRateGap.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        GTIGetRateGapRow rec;
        rec.uiID = (uint8)ShnGetU32(*t, _r, "ID");
        rec.uiGap = (uint16)ShnGetU32(*t, _r, "Gap");
        m_kById[rec.uiID] = m_kRows.size();
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("GTIGetRateGap.shn: %u rows", (uint32)m_kRows.size());
}

const GTIGetRateGapRow* GTIGetRateGapShn::FindById(uint32 uiID) const {
    std::map<uint32, size_t>::const_iterator it = m_kById.find(uiID);
    return (it == m_kById.end()) ? NULL : &m_kRows[it->second];
}

} // namespace shine
