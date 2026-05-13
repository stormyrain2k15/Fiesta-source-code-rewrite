// Server/DataReader/SHN/CollectCardStarRate.h
// Auto-generated: one-file-per-SHN split for CollectCardStarRate.shn
#ifndef SHINE_DATAREADER_SHN_COLLECTCARDSTARRATE_H
#define SHINE_DATAREADER_SHN_COLLECTCARDSTARRATE_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace shine {

struct CollectCardStarRateRow {
    uint32           uiCC_CardGradeType;
    uint8            uiCC_StarLot;
    uint16           uiCC_StarRate;
};

class CollectCardStarRateShn {
public:
    static CollectCardStarRateShn& Get();
    void Load();
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<CollectCardStarRateRow>& Rows() const { return m_kRows; }
private:
    std::vector<CollectCardStarRateRow>         m_kRows;
};

} // namespace shine
#endif // SHINE_DATAREADER_SHN_COLLECTCARDSTARRATE_H
