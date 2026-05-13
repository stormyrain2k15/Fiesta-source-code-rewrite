// Server/Zone/ScriptLoader.h
// scans the LuaScript/ tree shipped with the game data and loads each
// event's scripts into the Zone Lua VM in the canonical order:
//   1. Data/*.lua          (Name / Boss / Regen / Process / NPC / Chat / Stuff / ItemDrop / Servant / Monster)
//   2. SubFunc.lua
//   3. Progress.lua
//   4. Routine.lua
// The shipped tree (verified via the project owner's data archive listing):
//   LuaScript/common.lua
//   LuaScript/KQ/<KQName>/{Routine,Progress,SubFunc}.lua + Data/*.lua
//   LuaScript/ID/<InstanceName>/{Routine,Progress,SubFunc}.lua + Data/*.lua
//   LuaScript/Promote/Job2_Forest/Functions/{Routine,Progress,SubFunc}.lua
// Routine.lua is loaded last because it captures references to functions defined
// in the earlier files (Data tables + SubFunc helpers + Progress state).
#ifndef SHINE_ZONE_SCRIPTLOADER_H
#define SHINE_ZONE_SCRIPTLOADER_H
#include "../Shared/ShineTypes.h"
#include <string>
#include <vector>

namespace shine {

class LuaRuntime;

class ScriptLoader {
public:
    // szRoot points at the directory that *contains* "LuaScript" (e.g., "../9Data/Shine").
    static bool LoadCommon  (LuaRuntime& rL, const std::string& rRoot);
    static bool LoadKQ      (LuaRuntime& rL, const std::string& rRoot, const std::string& rKqName);
    static bool LoadInstance(LuaRuntime& rL, const std::string& rRoot, const std::string& rInstanceName);
    static bool LoadPromote (LuaRuntime& rL, const std::string& rRoot, const std::string& rPath);
    static bool LoadTutorial(LuaRuntime& rL, const std::string& rRoot);
    static bool LoadPetSystem(LuaRuntime& rL, const std::string& rRoot);
    // Loads every .lua under LuaScript/AIScript (NPC/event ai overlays).
    static size_t LoadAIScript(LuaRuntime& rL, const std::string& rRoot);

    // Convenience: load every event in a category that's present on disk.
    static size_t LoadAllKQ      (LuaRuntime& rL, const std::string& rRoot);
    static size_t LoadAllInstances(LuaRuntime& rL, const std::string& rRoot);

    // Canonical name registries from the supplied data manifest.
    static const std::vector<std::string>& KnownKQNames();
    static const std::vector<std::string>& KnownInstanceNames();
};

} // namespace shine
#endif
