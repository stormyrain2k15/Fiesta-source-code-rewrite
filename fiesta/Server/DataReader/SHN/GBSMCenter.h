// Server/DataReader/SHN/GBSMCenter.h
// Auto-generated: one-file-per-SHN split for GBSMCenter.shn
#ifndef SHINE_DATAREADER_SHN_GBSMCENTER_H
#define SHINE_DATAREADER_SHN_GBSMCENTER_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace shine {

struct GBSMCenterRow {
    uint8            uiGBSM_GroupID;
    uint16           uiGBSM_RatioCenter;
};

class GBSMCenterShn {
public:
    static GBSMCenterShn& Get();
    void Load();
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<GBSMCenterRow>& Rows() const { return m_kRows; }
private:
    std::vector<GBSMCenterRow>         m_kRows;
};

} // namespace shine
#endif // SHINE_DATAREADER_SHN_GBSMCENTER_H
