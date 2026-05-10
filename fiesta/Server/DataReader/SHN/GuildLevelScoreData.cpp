// Server/DataReader/SHN/GuildLevelScoreData.cpp
// Auto-generated: one-file-per-SHN split for GuildLevelScoreData.shn
#include "GuildLevelScoreData.h"
#include "../../Shared/ShineLogSystem.h"

namespace fiesta {

GuildLevelScoreDataShn& GuildLevelScoreDataShn::Get() { static GuildLevelScoreDataShn s; return s; }

void GuildLevelScoreDataShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("GuildLevelScoreData");
    if (!t) { SHINELOG_WARN("GuildLevelScoreData.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        GuildLevelScoreDataRow rec;
        rec.uiCheckLevel = (uint16)ShnGetU32(*t, _r, "CheckLevel");
        rec.uiAddLevelScore = (uint16)ShnGetU32(*t, _r, "AddLevelScore");
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("GuildLevelScoreData.shn: %u rows", (uint32)m_kRows.size());
}

} // namespace fiesta
