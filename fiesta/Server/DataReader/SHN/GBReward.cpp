// Server/DataReader/SHN/GBReward.cpp
// Auto-generated: one-file-per-SHN split for GBReward.shn
#include "GBReward.h"
#include "../../Shared/ShineLogSystem.h"

namespace fiesta {

GBRewardShn& GBRewardShn::Get() { static GBRewardShn s; return s; }

void GBRewardShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("GBReward");
    if (!t) { SHINELOG_WARN("GBReward.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        GBRewardRow rec;
        rec.uiGameType = ShnGetU32(*t, _r, "GameType");
        rec.uiRewardType = ShnGetU32(*t, _r, "RewardType");
        rec.kItem_INX = ShnGetStr(*t, _r, "Item_INX");
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("GBReward.shn: %u rows", (uint32)m_kRows.size());
}

} // namespace fiesta
