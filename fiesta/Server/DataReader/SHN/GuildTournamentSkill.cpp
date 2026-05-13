// Server/DataReader/SHN/GuildTournamentSkill.cpp
// Auto-generated: one-file-per-SHN split for GuildTournamentSkill.shn
#include "GuildTournamentSkill.h"
#include "../../Shared/ShineLogSystem.h"

namespace shine {

GuildTournamentSkillShn& GuildTournamentSkillShn::Get() { static GuildTournamentSkillShn s; return s; }

void GuildTournamentSkillShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("GuildTournamentSkill");
    if (!t) { SHINELOG_WARN("GuildTournamentSkill.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        GuildTournamentSkillRow rec;
        rec.uiMAP_TYPE = (uint16)ShnGetU32(*t, _r, "MAP_TYPE");
        rec.uiIndex = (uint16)ShnGetU32(*t, _r, "Index");
        rec.uiDeathPoint = (uint16)ShnGetU32(*t, _r, "DeathPoint");
        rec.kStaName = ShnGetStr(*t, _r, "StaName");
        rec.uiTargetType = ShnGetU32(*t, _r, "TargetType");
        rec.uiDlyTime = ShnGetU32(*t, _r, "DlyTime");
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("GuildTournamentSkill.shn: %u rows", (uint32)m_kRows.size());
}

} // namespace shine
