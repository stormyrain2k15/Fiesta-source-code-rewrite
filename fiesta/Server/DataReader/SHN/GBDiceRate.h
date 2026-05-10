// Server/DataReader/SHN/GBDiceRate.h
// Auto-generated: one-file-per-SHN split for GBDiceRate.shn
#ifndef FIESTA_DATAREADER_SHN_GBDICERATE_H
#define FIESTA_DATAREADER_SHN_GBDICERATE_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace fiesta {

struct GBDiceRateRow {
    uint32           uiRate;
};

class GBDiceRateShn {
public:
    static GBDiceRateShn& Get();
    void Load();
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<GBDiceRateRow>& Rows() const { return m_kRows; }
private:
    std::vector<GBDiceRateRow>         m_kRows;
};

} // namespace fiesta
#endif // FIESTA_DATAREADER_SHN_GBDICERATE_H
