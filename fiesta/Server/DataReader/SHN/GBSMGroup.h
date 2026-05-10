// Server/DataReader/SHN/GBSMGroup.h
// Auto-generated: one-file-per-SHN split for GBSMGroup.shn
#ifndef FIESTA_DATAREADER_SHN_GBSMGROUP_H
#define FIESTA_DATAREADER_SHN_GBSMGROUP_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace fiesta {

struct GBSMGroupRow {
    uint8            uiGBSM_GroupID;
    uint32           uiGBSMCard;
};

class GBSMGroupShn {
public:
    static GBSMGroupShn& Get();
    void Load();
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<GBSMGroupRow>& Rows() const { return m_kRows; }
private:
    std::vector<GBSMGroupRow>         m_kRows;
};

} // namespace fiesta
#endif // FIESTA_DATAREADER_SHN_GBSMGROUP_H
