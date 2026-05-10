// Server/DataReader/SHN/RareMoverRate.h
// Auto-generated: one-file-per-SHN split for RareMoverRate.shn
#ifndef FIESTA_DATAREADER_SHN_RAREMOVERRATE_H
#define FIESTA_DATAREADER_SHN_RAREMOVERRATE_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace fiesta {

struct RareMoverRateRow {
    uint16           uiRMR_Rate;
};

class RareMoverRateShn {
public:
    static RareMoverRateShn& Get();
    void Load();
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<RareMoverRateRow>& Rows() const { return m_kRows; }
private:
    std::vector<RareMoverRateRow>         m_kRows;
};

} // namespace fiesta
#endif // FIESTA_DATAREADER_SHN_RAREMOVERRATE_H
