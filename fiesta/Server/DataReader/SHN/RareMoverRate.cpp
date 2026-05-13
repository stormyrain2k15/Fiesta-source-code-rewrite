// Server/DataReader/SHN/RareMoverRate.cpp
// Auto-generated: one-file-per-SHN split for RareMoverRate.shn
#include "RareMoverRate.h"
#include "../../Shared/ShineLogSystem.h"

namespace shine {

RareMoverRateShn& RareMoverRateShn::Get() { static RareMoverRateShn s; return s; }

void RareMoverRateShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("RareMoverRate");
    if (!t) { SHINELOG_WARN("RareMoverRate.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        RareMoverRateRow rec;
        rec.uiRMR_Rate = (uint16)ShnGetU32(*t, _r, "RMR_Rate");
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("RareMoverRate.shn: %u rows", (uint32)m_kRows.size());
}

} // namespace shine
