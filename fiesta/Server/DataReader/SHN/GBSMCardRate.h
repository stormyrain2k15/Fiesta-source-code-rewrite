// Server/DataReader/SHN/GBSMCardRate.h
// Auto-generated: one-file-per-SHN split for GBSMCardRate.shn
#ifndef FIESTA_DATAREADER_SHN_GBSMCARDRATE_H
#define FIESTA_DATAREADER_SHN_GBSMCARDRATE_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace fiesta {

struct GBSMCardRateRow {
    uint32           uiGBSMCard;
    uint16           uiGBSM_CardRate;
};

class GBSMCardRateShn {
public:
    static GBSMCardRateShn& Get();
    void Load();
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<GBSMCardRateRow>& Rows() const { return m_kRows; }
private:
    std::vector<GBSMCardRateRow>         m_kRows;
};

} // namespace fiesta
#endif // FIESTA_DATAREADER_SHN_GBSMCARDRATE_H
