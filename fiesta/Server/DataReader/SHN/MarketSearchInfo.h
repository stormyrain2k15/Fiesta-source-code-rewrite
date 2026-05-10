// Server/DataReader/SHN/MarketSearchInfo.h
// Auto-generated: one-file-per-SHN split for MarketSearchInfo.shn
#ifndef FIESTA_DATAREADER_SHN_MARKETSEARCHINFO_H
#define FIESTA_DATAREADER_SHN_MARKETSEARCHINFO_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace fiesta {

struct MarketSearchInfoRow {
    uint32           uiAuctionSubType;
    uint32           uiMarketSearchType;
};

class MarketSearchInfoShn {
public:
    static MarketSearchInfoShn& Get();
    void Load();
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<MarketSearchInfoRow>& Rows() const { return m_kRows; }
private:
    std::vector<MarketSearchInfoRow>         m_kRows;
};

} // namespace fiesta
#endif // FIESTA_DATAREADER_SHN_MARKETSEARCHINFO_H
