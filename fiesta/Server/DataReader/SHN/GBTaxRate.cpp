// Server/DataReader/SHN/GBTaxRate.cpp
// Auto-generated: one-file-per-SHN split for GBTaxRate.shn
#include "GBTaxRate.h"
#include "../../Shared/ShineLogSystem.h"

namespace fiesta {

GBTaxRateShn& GBTaxRateShn::Get() { static GBTaxRateShn s; return s; }

void GBTaxRateShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("GBTaxRate");
    if (!t) { SHINELOG_WARN("GBTaxRate.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        GBTaxRateRow rec;
        rec.uiGameType = ShnGetU32(*t, _r, "GameType");
        rec.uiGB_TaxRate = (uint16)ShnGetU32(*t, _r, "GB_TaxRate");
        rec.uiGB_JPSave = ShnGetU32(*t, _r, "GB_JPSave");
        rec.uiGB_JPSaveRate = (uint16)ShnGetU32(*t, _r, "GB_JPSaveRate");
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("GBTaxRate.shn: %u rows", (uint32)m_kRows.size());
}

} // namespace fiesta
