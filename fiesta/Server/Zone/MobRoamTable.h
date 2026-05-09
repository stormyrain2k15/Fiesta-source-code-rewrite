// Server/Zone/MobRoamTable.h
// 19 -- MobRoam/<MapID>.txt -- patrol-path waypoints for "roaming" mobs.
// One Roaming table per file with ID/X/Y/EventIndex columns.
//
// Mob AI consumes this via the per-map MobAI tick: when a roamer reaches
// a waypoint, advance to (ID + 1) % count; if EventIndex != "-" the AI
// fires the named event (chat / mobregen / linkto / ...).
//
// EVIDENCE: DATA_CONFIRMED  source: project-owner-supplied MobRoam/.
#ifndef FIESTA_ZONE_MOBROAMTABLE_H
#define FIESTA_ZONE_MOBROAMTABLE_H
#include "../Shared/ShineTypes.h"
#include <map>
#include <string>
#include <vector>

namespace fiesta {

struct MobRoamPoint {
    uint16      uiID;
    uint32      uiX;
    uint32      uiY;
    std::string kEventIndex;     // "-" if none
};

class MobRoamMap {
public:
    bool Load(const std::string& rPath);
    const std::vector<MobRoamPoint>& Points() const { return m_kPoints; }
    const MobRoamPoint* Next(uint16 uiCurID) const;
private:
    std::vector<MobRoamPoint> m_kPoints;
};

class MobRoamBox {
public:
    static MobRoamBox& Get();
    const MobRoamMap* Load(const std::string& rRoot, const std::string& rMapID);
    const MobRoamMap* Find(const std::string& rMapID) const;
    void Clear();
private:
    std::map<std::string, MobRoamMap*> m_kAll;
};

} // namespace fiesta
#endif
