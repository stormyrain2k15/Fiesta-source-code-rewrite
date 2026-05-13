// Server/DataReader/SHN/GuildTournamentRequire.cpp
// Auto-generated: one-file-per-SHN split for GuildTournamentRequire.shn
#include "GuildTournamentRequire.h"
#include "../../Shared/ShineLogSystem.h"

namespace shine {

GuildTournamentRequireShn& GuildTournamentRequireShn::Get() { static GuildTournamentRequireShn s; return s; }

void GuildTournamentRequireShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("GuildTournamentRequire");
    if (!t) { SHINELOG_WARN("GuildTournamentRequire.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        GuildTournamentRequireRow rec;
        rec.uiMinLv = (uint8)ShnGetU32(*t, _r, "MinLv");
        rec.uiMinMem = (uint16)ShnGetU32(*t, _r, "MinMem");
        rec.uiJoinMoney = ShnGetU32(*t, _r, "JoinMoney");
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("GuildTournamentRequire.shn: %u rows", (uint32)m_kRows.size());
}

} // namespace shine
