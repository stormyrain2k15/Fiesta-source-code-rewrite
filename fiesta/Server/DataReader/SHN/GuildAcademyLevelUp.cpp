// Server/DataReader/SHN/GuildAcademyLevelUp.cpp
// Auto-generated: one-file-per-SHN split for GuildAcademyLevelUp.shn
#include "GuildAcademyLevelUp.h"
#include "../../Shared/ShineLogSystem.h"

namespace fiesta {

GuildAcademyLevelUpShn& GuildAcademyLevelUpShn::Get() { static GuildAcademyLevelUpShn s; return s; }

void GuildAcademyLevelUpShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("GuildAcademyLevelUp");
    if (!t) { SHINELOG_WARN("GuildAcademyLevelUp.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        GuildAcademyLevelUpRow rec;
        rec.uiLevel = (uint8)ShnGetU32(*t, _r, "Level");
        rec.uiPoint = ShnGetU32(*t, _r, "Point");
        rec.uiBuffTime = ShnGetU32(*t, _r, "BuffTime");
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("GuildAcademyLevelUp.shn: %u rows", (uint32)m_kRows.size());
}

} // namespace fiesta
