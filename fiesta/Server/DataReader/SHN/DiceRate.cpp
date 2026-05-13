// Server/DataReader/SHN/DiceRate.cpp
// Auto-generated: one-file-per-SHN split for DiceRate.shn
#include "DiceRate.h"
#include "../../Shared/ShineLogSystem.h"

namespace shine {

DiceRateShn& DiceRateShn::Get() { static DiceRateShn s; return s; }

void DiceRateShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("DiceRate");
    if (!t) { SHINELOG_WARN("DiceRate.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        DiceRateRow rec;
        rec.uiRate = ShnGetU32(*t, _r, "Rate");
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("DiceRate.shn: %u rows", (uint32)m_kRows.size());
}

} // namespace shine
