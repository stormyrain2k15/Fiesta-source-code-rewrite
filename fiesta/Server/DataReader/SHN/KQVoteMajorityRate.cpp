// Server/DataReader/SHN/KQVoteMajorityRate.cpp
// Auto-generated: one-file-per-SHN split for KQVoteMajorityRate.shn
#include "KQVoteMajorityRate.h"
#include "../../Shared/ShineLogSystem.h"

namespace shine {

KQVoteMajorityRateShn& KQVoteMajorityRateShn::Get() { static KQVoteMajorityRateShn s; return s; }

void KQVoteMajorityRateShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("KQVoteMajorityRate");
    if (!t) { SHINELOG_WARN("KQVoteMajorityRate.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        KQVoteMajorityRateRow rec;
        rec.uiVoteAgreeRate = (uint8)ShnGetU32(*t, _r, "VoteAgreeRate");
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("KQVoteMajorityRate.shn: %u rows", (uint32)m_kRows.size());
}

} // namespace shine
