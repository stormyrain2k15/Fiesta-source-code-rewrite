// Server/Zone/GambleHouse/GBTaxRateTable.h
// FEATURE: casino-tax -- per-game-type tax + JP-jackpot save rate.
// GBTaxRate.shn (4 cols): GameType, GB_TaxRate, GB_JPSave, GB_JPSaveRate.
// GB_JPSave=1 routes some of the bet into the slot machine's
// progressive jackpot pool at the GB_JPSaveRate (per-100).
#ifndef FIESTA_ZONE_GAMBLEHOUSE_GBTAXRATETABLE_H
#define FIESTA_ZONE_GAMBLEHOUSE_GBTAXRATETABLE_H
#include "../../../Shared/ShineTypes.h"
#include <map>

namespace fiesta {

struct GBTaxRow {
    uint16 uiTaxRate;       // per-100
    uint8  bJPSave;
    uint16 uiJPSaveRate;    // per-100
};

class GBTaxRateTable {
public:
    static GBTaxRateTable& Get();
    bool                   Bind();
    bool                   FindByGameType(uint32 uiGameType, GBTaxRow& rOut) const;

private:
    GBTaxRateTable() {}
    std::map<uint32, GBTaxRow> m_kRows;
};

} // namespace fiesta
#endif
