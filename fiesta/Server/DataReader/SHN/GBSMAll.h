// Server/DataReader/SHN/GBSMAll.h
// Auto-generated: one-file-per-SHN split for GBSMAll.shn
#ifndef FIESTA_DATAREADER_SHN_GBSMALL_H
#define FIESTA_DATAREADER_SHN_GBSMALL_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace fiesta {

struct GBSMAllRow {
    uint8            uiGBSM_GroupID;
    uint8            uiGBSM_Num;
    uint16           uiGBSM_RatioAll;
    uint8            uiGBSM_IsJP;
};

class GBSMAllShn {
public:
    static GBSMAllShn& Get();
    void Load();
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<GBSMAllRow>& Rows() const { return m_kRows; }
private:
    std::vector<GBSMAllRow>         m_kRows;
};

} // namespace fiesta
#endif // FIESTA_DATAREADER_SHN_GBSMALL_H
