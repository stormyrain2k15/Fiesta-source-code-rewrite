// Server/Zone/MobSpawnSystem.h
// 19 -- Per-zone mob spawn driver.
//
// Walks every loaded `MobRegenBox` table at boot, builds one
// `SpawnGroup` per `MobRegenGroup` row, and per-tick instantiates
// `MobNum` mobs of the referenced species inside the group's area.
// Killed mobs respawn after a `RegStandard` (+ aDelta/aSec curve) delay.
//
// EVIDENCE: PDB_CONFIRMED  symbol: MobSpawn, MobSpawnGroup
// EVIDENCE: DATA_CONFIRMED  source: 9Data\Shine\MobRegen\<Map>.txt
#ifndef FIESTA_ZONE_MOBSPAWNSYSTEM_H
#define FIESTA_ZONE_MOBSPAWNSYSTEM_H
#include "MobRegenTable.h"
#include "ShineObject.h"
#include <vector>
#include <string>

namespace fiesta {

struct MobSpawnGroup {
    std::string kGroupIndex;
    std::string kMobIndex;
    MobID       uiSpecies;       // resolved from MobTables::FindByInx
    MapID       uiMap;
    int32       iCenterX, iCenterY;
    int32       iHalfW,   iHalfH;
    uint8       uiTarget;        // MobNum: how many should be alive
    uint32      uiRespawnMs;     // base respawn delay
    std::vector<Handle> kAlive;  // currently alive mob handles
    uint64      uiNextRollMs;    // next planning tick
};

class MobSpawnSystem {
public:
    static MobSpawnSystem& Get();
    // Walk every MobRegenBox entry; resolve species via MobTables::FindByInx
    // and Map via MapTables::FindByName; populate the spawn-group list.
    size_t LoadAll();
    // Per-tick: top up any group below MobNum, restart respawn timers.
    void   Tick(uint64 uiNowMs);
    // Notification from Battle: a mob died -> remove it from the alive
    // list of its group and arm a respawn timer.
    void   OnMobDied(Handle uiMob);
    size_t GroupCount() const { return m_kGroups.size(); }
private:
    MobSpawnSystem() {}
    std::vector<MobSpawnGroup> m_kGroups;
    std::map<Handle, size_t>   m_kHandleToGroup;
};

} // namespace fiesta
#endif
