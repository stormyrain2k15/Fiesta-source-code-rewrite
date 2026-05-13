// Server/DataReader/SHN/GBSMJPRate.cpp
// Auto-generated: one-file-per-SHN split for GBSMJPRate.shn
#include "GBSMJPRate.h"
#include "../../Shared/ShineLogSystem.h"

namespace shine {

GBSMJPRateShn& GBSMJPRateShn::Get() { static GBSMJPRateShn s; return s; }

void GBSMJPRateShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("GBSMJPRate");
    if (!t) { SHINELOG_WARN("GBSMJPRate.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        GBSMJPRateRow rec;
        rec.uiGBSM_MinCount = ShnGetU32(*t, _r, "GBSM_MinCount");
        rec.uiGBSM_MaxCount = ShnGetU32(*t, _r, "GBSM_MaxCount");
        rec.uiGBSM_JPRate = ShnGetU32(*t, _r, "GBSM_JPRate");
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("GBSMJPRate.shn: %u rows", (uint32)m_kRows.size());
}

} // namespace shine
