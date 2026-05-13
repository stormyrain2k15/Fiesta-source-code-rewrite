// Server/DataReader/SHN/CollectCardReward.cpp
// Auto-generated: one-file-per-SHN split for CollectCardReward.shn
#include "CollectCardReward.h"
#include "../../Shared/ShineLogSystem.h"

namespace shine {

CollectCardRewardShn& CollectCardRewardShn::Get() { static CollectCardRewardShn s; return s; }

void CollectCardRewardShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("CollectCardReward");
    if (!t) { SHINELOG_WARN("CollectCardReward.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        CollectCardRewardRow rec;
        rec.uiCC_RewardID = (uint16)ShnGetU32(*t, _r, "CC_RewardID");
        rec.uiCC_CardRewardType = ShnGetU32(*t, _r, "CC_CardRewardType");
        rec.uiCC_CardLot = (uint16)ShnGetU32(*t, _r, "CC_CardLot");
        rec.kCC_RewardItemInx = ShnGetStr(*t, _r, "CC_RewardItemInx");
        rec.uiCC_RewardLot = (uint16)ShnGetU32(*t, _r, "CC_RewardLot");
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("CollectCardReward.shn: %u rows", (uint32)m_kRows.size());
}

} // namespace shine
