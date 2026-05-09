// Server/Zone/PromoteSystem.h
// unified runtime for job-change scenarios. Same dual-source pattern
// as InstanceSystem:
//   * Lua side: LuaScript/Promote/<JobChange>.lua (registration / hooks)
//   * PS  side: ScenarioBookShelf/Promote/<JobChange>.ps (scenario flow)
//                                   LuaScript/Promote + ScenarioBookShelf/Promote.
#ifndef FIESTA_ZONE_PROMOTESYSTEM_H
#define FIESTA_ZONE_PROMOTESYSTEM_H
#include "../Shared/ShineTypes.h"
#include <map>
#include <string>

namespace fiesta {

class LuaRuntime;
class ScenarioScript;

struct PromoteDef {
    std::string kName;            // e.g. "JobChange1", "JobChange3_Fighter"
    bool        bHasLua;
    bool        bHasPs;
};

class PromoteSystem {
public:
    static PromoteSystem& Get();

    bool   Load(LuaRuntime& rL, const std::string& rRoot, const std::string& rName);
    // Walks ScenarioBookShelf/Promote/*.ps + LuaScript/Promote/*.lua and
    // registers everything found.
    size_t LoadAll(LuaRuntime& rL, const std::string& rRoot);

    const PromoteDef* Find(const std::string& rName) const;
    bool                ShouldUseLua(const std::string& rName) const;
    const ScenarioScript* GetScenario(const std::string& rName) const;

private:
    PromoteSystem() {}
    std::map<std::string, PromoteDef> m_kAll;
};

} // namespace fiesta
#endif
