// Server/DataReader/SHN/BelongDice.h
// Auto-generated: one-file-per-SHN split for BelongDice.shn
#ifndef FIESTA_DATAREADER_SHN_BELONGDICE_H
#define FIESTA_DATAREADER_SHN_BELONGDICE_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace fiesta {

struct BelongDiceRow {
    uint8            uiDiceTimeLimit;
};

class BelongDiceShn {
public:
    static BelongDiceShn& Get();
    void Load();
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<BelongDiceRow>& Rows() const { return m_kRows; }
private:
    std::vector<BelongDiceRow>         m_kRows;
};

} // namespace fiesta
#endif // FIESTA_DATAREADER_SHN_BELONGDICE_H
