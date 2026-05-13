// Server/DataReader/SHN/GBTaxRate.h
// Auto-generated: one-file-per-SHN split for GBTaxRate.shn
#ifndef SHINE_DATAREADER_SHN_GBTAXRATE_H
#define SHINE_DATAREADER_SHN_GBTAXRATE_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace shine {

struct GBTaxRateRow {
    uint32           uiGameType;
    uint16           uiGB_TaxRate;
    uint32           uiGB_JPSave;
    uint16           uiGB_JPSaveRate;
};

class GBTaxRateShn {
public:
    static GBTaxRateShn& Get();
    void Load();
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<GBTaxRateRow>& Rows() const { return m_kRows; }
private:
    std::vector<GBTaxRateRow>         m_kRows;
};

} // namespace shine
#endif // SHINE_DATAREADER_SHN_GBTAXRATE_H
