// Server/DataReader/SHN/GuildTournamentReward.cpp
// Auto-generated: one-file-per-SHN split for GuildTournamentReward.shn
#include "GuildTournamentReward.h"
#include "../../Shared/ShineLogSystem.h"

namespace fiesta {

GuildTournamentRewardShn& GuildTournamentRewardShn::Get() { static GuildTournamentRewardShn s; return s; }

void GuildTournamentRewardShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("GuildTournamentReward");
    if (!t) { SHINELOG_WARN("GuildTournamentReward.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        GuildTournamentRewardRow rec;
        rec.uiRank = (uint8)ShnGetU32(*t, _r, "Rank");
        rec.uiRewardGroup = (uint8)ShnGetU32(*t, _r, "RewardGroup");
        rec.uiRewardType = ShnGetU32(*t, _r, "RewardType");
        rec.uiValue1 = ShnGetU32(*t, _r, "Value1");
        rec.uiValue2 = ShnGetU32(*t, _r, "Value2");
        rec.uiValue3 = ShnGetU32(*t, _r, "Value3");
        rec.uiIO_Str = ShnGetU32(*t, _r, "IO_Str");
        rec.uiIO_Con = ShnGetU32(*t, _r, "IO_Con");
        rec.uiIO_Dex = ShnGetU32(*t, _r, "IO_Dex");
        rec.uiIO_Int = ShnGetU32(*t, _r, "IO_Int");
        rec.uiIO_Men = ShnGetU32(*t, _r, "IO_Men");
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("GuildTournamentReward.shn: %u rows", (uint32)m_kRows.size());
}

} // namespace fiesta
