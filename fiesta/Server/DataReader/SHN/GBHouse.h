// Server/DataReader/SHN/GBHouse.h
// Auto-generated: one-file-per-SHN split for GBHouse.shn
#ifndef SHINE_DATAREADER_SHN_GBHOUSE_H
#define SHINE_DATAREADER_SHN_GBHOUSE_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace shine {

struct GBHouseRow {
    uint32           uiGB_GameMoney;
    uint32           uiGB_ExchangeTax;
    uint8            uiGB_ResetTimeHour;
    uint8            uiGB_ResetTimeMin;
    uint8            uiGB_ResetTimeSec;
};

class GBHouseShn {
public:
    static GBHouseShn& Get();
    void Load();
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<GBHouseRow>& Rows() const { return m_kRows; }
private:
    std::vector<GBHouseRow>         m_kRows;
};

} // namespace shine
#endif // SHINE_DATAREADER_SHN_GBHOUSE_H
