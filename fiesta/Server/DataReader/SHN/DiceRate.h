// Server/DataReader/SHN/DiceRate.h
// Auto-generated: one-file-per-SHN split for DiceRate.shn
#ifndef SHINE_DATAREADER_SHN_DICERATE_H
#define SHINE_DATAREADER_SHN_DICERATE_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace shine {

struct DiceRateRow {
    uint32           uiRate;
};

class DiceRateShn {
public:
    static DiceRateShn& Get();
    void Load();
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<DiceRateRow>& Rows() const { return m_kRows; }
private:
    std::vector<DiceRateRow>         m_kRows;
};

} // namespace shine
#endif // SHINE_DATAREADER_SHN_DICERATE_H
