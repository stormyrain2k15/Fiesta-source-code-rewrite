// Server/DataReader/SHN/GBBanTime.h
// Auto-generated: one-file-per-SHN split for GBBanTime.shn
#ifndef SHINE_DATAREADER_SHN_GBBANTIME_H
#define SHINE_DATAREADER_SHN_GBBANTIME_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace shine {

struct GBBanTimeRow {
    uint32           uiGameType;
    uint32           uiGB_Ban;
    uint32           uiGB_BanTime;
};

class GBBanTimeShn {
public:
    static GBBanTimeShn& Get();
    void Load();
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<GBBanTimeRow>& Rows() const { return m_kRows; }
private:
    std::vector<GBBanTimeRow>         m_kRows;
};

} // namespace shine
#endif // SHINE_DATAREADER_SHN_GBBANTIME_H
