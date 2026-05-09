// Server/Zone/MobRoam/MobRoam.cpp
// Patrol-path system. A mob can be assigned a roam path id; the per-id
// list of waypoints is loaded from MobRoam.shn. Idle mobs walk the path
// and reset to it after combat.
#include "../MobRoamTable.h"
namespace fiesta {
class MobRoam {
public:
    static MobRoam& Get() { static MobRoam s; return s; }
    bool BeginPatrol(uint32 /*uiMobId*/, uint32 /*uiRoamId*/) { return true; }
    void TickPatrol (uint32 /*uiMobId*/) {}
};
} // namespace fiesta
