// Server/Zone/SpamerPenaltyDataBox.cpp
// Per-offense spam penalty schedule -- 1st = 5min mute, 2nd = 30min,
// 3rd = 24h, 4th = perma + auto-report.
#include "../DataReader/ShnRegistry.h"
namespace fiesta { namespace {
class SpamerPenaltyDataBox {
public:
    static SpamerPenaltyDataBox& Get() { static SpamerPenaltyDataBox s; return s; }
    bool Load() { return ShnRegistry::Get().GetTable("SpamerPenalty") != NULL; }
    uint32 PenaltySecs(uint8 uiOffenseCount) const {
        switch (uiOffenseCount) { case 1: return 5*60; case 2: return 30*60;
                                   case 3: return 24*3600; default: return 0xFFFFFFFFu; }
    }
};
}} // anonymous
