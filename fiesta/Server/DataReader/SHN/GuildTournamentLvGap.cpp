// Server/DataReader/SHN/GuildTournamentLvGap.cpp
// Auto-generated: one-file-per-SHN split for GuildTournamentLvGap.shn
#include "GuildTournamentLvGap.h"
#include "../../Shared/ShineLogSystem.h"

namespace shine {

GuildTournamentLvGapShn& GuildTournamentLvGapShn::Get() { static GuildTournamentLvGapShn s; return s; }

void GuildTournamentLvGapShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("GuildTournamentLvGap");
    if (!t) { SHINELOG_WARN("GuildTournamentLvGap.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        GuildTournamentLvGapRow rec;
        rec.uiMAP_TYPE = (uint16)ShnGetU32(*t, _r, "MAP_TYPE");
        rec.fLvGap = ShnGetF32(*t, _r, "LvGap");
        rec.uiPointRate = (uint16)ShnGetU32(*t, _r, "PointRate");
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("GuildTournamentLvGap.shn: %u rows", (uint32)m_kRows.size());
}

} // namespace shine
