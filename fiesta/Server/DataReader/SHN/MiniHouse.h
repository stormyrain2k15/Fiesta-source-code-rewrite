// Server/DataReader/SHN/MiniHouse.h
// Auto-generated: one-file-per-SHN split for MiniHouse.shn
#ifndef FIESTA_DATAREADER_SHN_MINIHOUSE_H
#define FIESTA_DATAREADER_SHN_MINIHOUSE_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace fiesta {

struct MiniHouseRow {
    uint16           uiHandle;
    std::string      kItemID;
    std::string      kDummyType;
    std::string      kBackimage;
    uint16           uiKeepTime_Hour;
    uint16           uiHPTick;
    uint16           uiSPTick;
    uint16           uiHPRecovery;
    uint16           uiSPRecovery;
    uint16           uiCasting;
    uint8            uiSlot;
};

class MiniHouseShn {
public:
    static MiniHouseShn& Get();
    void Load();
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<MiniHouseRow>& Rows() const { return m_kRows; }
private:
    std::vector<MiniHouseRow>         m_kRows;
};

} // namespace fiesta
#endif // FIESTA_DATAREADER_SHN_MINIHOUSE_H
