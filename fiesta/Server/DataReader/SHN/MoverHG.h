// Server/DataReader/SHN/MoverHG.h
// Auto-generated: one-file-per-SHN split for MoverHG.shn
#ifndef SHINE_DATAREADER_SHN_MOVERHG_H
#define SHINE_DATAREADER_SHN_MOVERHG_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace shine {

struct MoverHGRow {
    std::string      kMoverIDX;
    std::string      kFeedType;
    uint16           uiRestoreAmount;
    uint16           uiMaxHG;
    uint16           uiCreateHG;
    uint16           uiTick;
    uint16           uiConsumeHG;
};

class MoverHGShn {
public:
    static MoverHGShn& Get();
    void Load();
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<MoverHGRow>& Rows() const { return m_kRows; }
private:
    std::vector<MoverHGRow>         m_kRows;
};

} // namespace shine
#endif // SHINE_DATAREADER_SHN_MOVERHG_H
