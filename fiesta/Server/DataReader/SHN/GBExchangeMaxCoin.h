// Server/DataReader/SHN/GBExchangeMaxCoin.h
// Auto-generated: one-file-per-SHN split for GBExchangeMaxCoin.shn
#ifndef SHINE_DATAREADER_SHN_GBEXCHANGEMAXCOIN_H
#define SHINE_DATAREADER_SHN_GBEXCHANGEMAXCOIN_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace shine {

struct GBExchangeMaxCoinRow {
    uint8            uiLevel;
    uint32           uiExchangeMaxCoin;
};

class GBExchangeMaxCoinShn {
public:
    static GBExchangeMaxCoinShn& Get();
    void Load();
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<GBExchangeMaxCoinRow>& Rows() const { return m_kRows; }
private:
    std::vector<GBExchangeMaxCoinRow>         m_kRows;
};

} // namespace shine
#endif // SHINE_DATAREADER_SHN_GBEXCHANGEMAXCOIN_H
