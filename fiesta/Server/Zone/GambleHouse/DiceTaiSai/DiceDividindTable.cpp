// Server/Zone/GambleHouse/DiceTaiSai/DiceDividindTable.cpp
// FEATURE: casino-dice
#include "DiceDividindTable.h"
#include "../../../DataReader/ShnRegistry.h"
#include "../../../Shared/ShineLogSystem.h"
#include <stdio.h>

namespace shine {

DiceDividindTable& DiceDividindTable::Get() {
    static DiceDividindTable s; return s;
}

DiceDividindTable::DiceDividindTable() : m_bLoaded(false) {
    // Sane fallback tiers (per-100 multipliers): 100, 100, 800, 1500,
    // 2400, 5000, 3000, 1800, 1200, 800, 600, 500, 100, 200, 100, 100.
    // Mirrors the NA2016 sample row from DiceDividind.shn so the bowl
    // is at least playable when the SHN is missing.
    static const int32 kFallback[16] = {
        100, 100, 800, 15000, 2400, 5000, 3000, 1800,
        1200, 800, 600, 500, 100, 200, 100, 100
    };
    for (int i = 0; i < 16; ++i) m_aTiers[i] = kFallback[i];
}

bool DiceDividindTable::Bind() {
    // FEATURE: casino-dice -- column read: DividendRate, UnkCol1..14
    const ShnFile* t = ShnRegistry::Get().GetTable("DiceDividind");
    if (!t || t->Rows().empty()) {
        SHINELOG_WARN("DiceDividind.shn missing -- using fallback ladder");
        return false;
    }
    const size_t r = 0;
    m_aTiers[0]  = (int32)ShnGetU32(*t, r, "DividendRate");
    char name[16];
    for (int i = 1; i < 16; ++i) {
        // Column names are "UnkCol1".."UnkCol14" + (the 14th likely
        // wraps); the data dictionary has 16 columns total. Read by
        // index name; missing columns harmlessly leave the fallback.
        sprintf(name, "UnkCol%d", i);
        int32 v = (int32)ShnGetI32(*t, r, name);
        if (v != 0) m_aTiers[i] = v;
    }
    m_bLoaded = true;
    SHINELOG_INFO("DiceDividind: ladder loaded (head=%d, tail=%d)",
                  m_aTiers[0], m_aTiers[15]);
    return true;
}

int32 DiceDividindTable::Tier(uint8 uiTier) const {
    if (uiTier >= 16) uiTier = 15;
    return m_aTiers[uiTier];
}

} // namespace shine
