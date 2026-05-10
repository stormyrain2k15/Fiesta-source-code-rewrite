// Server/DataReader/SHN/MiniHouseDummy.h
// Auto-generated: one-file-per-SHN split for MiniHouseDummy.shn
#ifndef FIESTA_DATAREADER_SHN_MINIHOUSEDUMMY_H
#define FIESTA_DATAREADER_SHN_MINIHOUSEDUMMY_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace fiesta {

struct MiniHouseDummyRow {
    uint16           uiNo;
    std::string      kIndex;
    std::string      kIconFileName;
    uint16           uiNIconNum;
    uint16           uiHPTick;
    uint16           uiSPTick;
    uint16           uiHPRecovery;
    uint16           uiSPRecovery;
    uint16           uiCasting;
    uint8            uiSlot;
    std::string      kHouseAType;
    uint32           uiHouseALoc;
    std::string      kHouseBType;
    uint32           uiHouseBLoc;
    std::string      kHouseCType;
    uint32           uiHouseCLoc;
};

class MiniHouseDummyShn {
public:
    static MiniHouseDummyShn& Get();
    void Load();
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<MiniHouseDummyRow>& Rows() const { return m_kRows; }
private:
    std::vector<MiniHouseDummyRow>         m_kRows;
};

} // namespace fiesta
#endif // FIESTA_DATAREADER_SHN_MINIHOUSEDUMMY_H
