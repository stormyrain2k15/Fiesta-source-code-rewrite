// Server/Zone/GambleHouse/GBReward.cpp
// FEATURE: casino-reward
#include "GBAuxTables.h"
#include "../../DataReader/ShnRegistry.h"
#include "../../Shared/ShineLogSystem.h"
namespace fiesta {
GBRewardTable& GBRewardTable::Get() { static GBRewardTable s; return s; }
bool GBRewardTable::Bind() {
    m_kRows.clear();
    // FEATURE: casino-reward -- column read: GameType, RewardType, Item_INX
    const ShnFile* t = ShnRegistry::Get().GetTable("GBReward");
    if (!t) { SHINELOG_WARN("GBReward.shn missing"); return false; }
    for (size_t i = 0; i < t->Rows().size(); ++i) {
        LegacyGBRewardRow r;
        r.uiGameType   = ShnGetU32(*t, i, "GameType");
        r.uiRewardType = ShnGetU32(*t, i, "RewardType");
        r.kItemInx     = ShnGetStr(*t, i, "Item_INX");
        if (r.kItemInx.empty()) continue;
        m_kRows.push_back(r);
    }
    SHINELOG_INFO("GBReward: %u rows", (uint32)m_kRows.size());
    return !m_kRows.empty();
}
void GBRewardTable::FindByGameType(uint32 uiGT,
                                   std::vector<LegacyGBRewardRow>& rOut) const {
    rOut.clear();
    for (size_t i = 0; i < m_kRows.size(); ++i)
        if (m_kRows[i].uiGameType == uiGT) rOut.push_back(m_kRows[i]);
}
} // namespace fiesta
