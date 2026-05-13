// Server/DataReader/SHN/TermExtendMatch.cpp
// Auto-generated: one-file-per-SHN split for TermExtendMatch.shn
#include "TermExtendMatch.h"
#include "../../Shared/ShineLogSystem.h"

namespace shine {

TermExtendMatchShn& TermExtendMatchShn::Get() { static TermExtendMatchShn s; return s; }

void TermExtendMatchShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("TermExtendMatch");
    if (!t) { SHINELOG_WARN("TermExtendMatch.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        TermExtendMatchRow rec;
        rec.kExtendItemIDX = ShnGetStr(*t, _r, "ExtendItemIDX");
        rec.kTermItemIDX = ShnGetStr(*t, _r, "TermItemIDX");
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("TermExtendMatch.shn: %u rows", (uint32)m_kRows.size());
}

} // namespace shine
