// Server/DataReader/SHN/GuildAcademy.cpp
// Auto-generated: one-file-per-SHN split for GuildAcademy.shn
#include "GuildAcademy.h"
#include "../../Shared/ShineLogSystem.h"

namespace shine {

GuildAcademyShn& GuildAcademyShn::Get() { static GuildAcademyShn s; return s; }

void GuildAcademyShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("GuildAcademy");
    if (!t) { SHINELOG_WARN("GuildAcademy.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        GuildAcademyRow rec;
        rec.kBuffName = ShnGetStr(*t, _r, "BuffName");
        rec.uiLeastJoinTime = ShnGetU32(*t, _r, "LeastJoinTime");
        rec.iRankAggregationTime = (int8)ShnGetI32(*t, _r, "RankAggregationTime");
        rec.iUnkCol3 = (int8)ShnGetI32(*t, _r, "UnkCol3");
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("GuildAcademy.shn: %u rows", (uint32)m_kRows.size());
}

} // namespace shine
