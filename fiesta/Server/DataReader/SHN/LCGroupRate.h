// Server/DataReader/SHN/LCGroupRate.h
// Auto-generated: one-file-per-SHN split for LCGroupRate.shn
#ifndef SHINE_DATAREADER_SHN_LCGROUPRATE_H
#define SHINE_DATAREADER_SHN_LCGROUPRATE_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace shine {

struct LCGroupRateRow {
    uint32           uiItem_ID;
    uint8            uiLCR_Group;
    uint32           uiLCR_Rate;
};

class LCGroupRateShn {
public:
    static LCGroupRateShn& Get();
    void Load();
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<LCGroupRateRow>& Rows() const { return m_kRows; }
private:
    std::vector<LCGroupRateRow>         m_kRows;
};

} // namespace shine
#endif // SHINE_DATAREADER_SHN_LCGROUPRATE_H
