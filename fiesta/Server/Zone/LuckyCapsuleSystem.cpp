// Server/Zone/LuckyCapsuleSystem.cpp
// FEATURE: lucky-capsule
#include "LuckyCapsuleSystem.h"
#include "ShineObject.h"
#include "Inventory.h"
#include "GroupTables.h"
#include "../DataReader/ShnRegistry.h"
#include "../Shared/well512.h"
#include "../Shared/ShineLogSystem.h"

namespace shine {

static well512 s_kRng;

LuckyCapsuleSystem& LuckyCapsuleSystem::Get() {
    static LuckyCapsuleSystem s; return s;
}

bool LuckyCapsuleSystem::Load() {
    m_kGroupRates.clear();
    m_kRewards.clear();
    m_kRewardGroups.clear();

    // FEATURE: lucky-capsule -- column read: Item_ID, LCR_Group, LCR_Rate
    const ShnFile* tRate = ShnRegistry::Get().GetTable("LCGroupRate");
    if (tRate) {
        for (size_t i = 0; i < tRate->Rows().size(); ++i) {
            LCGroupRateEntry e;
            e.uiCapsuleItemId = ShnGetU32(*tRate, i, "Item_ID");
            e.uiGroup         = (uint8)ShnGetU32(*tRate, i, "LCR_Group");
            e.uiRate          = ShnGetU32(*tRate, i, "LCR_Rate");
            if (e.uiCapsuleItemId == 0) continue;
            m_kGroupRates.push_back(e);
        }
    } else {
        SHINELOG_WARN("LuckyCapsule: LCGroupRate.shn missing");
    }

    // FEATURE: lucky-capsule -- column read: LCR_Group, Item_Inx, LCR_Lot
    const ShnFile* tRew = ShnRegistry::Get().GetTable("LCReward");
    if (tRew) {
        for (size_t i = 0; i < tRew->Rows().size(); ++i) {
            LCRewardEntry r;
            r.kItemInx = ShnGetStr(*tRew, i, "Item_Inx");
            r.uiLot    = (uint8)ShnGetU32(*tRew, i, "LCR_Lot");
            uint8 g    = (uint8)ShnGetU32(*tRew, i, "LCR_Group");
            if (r.kItemInx.empty()) continue;
            if (r.uiLot == 0) r.uiLot = 1;
            m_kRewards.push_back(r);
            m_kRewardGroups.push_back(g);
        }
    } else {
        SHINELOG_WARN("LuckyCapsule: LCReward.shn missing");
    }

    SHINELOG_INFO("LuckyCapsule: %u group-rate rows, %u reward rows",
                  (uint32)m_kGroupRates.size(),
                  (uint32)m_kRewards.size());
    return !m_kGroupRates.empty() && !m_kRewards.empty();
}

void LuckyCapsuleSystem::GroupRatesFor(uint32 uiCapsuleId,
                                       std::vector<LCGroupRateEntry>& rOut) const
{
    rOut.clear();
    for (size_t i = 0; i < m_kGroupRates.size(); ++i) {
        if (m_kGroupRates[i].uiCapsuleItemId == uiCapsuleId)
            rOut.push_back(m_kGroupRates[i]);
    }
}

void LuckyCapsuleSystem::RewardsForGroup(uint8 uiGroup,
                                         std::vector<LCRewardEntry>& rOut) const
{
    rOut.clear();
    for (size_t i = 0; i < m_kRewards.size(); ++i) {
        if (m_kRewardGroups[i] == uiGroup) rOut.push_back(m_kRewards[i]);
    }
}

bool LuckyCapsuleSystem::IsCapsule(uint32 uiItemId) const {
    for (size_t i = 0; i < m_kGroupRates.size(); ++i) {
        if (m_kGroupRates[i].uiCapsuleItemId == uiItemId) return true;
    }
    return false;
}

bool LuckyCapsuleSystem::TryOpen(ShinePlayer* pk, uint32 uiItemId) const {
    if (!pk) return false;
    std::vector<LCGroupRateEntry> rates;
    GroupRatesFor(uiItemId, rates);
    if (rates.empty()) return false;

    // 1) Sum the rate weights, roll, pick a group.
    uint64 total = 0;
    for (size_t i = 0; i < rates.size(); ++i) total += rates[i].uiRate;
    if (total == 0) return false;
    uint64 roll = (uint64)(s_kRng.NextDouble() * (double)total);
    uint8 chosenGroup = 0;
    uint64 acc = 0;
    for (size_t i = 0; i < rates.size(); ++i) {
        acc += rates[i].uiRate;
        if (roll < acc) { chosenGroup = rates[i].uiGroup; break; }
    }

    // 2) Pick a uniform reward from that group.
    std::vector<LCRewardEntry> bag;
    RewardsForGroup(chosenGroup, bag);
    if (bag.empty()) {
        SHINELOG_WARN("LuckyCapsule: capsule=%u rolled empty group=%u",
                      uiItemId, chosenGroup);
        return false;
    }
    size_t idx = (size_t)(s_kRng.NextDouble() * (double)bag.size());
    if (idx >= bag.size()) idx = bag.size() - 1;
    const LCRewardEntry& reward = bag[idx];

    // 3) Resolve item inx-name -> ItemID and grant.
    const ItemInfoRow* pkInfo = ItemTables::Get().FindByInx(reward.kItemInx);
    if (!pkInfo) {
        SHINELOG_WARN("LuckyCapsule: reward inx '%s' unknown",
                      reward.kItemInx.c_str());
        return false;
    }
    ShineItem kNew;
    kNew.uiItemId  = (uint32)pkInfo->uiID;
    kNew.uiInxName = (ItemID) pkInfo->uiID;
    kNew.uiQty     = reward.uiLot;
    kNew.kItemIndex = reward.kItemInx;
    pk->Inv().Add(kNew);
    SHINELOG_INFO("LuckyCapsule: cid=%u capsule=%u group=%u -> %s x%u",
                  pk->GetCharID(), uiItemId, chosenGroup,
                  reward.kItemInx.c_str(), reward.uiLot);
    return true;
}

} // namespace shine
