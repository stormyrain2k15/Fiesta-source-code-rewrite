// Server/DataReader/SHN/LCReward.h
// Auto-generated: one-file-per-SHN split for LCReward.shn
#ifndef FIESTA_DATAREADER_SHN_LCREWARD_H
#define FIESTA_DATAREADER_SHN_LCREWARD_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace fiesta {

struct LCRewardRow {
    uint8            uiLCR_Group;
    std::string      kItem_Inx;
    uint8            uiLCR_Lot;
};

class LCRewardShn {
public:
    static LCRewardShn& Get();
    void Load();
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<LCRewardRow>& Rows() const { return m_kRows; }
private:
    std::vector<LCRewardRow>         m_kRows;
};

} // namespace fiesta
#endif // FIESTA_DATAREADER_SHN_LCREWARD_H
