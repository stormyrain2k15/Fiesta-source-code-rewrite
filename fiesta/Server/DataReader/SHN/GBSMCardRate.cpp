// Server/DataReader/SHN/GBSMCardRate.cpp
// Auto-generated: one-file-per-SHN split for GBSMCardRate.shn
#include "GBSMCardRate.h"
#include "../../Shared/ShineLogSystem.h"

namespace fiesta {

GBSMCardRateShn& GBSMCardRateShn::Get() { static GBSMCardRateShn s; return s; }

void GBSMCardRateShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("GBSMCardRate");
    if (!t) { SHINELOG_WARN("GBSMCardRate.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        GBSMCardRateRow rec;
        rec.uiGBSMCard = ShnGetU32(*t, _r, "GBSMCard");
        rec.uiGBSM_CardRate = (uint16)ShnGetU32(*t, _r, "GBSM_CardRate");
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("GBSMCardRate.shn: %u rows", (uint32)m_kRows.size());
}

} // namespace fiesta
