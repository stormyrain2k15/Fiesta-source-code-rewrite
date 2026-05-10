// Server/DataReader/SHN/SpamerPenalty.cpp
// Auto-generated: one-file-per-SHN split for SpamerPenalty.shn
#include "SpamerPenalty.h"
#include "../../Shared/ShineLogSystem.h"

namespace fiesta {

SpamerPenaltyShn& SpamerPenaltyShn::Get() { static SpamerPenaltyShn s; return s; }

void SpamerPenaltyShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("SpamerPenalty");
    if (!t) { SHINELOG_WARN("SpamerPenalty.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        SpamerPenaltyRow rec;
        rec.uiPenaltyLv = (uint8)ShnGetU32(*t, _r, "PenaltyLv");
        rec.uiChatBlockTime = (uint16)ShnGetU32(*t, _r, "ChatBlockTime");
        rec.uiProbateTime = (uint16)ShnGetU32(*t, _r, "ProbateTime");
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("SpamerPenalty.shn: %u rows", (uint32)m_kRows.size());
}

} // namespace fiesta
