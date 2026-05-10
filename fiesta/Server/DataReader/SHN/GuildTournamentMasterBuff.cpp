// Server/DataReader/SHN/GuildTournamentMasterBuff.cpp
// Auto-generated: one-file-per-SHN split for GuildTournamentMasterBuff.shn
#include "GuildTournamentMasterBuff.h"
#include "../../Shared/ShineLogSystem.h"

namespace fiesta {

GuildTournamentMasterBuffShn& GuildTournamentMasterBuffShn::Get() { static GuildTournamentMasterBuffShn s; return s; }

void GuildTournamentMasterBuffShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("GuildTournamentMasterBuff");
    if (!t) { SHINELOG_WARN("GuildTournamentMasterBuff.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        GuildTournamentMasterBuffRow rec;
        rec.uiMAP_TYPE = (uint16)ShnGetU32(*t, _r, "MAP_TYPE");
        rec.kStateName = ShnGetStr(*t, _r, "StateName");
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("GuildTournamentMasterBuff.shn: %u rows", (uint32)m_kRows.size());
}

} // namespace fiesta
