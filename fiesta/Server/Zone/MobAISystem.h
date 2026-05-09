// Server/Zone/MobAISystem.h
// 19 -- unified mob AI resolver.
//
// One mob species can be driven by EITHER (or BOTH):
//   1. A Lua AI script: LuaScript/AIScript/<MobName>.lua
//   2. A PS behavior  : MobBehaviorDescript/[<KQ>/]<MobName>.ps
//
// Selection rule: Lua takes precedence when both exist (the live game
// uses Lua for the more sophisticated AIs and PS as the legacy /
// designer-friendly fallback). When neither exists the mob falls
// through to MobBehaviorDescript/DefaultBehavior.ps.
//
// At ENTER time a mob asks `MobAISystem::ResolveFor(species)` and
// receives a small descriptor that names which runtime to drive.
//
// EVIDENCE: DATA_CONFIRMED  source: LuaScript/AIScript/* (Defense.lua,
//                                   BH_Helga.lua) + MobBehaviorDescript/.
#ifndef FIESTA_ZONE_MOBAISYSTEM_H
#define FIESTA_ZONE_MOBAISYSTEM_H
#include "../Shared/ShineTypes.h"
#include <map>
#include <string>

namespace fiesta {

class LuaRuntime;
class MobBehaviorScript;

enum eMobAiKind {
    AI_NONE    = 0,
    AI_LUA     = 1,
    AI_PS      = 2,
    AI_DEFAULT = 3   // fall through to DefaultBehavior.ps
};

struct MobAiDef {
    std::string kSpecies;
    eMobAiKind  eKind;
    std::string kLuaSym;       // Lua entry function name ("AI_<Species>") if AI_LUA
    std::string kPsBlock;      // PS block name (typically "main") if AI_PS / AI_DEFAULT
    std::string kKqOverride;   // KQ name if PS lives under MobBehaviorDescript/KQ/
};

class MobAISystem {
public:
    static MobAISystem& Get();

    // Load every species we know about. Walks both folders and registers
    // every match found.
    size_t LoadAll(LuaRuntime& rL, const std::string& rRoot);

    // Per-species lookup. Returns AI_NONE if the species was never
    // registered (caller falls back to AI_DEFAULT).
    const MobAiDef* Find(const std::string& rSpecies) const;

    // KQ override: associates `species` with a KQ-specific PS file living
    // under MobBehaviorDescript/KQ/<KQ>.ps. Used by KQ scripts that
    // need a tweaked AI for the KQ duration.
    void RegisterKqOverride(const std::string& rSpecies, const std::string& rKqName);

    // Convenience: get the resolved kind, with the AI_DEFAULT fallback.
    eMobAiKind ResolvedKind(const std::string& rSpecies) const;

private:
    MobAISystem() {}
    std::map<std::string, MobAiDef> m_kAll;
};

} // namespace fiesta
#endif
