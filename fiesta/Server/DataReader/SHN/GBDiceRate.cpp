// Server/DataReader/SHN/GBDiceRate.cpp
// Auto-generated: one-file-per-SHN split for GBDiceRate.shn
#include "GBDiceRate.h"
#include "../../Shared/ShineLogSystem.h"

namespace fiesta {

GBDiceRateShn& GBDiceRateShn::Get() { static GBDiceRateShn s; return s; }

void GBDiceRateShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("GBDiceRate");
    if (!t) { SHINELOG_WARN("GBDiceRate.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        GBDiceRateRow rec;
        rec.uiRate = ShnGetU32(*t, _r, "Rate");
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("GBDiceRate.shn: %u rows", (uint32)m_kRows.size());
}

} // namespace fiesta
