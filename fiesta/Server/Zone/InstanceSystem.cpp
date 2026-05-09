// Server/Zone/InstanceSystem.cpp
#include "InstanceSystem.h"
#include "ScriptLoader.h"
#include "ScenarioScript.h"
#include "../Lua/LuaRuntime.h"
#include "../Shared/ShineLogSystem.h"

namespace fiesta {

InstanceSystem& InstanceSystem::Get() { static InstanceSystem s; return s; }

bool InstanceSystem::Load(LuaRuntime& rL, const std::string& rRoot, const std::string& rName) {
    InstanceDef d;
    d.kName  = rName;
    d.bHasLua = ScriptLoader::LoadInstance(rL, rRoot, rName);
    d.bHasPs  = (ScenarioBookShelf::Get().Load(rRoot, "ID", rName) != NULL);
    if (!d.bHasLua && !d.bHasPs) return false;
    m_kIndex[rName] = m_kAll.size();
    m_kAll.push_back(d);
    SHINELOG_INFO("Instance '%s' lua=%d ps=%d", rName.c_str(), (int)d.bHasLua, (int)d.bHasPs);
    return true;
}

size_t InstanceSystem::LoadAll(LuaRuntime& rL, const std::string& rRoot) {
    const std::vector<std::string>& v = ScriptLoader::KnownInstanceNames();
    size_t n = 0;
    for (size_t i = 0; i < v.size(); ++i) if (Load(rL, rRoot, v[i])) ++n;
    return n;
}

const InstanceDef* InstanceSystem::Find(const std::string& rName) const {
    std::map<std::string, size_t>::const_iterator it = m_kIndex.find(rName);
    return (it == m_kIndex.end()) ? NULL : &m_kAll[it->second];
}

bool InstanceSystem::ShouldUseLua(const std::string& rName) const {
    const InstanceDef* p = Find(rName);
    return (p && p->bHasLua);
}
const ScenarioScript* InstanceSystem::GetScenario(const std::string& rName) const {
    return ScenarioBookShelf::Get().Find("ID/" + rName);
}

} // namespace fiesta
