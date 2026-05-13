// Server/DataReader/SHN/ShineReward.h
// Auto-generated: one-file-per-SHN split for ShineReward.shn
#ifndef SHINE_DATAREADER_SHN_SHINEREWARD_H
#define SHINE_DATAREADER_SHN_SHINEREWARD_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace shine {

struct ShineRewardRow {
    uint16           uiRewardHandle;
    uint8            uiRewardType;
    std::string      kArgument;
    uint32           uiQuantity;
    int16            iUpgrade;
    int16            iUnkCol5;
    int16            iUnkCol6;
    int16            iUnkCol7;
    int16            iUnkCol8;
    int16            iUnkCol9;
    int16            iUnkCol10;
    int16            iUnkCol11;
    int16            iUnkCol12;
    int16            iUnkCol13;
    uint16           uiOptionDegree;
    uint32           uiTitleDegree;
};

class ShineRewardShn {
public:
    static ShineRewardShn& Get();
    void Load();
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<ShineRewardRow>& Rows() const { return m_kRows; }
private:
    std::vector<ShineRewardRow>         m_kRows;
};

} // namespace shine
#endif // SHINE_DATAREADER_SHN_SHINEREWARD_H
