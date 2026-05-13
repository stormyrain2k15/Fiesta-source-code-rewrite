// Server/Zone/Tables/TreasureRewardTable.h
// FEATURE: world-creation -- World/TreasureReward.txt declarations.
// Treasure-bag system. Two coupled tables in one file:
//   Container: declares a treasure-bag item (parent ItemID, CardInx,
//              MinLot..MaxLot of total picks per open).
//   Content:   per-bag reward bag (RewardInx, lot, upgrade range, rate).
// Sharing one .h because the two tables form one TreasureRewardTable
// surface with a single Get().
#ifndef SHINE_ZONE_TABLES_TREASUREREWARDTABLE_H
#define SHINE_ZONE_TABLES_TREASUREREWARDTABLE_H
#include "../../Shared/ShineTypes.h"
#include <map>
#include <string>
#include <vector>

namespace shine {

struct TreasureContainer {
    uint32      uiItemID;
    uint16      uiIndex;
    std::string kCardInx;
    uint16      uiMinLot;
    uint16      uiMaxLot;
    std::string kDummyInx;
};
struct TreasureContent {
    uint32      uiItemID;       // parent container ItemID
    uint16      uiIndex;
    std::string kRewardInx;
    uint16      uiRewardLot;
    uint8       uiUpgradeLow;
    uint8       uiUpgradeHigh;
    uint32      uiRewardRate;   // per-million
};

class TreasureRewardTable {
public:
    static TreasureRewardTable& Get();
    bool Load(const std::string& rRoot);
    const TreasureContainer* FindContainer(uint32 uiItemID) const;
    void ContentsForContainer(uint32 uiItemID,
                              std::vector<const TreasureContent*>& rOut) const;
private:
    TreasureRewardTable() {}
    std::vector<TreasureContainer> m_kContainers;
    std::map<uint32, size_t>        m_kCByItem;
    std::vector<TreasureContent>   m_kContents;
    std::map<uint32, std::vector<size_t> > m_kContentByItem;
};

} // namespace shine
#endif
