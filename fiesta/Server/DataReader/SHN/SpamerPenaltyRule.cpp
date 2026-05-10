// Server/DataReader/SHN/SpamerPenaltyRule.cpp
// Auto-generated: one-file-per-SHN split for SpamerPenaltyRule.shn
#include "SpamerPenaltyRule.h"
#include "../../Shared/ShineLogSystem.h"

namespace fiesta {

SpamerPenaltyRuleShn& SpamerPenaltyRuleShn::Get() { static SpamerPenaltyRuleShn s; return s; }

void SpamerPenaltyRuleShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("SpamerPenaltyRule");
    if (!t) { SHINELOG_WARN("SpamerPenaltyRule.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        SpamerPenaltyRuleRow rec;
        rec.uiSecondRule = (uint16)ShnGetU32(*t, _r, "SecondRule");
        rec.uiRepeatRule = (uint8)ShnGetU32(*t, _r, "RepeatRule");
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("SpamerPenaltyRule.shn: %u rows", (uint32)m_kRows.size());
}

} // namespace fiesta
