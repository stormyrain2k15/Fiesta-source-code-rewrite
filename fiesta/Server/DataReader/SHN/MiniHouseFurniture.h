// Server/DataReader/SHN/MiniHouseFurniture.h
// Auto-generated: one-file-per-SHN split for MiniHouseFurniture.shn
#ifndef FIESTA_DATAREADER_SHN_MINIHOUSEFURNITURE_H
#define FIESTA_DATAREADER_SHN_MINIHOUSEFURNITURE_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace fiesta {

struct MiniHouseFurnitureRow {
    uint16           uiHandle;
    std::string      kItemID;
    std::string      kFurnitureType;
    std::string      kInvenType;
    uint32           uiGameType;
    uint8            uiCanSet;
    std::string      kBackimage;
    uint8            uiWALL;
    uint8            uiBOTTOM;
    uint8            uiCEILING;
    uint8            uiIsAnimation;
    uint16           uiWeight;
    uint16           uiKeepTime_Hour;
    uint16           uiKeepTime_Endure;
    uint8            uiGrip;
    uint8            uiMaxSlot;
    uint8            uiMHEmotionID;
};

class MiniHouseFurnitureShn {
public:
    static MiniHouseFurnitureShn& Get();
    void Load();
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<MiniHouseFurnitureRow>& Rows() const { return m_kRows; }
private:
    std::vector<MiniHouseFurnitureRow>         m_kRows;
};

} // namespace fiesta
#endif // FIESTA_DATAREADER_SHN_MINIHOUSEFURNITURE_H
