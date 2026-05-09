// Server/Zone/LevelGapDamageTable.cpp
// Per-(level diff) damage modifier curve. Canonical-named alias for the
// per-table accessor; the actual data is held in LevelGapTable.
#include "LevelGapTable.h"
namespace fiesta {
class LevelGapDamageTable {
public:
    static int32 PvE(int32 a, int32 d) { return LevelGapTable::Get().GetMultiplier(a, d, LVGAP_PVE); }
    static int32 PvP(int32 a, int32 d) { return LevelGapTable::Get().GetMultiplier(a, d, LVGAP_PVP); }
    static int32 EvP(int32 a, int32 d) { return LevelGapTable::Get().GetMultiplier(a, d, LVGAP_EVP); }
};
} // namespace fiesta

