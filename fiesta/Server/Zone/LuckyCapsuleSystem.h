// Server/Zone/LuckyCapsuleSystem.h
// FEATURE: lucky-capsule -- buy-and-open random-item capsules.
//
// Three capsule colors ship in NA2016 (red / blue / yellow). Each is
// just a different item-id; the rolls are driven by two SHN tables:
//
//   LCGroupRate.shn  (Item_ID, LCR_Group, LCR_Rate)
//     - Per-capsule probability over reward groups. Rates are per-100k.
//
//   LCReward.shn     (LCR_Group, Item_Inx, LCR_Lot)
//     - Per-group reward bag. LCR_Lot is the quantity to grant when
//       this entry rolls. Multiple rows in the same group are sampled
//       uniformly at the end of the roll (canon Fiesta behavior:
//       group decides quality, then a random item from the group is
//       picked as the actual prize).
//
// Hook: ZoneHandlers H_ItemUse calls LuckyCapsuleSystem::TryOpen on the
// item-use packet path; if it returns true the item was consumed.
#ifndef FIESTA_ZONE_LUCKYCAPSULESYSTEM_H
#define FIESTA_ZONE_LUCKYCAPSULESYSTEM_H
#include "../Shared/ShineTypes.h"
#include <string>
#include <vector>

namespace fiesta {
class ShinePlayer;

struct LCRewardEntry {
    std::string kItemInx;
    uint8       uiLot;
};
struct LCGroupRateEntry {
    uint32 uiCapsuleItemId;     // matches ItemInfo.shn ID column
    uint8  uiGroup;
    uint32 uiRate;              // per-100k weight
};

class LuckyCapsuleSystem {
public:
    static LuckyCapsuleSystem& Get();
    bool Load();

    // Returns true if the item is a known capsule (i.e. has any rows in
    // LCGroupRate). Keeps the use-handler dispatch cheap.
    bool IsCapsule(uint32 uiItemId) const;

    // Roll once for the given capsule item id and grant the reward to
    // the player. Returns true on success (item consumed by caller).
    bool TryOpen(ShinePlayer* pk, uint32 uiItemId) const;

    size_t GroupRateCount() const { return m_kGroupRates.size(); }
    size_t RewardCount()    const { return m_kRewards.size(); }

private:
    LuckyCapsuleSystem() {}
    void   GroupRatesFor(uint32 uiCapsuleId,
                         std::vector<LCGroupRateEntry>& rOut) const;
    void   RewardsForGroup(uint8 uiGroup,
                           std::vector<LCRewardEntry>& rOut) const;

    std::vector<LCGroupRateEntry> m_kGroupRates;
    std::vector<LCRewardEntry>    m_kRewards;        // (group, item, lot)
    std::vector<uint8>            m_kRewardGroups;   // parallel to m_kRewards
};

} // namespace fiesta
#endif
