// Server/DataReader/SHN/RareMoverSubRate.cpp
// Auto-generated: one-file-per-SHN split for RareMoverSubRate.shn
#include "RareMoverSubRate.h"
#include "../../Shared/ShineLogSystem.h"

namespace fiesta {

RareMoverSubRateShn& RareMoverSubRateShn::Get() { static RareMoverSubRateShn s; return s; }

void RareMoverSubRateShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("RareMoverSubRate");
    if (!t) { SHINELOG_WARN("RareMoverSubRate.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        RareMoverSubRateRow rec;
        rec.uiRMR_SubRate = (uint16)ShnGetU32(*t, _r, "RMR_SubRate");
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("RareMoverSubRate.shn: %u rows", (uint32)m_kRows.size());
}

} // namespace fiesta
