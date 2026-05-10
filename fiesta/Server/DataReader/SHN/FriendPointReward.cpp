// Server/DataReader/SHN/FriendPointReward.cpp
// Auto-generated: one-file-per-SHN split for FriendPointReward.shn
#include "FriendPointReward.h"
#include "../../Shared/ShineLogSystem.h"

namespace fiesta {

FriendPointRewardShn& FriendPointRewardShn::Get() { static FriendPointRewardShn s; return s; }

void FriendPointRewardShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("FriendPointReward");
    if (!t) { SHINELOG_WARN("FriendPointReward.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        FriendPointRewardRow rec;
        rec.kInxName = ShnGetStr(*t, _r, "InxName");
        rec.uiLot = ShnGetU32(*t, _r, "Lot");
        rec.uiFPR_Rate = ShnGetU32(*t, _r, "FPR_Rate");
        m_kByInx[rec.kInxName] = m_kRows.size();
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("FriendPointReward.shn: %u rows", (uint32)m_kRows.size());
}

const FriendPointRewardRow* FriendPointRewardShn::FindByInx(const std::string& rInx) const {
    std::map<std::string, size_t>::const_iterator it = m_kByInx.find(rInx);
    return (it == m_kByInx.end()) ? NULL : &m_kRows[it->second];
}

} // namespace fiesta
