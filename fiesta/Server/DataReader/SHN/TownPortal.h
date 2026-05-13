// Server/DataReader/SHN/TownPortal.h
// Auto-generated: one-file-per-SHN split for TownPortal.shn
#ifndef SHINE_DATAREADER_SHN_TOWNPORTAL_H
#define SHINE_DATAREADER_SHN_TOWNPORTAL_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace shine {

struct TownPortalRow {
    uint8            uiIndex;
    uint8            uiMinLevel;
    uint8            uiTP_GroupNo;
    std::string      kMapName;
    uint32           uiX;
    uint32           uiY;
};

class TownPortalShn {
public:
    static TownPortalShn& Get();
    void Load();
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<TownPortalRow>& Rows() const { return m_kRows; }
private:
    std::vector<TownPortalRow>         m_kRows;
};

} // namespace shine
#endif // SHINE_DATAREADER_SHN_TOWNPORTAL_H
