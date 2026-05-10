// Server/DataReader/SHN/RareMoverSubRate.h
// Auto-generated: one-file-per-SHN split for RareMoverSubRate.shn
#ifndef FIESTA_DATAREADER_SHN_RAREMOVERSUBRATE_H
#define FIESTA_DATAREADER_SHN_RAREMOVERSUBRATE_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace fiesta {

struct RareMoverSubRateRow {
    uint16           uiRMR_SubRate;
};

class RareMoverSubRateShn {
public:
    static RareMoverSubRateShn& Get();
    void Load();
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<RareMoverSubRateRow>& Rows() const { return m_kRows; }
private:
    std::vector<RareMoverSubRateRow>         m_kRows;
};

} // namespace fiesta
#endif // FIESTA_DATAREADER_SHN_RAREMOVERSUBRATE_H
