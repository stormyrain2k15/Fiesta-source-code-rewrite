// Server/Zone/Tables/DamageBySoulTable.cpp
// FEATURE: world-creation -- World/DamageBySoul.txt binder.
// 8x8 demand-soul x current-soul matrix. The diagonal returns 1000
// (1.0x); off-diagonal cells are the per-1000 damage bonus when the
// attacker's soul mismatches the recipe's demand soul.
#include "../WorldTables.h"
#include "../../DataReader/TableScriptFile.h"
#include "../../Shared/ShineLogSystem.h"
#include <stdio.h>

namespace fiesta {

DamageBySoulTable& DamageBySoulTable::Get() { static DamageBySoulTable s; return s; }

bool DamageBySoulTable::Load(const std::string& rRoot) {
    for (int i = 0; i < 8; ++i) for (int j = 0; j < 8; ++j) m_aMatrix[i][j] = 1000;
    TableScriptFile f;
    if (!f.Load(rRoot + "\\World\\DamageBySoul.txt")) return false;
    const TsTable* t = f.Find("DamageBySoul"); if (!t) return false;
    // FEATURE: world-creation -- column read: DemandSoul, Soul00..Soul07
    for (size_t r = 0; r < t->kRecords.size(); ++r) {
        int d = (int)t->GetInt(r, "DemandSoul");
        if (d < 0 || d >= 8) continue;
        char buf[12];
        for (int k = 0; k < 8; ++k) {
            sprintf_s(buf, sizeof(buf), "Soul%02d", k);
            int32 v = (int32)t->GetInt(r, buf, 1000);
            m_aMatrix[d][k] = v;
        }
    }
    return true;
}

int32 DamageBySoulTable::BonusX1k(uint8 dem, uint8 cnt) const {
    if (dem >= 8 || cnt >= 8) return 1000;
    return m_aMatrix[dem][cnt];
}

} // namespace fiesta
