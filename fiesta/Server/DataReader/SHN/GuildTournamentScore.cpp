// Server/DataReader/SHN/GuildTournamentScore.cpp
// Auto-generated: one-file-per-SHN split for GuildTournamentScore.shn
#include "GuildTournamentScore.h"
#include "../../Shared/ShineLogSystem.h"

namespace shine {

GuildTournamentScoreShn& GuildTournamentScoreShn::Get() { static GuildTournamentScoreShn s; return s; }

void GuildTournamentScoreShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("GuildTournamentScore");
    if (!t) { SHINELOG_WARN("GuildTournamentScore.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        GuildTournamentScoreRow rec;
        rec.uiMAP_TYPE = (uint16)ShnGetU32(*t, _r, "MAP_TYPE");
        rec.iGradeScore = (int16)ShnGetI32(*t, _r, "GradeScore");
        rec.iUnkCol2 = (int16)ShnGetI32(*t, _r, "UnkCol2");
        rec.iUnkCol3 = (int16)ShnGetI32(*t, _r, "UnkCol3");
        rec.iUnkCol4 = (int16)ShnGetI32(*t, _r, "UnkCol4");
        rec.iUnkCol5 = (int16)ShnGetI32(*t, _r, "UnkCol5");
        rec.iUnkCol6 = (int16)ShnGetI32(*t, _r, "UnkCol6");
        rec.iUnkCol7 = (int16)ShnGetI32(*t, _r, "UnkCol7");
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("GuildTournamentScore.shn: %u rows", (uint32)m_kRows.size());
}

} // namespace shine
