// Server/DataReader/SHN/GTIGetRate.cpp
// Auto-generated: one-file-per-SHN split for GTIGetRate.shn
#include "GTIGetRate.h"
#include "../../Shared/ShineLogSystem.h"

namespace shine {

GTIGetRateShn& GTIGetRateShn::Get() { static GTIGetRateShn s; return s; }

void GTIGetRateShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("GTIGetRate");
    if (!t) { SHINELOG_WARN("GTIGetRate.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        GTIGetRateRow rec;
        rec.uiID = (uint8)ShnGetU32(*t, _r, "ID");
        rec.uiWin1 = (uint8)ShnGetU32(*t, _r, "Win1");
        rec.uiLose1 = (uint8)ShnGetU32(*t, _r, "Lose1");
        rec.uiWin2 = (uint8)ShnGetU32(*t, _r, "Win2");
        rec.uiLose2 = (uint8)ShnGetU32(*t, _r, "Lose2");
        rec.uiWin3 = (uint8)ShnGetU32(*t, _r, "Win3");
        rec.uiLose3 = (uint8)ShnGetU32(*t, _r, "Lose3");
        m_kById[rec.uiID] = m_kRows.size();
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("GTIGetRate.shn: %u rows", (uint32)m_kRows.size());
}

const GTIGetRateRow* GTIGetRateShn::FindById(uint32 uiID) const {
    std::map<uint32, size_t>::const_iterator it = m_kById.find(uiID);
    return (it == m_kById.end()) ? NULL : &m_kRows[it->second];
}

} // namespace shine
