// Server/DataReader/SHN/GuildAcademyRank.cpp
// Auto-generated: one-file-per-SHN split for GuildAcademyRank.shn
#include "GuildAcademyRank.h"
#include "../../Shared/ShineLogSystem.h"

namespace fiesta {

GuildAcademyRankShn& GuildAcademyRankShn::Get() { static GuildAcademyRankShn s; return s; }

void GuildAcademyRankShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("GuildAcademyRank");
    if (!t) { SHINELOG_WARN("GuildAcademyRank.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        GuildAcademyRankRow rec;
        rec.iRank = (int8)ShnGetI32(*t, _r, "Rank");
        rec.iUnkCol1 = (int8)ShnGetI32(*t, _r, "UnkCol1");
        rec.uiFame = ShnGetU32(*t, _r, "Fame");
        rec.kBuffName = ShnGetStr(*t, _r, "BuffName");
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("GuildAcademyRank.shn: %u rows", (uint32)m_kRows.size());
}

} // namespace fiesta
