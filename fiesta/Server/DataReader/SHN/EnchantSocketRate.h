// Server/DataReader/SHN/EnchantSocketRate.h
// Auto-generated: one-file-per-SHN split for EnchantSocketRate.shn
#ifndef FIESTA_DATAREADER_SHN_ENCHANTSOCKETRATE_H
#define FIESTA_DATAREADER_SHN_ENCHANTSOCKETRATE_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace fiesta {

struct EnchantSocketRateRow {
    uint32           uiItemGradeType;
    uint16           uiSocket0;
    uint16           uiSocket1;
    uint16           uiSocket2;
};

class EnchantSocketRateShn {
public:
    static EnchantSocketRateShn& Get();
    void Load();
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<EnchantSocketRateRow>& Rows() const { return m_kRows; }
private:
    std::vector<EnchantSocketRateRow>         m_kRows;
};

} // namespace fiesta
#endif // FIESTA_DATAREADER_SHN_ENCHANTSOCKETRATE_H
