// Server/DataReader/SHN/DiceGame.h
// Auto-generated: one-file-per-SHN split for DiceGame.shn
#ifndef FIESTA_DATAREADER_SHN_DICEGAME_H
#define FIESTA_DATAREADER_SHN_DICEGAME_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace fiesta {

struct DiceGameRow {
    uint16           uiItemID;
    uint16           uiUseMinLv;
    uint16           uiGetSysRate;
    uint16           uiGetMasterRate;
    uint32           uiMinGetMoney;
    uint32           uiMaxBetMoney;
    uint16           uiCastTime;
    uint16           uiDelayTime;
    int32            iWinCode;
    int32            iUnkCol9;
    int32            iUnkCol10;
    int32            iLoseCode;
    int32            iUnkCol12;
    int32            iUnkCol13;
};

class DiceGameShn {
public:
    static DiceGameShn& Get();
    void Load();
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<DiceGameRow>& Rows() const { return m_kRows; }
private:
    std::vector<DiceGameRow>         m_kRows;
};

} // namespace fiesta
#endif // FIESTA_DATAREADER_SHN_DICEGAME_H
