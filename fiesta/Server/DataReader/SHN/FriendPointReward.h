// Server/DataReader/SHN/FriendPointReward.h
// Auto-generated: one-file-per-SHN split for FriendPointReward.shn
#ifndef SHINE_DATAREADER_SHN_FRIENDPOINTREWARD_H
#define SHINE_DATAREADER_SHN_FRIENDPOINTREWARD_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace shine {

struct FriendPointRewardRow {
    std::string      kInxName;
    uint32           uiLot;
    uint32           uiFPR_Rate;
};

class FriendPointRewardShn {
public:
    static FriendPointRewardShn& Get();
    void Load();
    const FriendPointRewardRow* FindByInx(const std::string& rInx) const;
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<FriendPointRewardRow>& Rows() const { return m_kRows; }
private:
    std::vector<FriendPointRewardRow>         m_kRows;
    std::map<std::string, size_t>  m_kByInx;
};

} // namespace shine
#endif // SHINE_DATAREADER_SHN_FRIENDPOINTREWARD_H
