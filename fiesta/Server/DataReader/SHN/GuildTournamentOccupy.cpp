// Server/DataReader/SHN/GuildTournamentOccupy.cpp
// Auto-generated: one-file-per-SHN split for GuildTournamentOccupy.shn
#include "GuildTournamentOccupy.h"
#include "../../Shared/ShineLogSystem.h"

namespace shine {

GuildTournamentOccupyShn& GuildTournamentOccupyShn::Get() { static GuildTournamentOccupyShn s; return s; }

void GuildTournamentOccupyShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("GuildTournamentOccupy");
    if (!t) { SHINELOG_WARN("GuildTournamentOccupy.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        GuildTournamentOccupyRow rec;
        rec.uiMAP_TYPE = (uint16)ShnGetU32(*t, _r, "MAP_TYPE");
        rec.uiOccupyTime = (uint16)ShnGetU32(*t, _r, "OccupyTime");
        rec.uiScore = (uint16)ShnGetU32(*t, _r, "Score");
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("GuildTournamentOccupy.shn: %u rows", (uint32)m_kRows.size());
}

} // namespace shine
