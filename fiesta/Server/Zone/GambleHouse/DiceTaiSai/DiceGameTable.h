// Server/Zone/GambleHouse/DiceTaiSai/DiceGameTable.h
// FEATURE: casino-dice -- per-dice-bowl tunables.
// DiceGame.shn (14 cols): per-row defines one dice "bowl" / table
// instance. Key columns: ItemID (the chip-token item id used at this
// bowl), UseMinLv, MinGetMoney, MaxBetMoney, GetSysRate, GetMasterRate,
// CastTime, DelayTime. 9 trailing animation/event-code columns
// (WinCode/LoseCode/UnkCol*) drive client visuals.
#ifndef SHINE_ZONE_GAMBLEHOUSE_DICETAISAI_DICEGAMETABLE_H
#define SHINE_ZONE_GAMBLEHOUSE_DICETAISAI_DICEGAMETABLE_H
#include "../../../../Shared/ShineTypes.h"
#include <vector>

namespace shine {

struct LegacyDiceGameRow {
    uint16 uiItemID;
    uint16 uiUseMinLv;
    uint16 uiGetSysRate;     // per-1000 house edge (system take)
    uint16 uiGetMasterRate;  // per-1000 house master cap
    uint32 uiMinGetMoney;
    uint32 uiMaxBetMoney;
    uint16 uiCastTime;       // ms; bet window
    uint16 uiDelayTime;      // ms; reveal delay
    int32  iWinCode;
    int32  iLoseCode;
    int32  aUnkCode[4];      // animation/event codes -- broadcast only
};

class DiceGameTable {
public:
    static DiceGameTable& Get();
    bool   Bind();
    const LegacyDiceGameRow* FindByItem(uint16 uiItemID) const;
    size_t Count() const { return m_kRows.size(); }

private:
    DiceGameTable() {}
    std::vector<LegacyDiceGameRow> m_kRows;
};

} // namespace shine
#endif
