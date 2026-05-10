// Server/DataReader/SHN/BelongDice.cpp
// Auto-generated: one-file-per-SHN split for BelongDice.shn
#include "BelongDice.h"
#include "../../Shared/ShineLogSystem.h"

namespace fiesta {

BelongDiceShn& BelongDiceShn::Get() { static BelongDiceShn s; return s; }

void BelongDiceShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("BelongDice");
    if (!t) { SHINELOG_WARN("BelongDice.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        BelongDiceRow rec;
        rec.uiDiceTimeLimit = (uint8)ShnGetU32(*t, _r, "DiceTimeLimit");
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("BelongDice.shn: %u rows", (uint32)m_kRows.size());
}

} // namespace fiesta
