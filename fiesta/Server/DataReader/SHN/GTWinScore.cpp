// Server/DataReader/SHN/GTWinScore.cpp
// Auto-generated: one-file-per-SHN split for GTWinScore.shn
#include "GTWinScore.h"
#include "../../Shared/ShineLogSystem.h"

namespace fiesta {

GTWinScoreShn& GTWinScoreShn::Get() { static GTWinScoreShn s; return s; }

void GTWinScoreShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("GTWinScore");
    if (!t) { SHINELOG_WARN("GTWinScore.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        GTWinScoreRow rec;
        rec.uiWinScore = (uint16)ShnGetU32(*t, _r, "WinScore");
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("GTWinScore.shn: %u rows", (uint32)m_kRows.size());
}

} // namespace fiesta
