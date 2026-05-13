// Server/DataReader/SHN/GBDiceGame.h
// Auto-generated: one-file-per-SHN split for GBDiceGame.shn
#ifndef SHINE_DATAREADER_SHN_GBDICEGAME_H
#define SHINE_DATAREADER_SHN_GBDICEGAME_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace shine {

struct GBDiceGameRow {
    uint32           uiMaxBetMoney;
    uint16           uiCastTime;
    uint16           uiDelayTime;
};

class GBDiceGameShn {
public:
    static GBDiceGameShn& Get();
    void Load();
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<GBDiceGameRow>& Rows() const { return m_kRows; }
private:
    std::vector<GBDiceGameRow>         m_kRows;
};

} // namespace shine
#endif // SHINE_DATAREADER_SHN_GBDICEGAME_H
