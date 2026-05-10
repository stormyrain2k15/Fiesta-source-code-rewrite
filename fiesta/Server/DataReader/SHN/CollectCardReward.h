// Server/DataReader/SHN/CollectCardReward.h
// Auto-generated: one-file-per-SHN split for CollectCardReward.shn
#ifndef FIESTA_DATAREADER_SHN_COLLECTCARDREWARD_H
#define FIESTA_DATAREADER_SHN_COLLECTCARDREWARD_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace fiesta {

struct CollectCardRewardRow {
    uint16           uiCC_RewardID;
    uint32           uiCC_CardRewardType;
    uint16           uiCC_CardLot;
    std::string      kCC_RewardItemInx;
    uint16           uiCC_RewardLot;
};

class CollectCardRewardShn {
public:
    static CollectCardRewardShn& Get();
    void Load();
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<CollectCardRewardRow>& Rows() const { return m_kRows; }
private:
    std::vector<CollectCardRewardRow>         m_kRows;
};

} // namespace fiesta
#endif // FIESTA_DATAREADER_SHN_COLLECTCARDREWARD_H
