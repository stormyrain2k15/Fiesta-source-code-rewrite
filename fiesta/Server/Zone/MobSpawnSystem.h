// Server/Zone/MobSpawnSystem.h
// Per-zone mob spawn driver.
// Walks every loaded `MobRegenBox` table at boot, builds one
// `SpawnGroup` per `MobRegenGroup` row, and per-tick instantiates
// `MobNum` mobs of the referenced species inside the group's area.
// Killed mobs respawn after a `RegStandard` (+ aDelta/aSec curve) delay.
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

    // -------------------------------------------------------------------
    //  Script-driven spawn entry points (PineScript / Lua bindings).
    //  Each one resolves the species through MobTables::FindByInx and
    //  registers the spawn in the unified ZoneServer registry. The
    //  returned handle is suitable for cNPCVanish / cObjectHP / etc.
    //  Spawns are NOT tracked under MobSpawnGroup, so they don't auto-
    //  respawn; the caller (KQ / instance script) owns the lifetime.
    // -------------------------------------------------------------------
    Handle SpawnAt       (const std::string& rMapInx, const std::string& rMobInx,
                          float fX, float fY);
    Handle SpawnRectangle(const std::string& rMapInx, const std::string& rMobInx,
                          float fX0, float fY0, float fX1, float fY1, uint32 uiCount);
    Handle SpawnCircle   (const std::string& rMapInx, const std::string& rMobInx,
                          float fCx, float fCy, float fRadius, uint32 uiCount);
    // Re-runs a configured MobRegen group on demand. Safe no-op when the
    // group name is unknown. Returns spawn count actually placed.
    uint32 SpawnGroup    (const std::string& rMapInx, const std::string& rGroupInx);
private:
    MobSpawnSystem() {}
    std::vector<MobSpawnGroup> m_kGroups;
    std::map<Handle, size_t>   m_kHandleToGroup;
};

} // namespace fiesta
#endif
