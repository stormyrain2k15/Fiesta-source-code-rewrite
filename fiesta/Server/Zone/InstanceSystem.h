// Server/Zone/InstanceSystem.h
// 25 -- unified runtime for instance dungeons.
//
// One named instance is backed by **two source files** living in different
// folders -- both are read by this single system:
//
//   1. Lua side: LuaScript/ID/<Name>/<Name>.lua + Functions/{Progress,Routine,SubFunc}.lua
//   2. PS  side: ScenarioBookShelf/ID/<Name>.ps      (when present)
//
// At zone start the engine asks `InstanceSystem` to load every known
// instance name (the registry from `ScriptLoader::KnownInstanceNames()`).
// `Load(name)` calls both `ScriptLoader::LoadInstance` and
// `ScenarioBookShelf::Load("ID", name)`. Either can be missing for a
// given dungeon; the runtime treats the union as the authoritative
// definition.
//
// At ENTER time, the instance lifecycle picks Lua first (if its
// `Functions/Progress.lua` declares the standard hooks); PS scenario is
// the fallback / KQ-style scenario host.
//
// EVIDENCE: DATA_CONFIRMED  source: KingSlime + WarBL + IyzelTower
//                                   ship both .lua and .ps definitions.
#ifndef FIESTA_ZONE_INSTANCESYSTEM_H
#define FIESTA_ZONE_INSTANCESYSTEM_H
#include "../Shared/ShineTypes.h"
#include <map>
#include <string>
#include <vector>

namespace fiesta {

class LuaRuntime;
class ScenarioScript;

struct InstanceDef {
    std::string kName;
    bool        bHasLua;
    bool        bHasPs;
};

class InstanceSystem {
public:
    static InstanceSystem& Get();

    bool   Load(LuaRuntime& rL, const std::string& rRoot, const std::string& rName);
    size_t LoadAll(LuaRuntime& rL, const std::string& rRoot);
    const InstanceDef* Find(const std::string& rName) const;
    const std::vector<InstanceDef>& All() const { return m_kAll; }

    // Pick the runtime to drive when an instance "starts":
    //   bUseLua = true if Lua side present; the PS fallback is exposed via
    //   `GetScenario(name)` for code paths that need it.
    bool                        ShouldUseLua(const std::string& rName) const;
    const ScenarioScript*       GetScenario (const std::string& rName) const;

private:
    InstanceSystem() {}
    std::vector<InstanceDef>     m_kAll;
    std::map<std::string, size_t> m_kIndex;
};

} // namespace fiesta
#endif
