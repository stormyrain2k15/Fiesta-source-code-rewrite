// Server/DataReader/SHN/CollectCardDropRate.h
// Auto-generated: one-file-per-SHN split for CollectCardDropRate.shn
#ifndef SHINE_DATAREADER_SHN_COLLECTCARDDROPRATE_H
#define SHINE_DATAREADER_SHN_COLLECTCARDDROPRATE_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace shine {

struct CollectCardDropRateRow {
    uint16           uiCC_CardID;
    uint16           uiCC_CardGetRate;
};

class CollectCardDropRateShn {
public:
    static CollectCardDropRateShn& Get();
    void Load();
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<CollectCardDropRateRow>& Rows() const { return m_kRows; }
private:
    std::vector<CollectCardDropRateRow>         m_kRows;
};

} // namespace shine
#endif // SHINE_DATAREADER_SHN_COLLECTCARDDROPRATE_H
