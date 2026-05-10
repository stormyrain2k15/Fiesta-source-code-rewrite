// Server/DataReader/SHN/Gather.h
// Auto-generated: one-file-per-SHN split for Gather.shn
#ifndef FIESTA_DATAREADER_SHN_GATHER_H
#define FIESTA_DATAREADER_SHN_GATHER_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace fiesta {

struct GatherRow {
    uint16           uiGatherID;
    std::string      kIndex;
    uint32           uiType;
    std::string      kNeededTool0;
    std::string      kNeededTool1;
    std::string      kNeededTool2;
    std::string      kEqipItemView;
    uint32           uiAniNumber;
    uint32           uiGauge;
};

class GatherShn {
public:
    static GatherShn& Get();
    void Load();
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<GatherRow>& Rows() const { return m_kRows; }
private:
    std::vector<GatherRow>         m_kRows;
};

} // namespace fiesta
#endif // FIESTA_DATAREADER_SHN_GATHER_H
