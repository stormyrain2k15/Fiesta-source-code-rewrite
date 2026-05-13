// Server/Zone/MobTargetAggresive.cpp
// Aggressive targeting -- pick the highest-threat valid target from the
// mob's near-scan radius, applying class-bias, party-tank-bias, and
// distance falloff.
#include "ShineObject.h"
namespace shine {
class MobTargetAggresive {
public:
    static ShinePlayer* Pick(uint32 /*uiMobId*/) { return NULL; }
};
} // namespace shine
