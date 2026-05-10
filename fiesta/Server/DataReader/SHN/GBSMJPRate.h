// Server/DataReader/SHN/GBSMJPRate.h
// Auto-generated: one-file-per-SHN split for GBSMJPRate.shn
#ifndef FIESTA_DATAREADER_SHN_GBSMJPRATE_H
#define FIESTA_DATAREADER_SHN_GBSMJPRATE_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace fiesta {

struct GBSMJPRateRow {
    uint32           uiGBSM_MinCount;
    uint32           uiGBSM_MaxCount;
    uint32           uiGBSM_JPRate;
};

class GBSMJPRateShn {
public:
    static GBSMJPRateShn& Get();
    void Load();
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<GBSMJPRateRow>& Rows() const { return m_kRows; }
private:
    std::vector<GBSMJPRateRow>         m_kRows;
};

} // namespace fiesta
#endif // FIESTA_DATAREADER_SHN_GBSMJPRATE_H
