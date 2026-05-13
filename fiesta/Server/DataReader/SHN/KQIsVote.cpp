// Server/DataReader/SHN/KQIsVote.cpp
// Auto-generated: one-file-per-SHN split for KQIsVote.shn
#include "KQIsVote.h"
#include "../../Shared/ShineLogSystem.h"

namespace shine {

KQIsVoteShn& KQIsVoteShn::Get() { static KQIsVoteShn s; return s; }

void KQIsVoteShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("KQIsVote");
    if (!t) { SHINELOG_WARN("KQIsVote.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        KQIsVoteRow rec;
        rec.iID = (int16)ShnGetI32(*t, _r, "ID");
        rec.uiIsVote = (uint8)ShnGetU32(*t, _r, "IsVote");
        m_kById[rec.iID] = m_kRows.size();
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("KQIsVote.shn: %u rows", (uint32)m_kRows.size());
}

const KQIsVoteRow* KQIsVoteShn::FindById(uint32 uiID) const {
    std::map<uint32, size_t>::const_iterator it = m_kById.find(uiID);
    return (it == m_kById.end()) ? NULL : &m_kRows[it->second];
}

} // namespace shine
