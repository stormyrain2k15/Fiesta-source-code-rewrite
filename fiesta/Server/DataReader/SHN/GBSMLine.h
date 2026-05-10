// Server/DataReader/SHN/GBSMLine.h
// Auto-generated: one-file-per-SHN split for GBSMLine.shn
#ifndef FIESTA_DATAREADER_SHN_GBSMLINE_H
#define FIESTA_DATAREADER_SHN_GBSMLINE_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace fiesta {

struct GBSMLineRow {
    uint8            uiGBSM_GroupID;
    uint8            uiGBSM_Num;
    uint16           uiGBSM_RatioLine;
};

class GBSMLineShn {
public:
    static GBSMLineShn& Get();
    void Load();
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<GBSMLineRow>& Rows() const { return m_kRows; }
private:
    std::vector<GBSMLineRow>         m_kRows;
};

} // namespace fiesta
#endif // FIESTA_DATAREADER_SHN_GBSMLINE_H
