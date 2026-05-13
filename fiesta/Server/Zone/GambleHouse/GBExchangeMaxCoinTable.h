// Server/Zone/GambleHouse/GBExchangeMaxCoinTable.h
// FEATURE: casino-exchange -- per-character exchange cap from
// GBExchangeMaxCoin.shn. Two columns: Level, ExchangeMaxCoin. Caps the
// number of chips a character of given level may convert in one
// daily window. The level here is the gamble-house seasonal "exchange
// level", not character level; it's tracked per-character on the WM.
#ifndef SHINE_ZONE_GAMBLEHOUSE_GBEXCHANGEMAXCOINTABLE_H
#define SHINE_ZONE_GAMBLEHOUSE_GBEXCHANGEMAXCOINTABLE_H
#include "../../../Shared/ShineTypes.h"
#include <map>

namespace shine {

class GBExchangeMaxCoinTable {
public:
    static GBExchangeMaxCoinTable& Get();
    bool   Bind();
    // Returns the cap for the given exchange level. Zero on miss
    // (caller treats as "no exchange allowed").
    uint32 Cap(uint8 uiLevel) const;
    size_t Count() const { return m_kRows.size(); }

private:
    GBExchangeMaxCoinTable() {}
    std::map<uint8, uint32> m_kRows;
};

} // namespace shine
#endif
