// Server/Zone/GambleHouse/GBHouseTable.h
// FEATURE: casino-house -- one binder for GBHouse.shn.
//
// GBHouse.shn (5 cols) holds the global gamble-house tunables:
//   GB_GameMoney       gold-per-chip exchange rate
//   GB_ExchangeTax     tax (per-100) on coin->gold returns
//   GB_ResetTimeHour   daily reset hour (server local)
//   GB_ResetTimeMin    daily reset minute
//   GB_ResetTimeSec    daily reset second
//
// One file per SHN; this is the per-table binder convention adopted
// 2026-02 (NA2016 canon).
#ifndef FIESTA_ZONE_GAMBLEHOUSE_GBHOUSETABLE_H
#define FIESTA_ZONE_GAMBLEHOUSE_GBHOUSETABLE_H
#include "../../../Shared/ShineTypes.h"

namespace fiesta {

struct LegacyGBHouseRow {
    uint32 uiGameMoney;     // gold cost per 1 chip (Lucky Coin)
    uint32 uiExchangeTax;   // % tax on coin->gold (per-100)
    uint8  uiResetH;
    uint8  uiResetM;
    uint8  uiResetS;
};

class GBHouseTable {
public:
    static GBHouseTable& Get();
    bool                 Bind();
    const LegacyGBHouseRow&    Row() const { return m_kRow; }
    bool                 IsLoaded() const { return m_bLoaded; }

private:
    GBHouseTable();
    LegacyGBHouseRow m_kRow;
    bool       m_bLoaded;
};

} // namespace fiesta
#endif
