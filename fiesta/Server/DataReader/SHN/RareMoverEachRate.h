// Server/DataReader/SHN/RareMoverEachRate.h
// Auto-generated: one-file-per-SHN split for RareMoverEachRate.shn
#ifndef FIESTA_DATAREADER_SHN_RAREMOVEREACHRATE_H
#define FIESTA_DATAREADER_SHN_RAREMOVEREACHRATE_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace fiesta {

struct RareMoverEachRateRow {
    std::string      kRMER_ItemIDX;
    uint16           uiRMER_Rate;
};

class RareMoverEachRateShn {
public:
    static RareMoverEachRateShn& Get();
    void Load();
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<RareMoverEachRateRow>& Rows() const { return m_kRows; }
private:
    std::vector<RareMoverEachRateRow>         m_kRows;
};

} // namespace fiesta
#endif // FIESTA_DATAREADER_SHN_RAREMOVEREACHRATE_H
