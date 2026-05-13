// Server/DataReader/SHN/GBSMBetCoin.h
// Auto-generated: one-file-per-SHN split for GBSMBetCoin.shn
#ifndef SHINE_DATAREADER_SHN_GBSMBETCOIN_H
#define SHINE_DATAREADER_SHN_GBSMBETCOIN_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace shine {

struct GBSMBetCoinRow {
    uint32           uiGBSMBet;
    uint16           uiGBSMCoin;
};

class GBSMBetCoinShn {
public:
    static GBSMBetCoinShn& Get();
    void Load();
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<GBSMBetCoinRow>& Rows() const { return m_kRows; }
private:
    std::vector<GBSMBetCoinRow>         m_kRows;
};

} // namespace shine
#endif // SHINE_DATAREADER_SHN_GBSMBETCOIN_H
