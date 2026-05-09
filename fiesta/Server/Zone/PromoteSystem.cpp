// Server/Zone/PromoteSystem.cpp
#include "PromoteSystem.h"
#include "ScriptLoader.h"
#include "ScenarioScript.h"
#include "../Lua/LuaRuntime.h"
#include "../Shared/ShineLogSystem.h"
#include <windows.h>

namespace fiesta {

PromoteSystem& PromoteSystem::Get() { static PromoteSystem s; return s; }

bool PromoteSystem::Load(LuaRuntime& rL, const std::string& rRoot, const std::string& rName) {
    PromoteDef d; d.kName = rName;
    std::string luaPath = rRoot + "\\LuaScript\\Promote\\" + rName + ".lua";
    d.bHasLua = ScriptLoader::LoadPromote(rL, rRoot, luaPath);
    d.bHasPs  = (ScenarioBookShelf::Get().Load(rRoot, "Promote", rName) != NULL);
    if (!d.bHasLua && !d.bHasPs) return false;
    m_kAll[rName] = d;
    return true;
}

static void EnumeratePromote(const std::string& rDir, const char* szExt, std::map<std::string, bool>& rNames) {
    std::string pat = rDir + "\\*" + szExt;
    WIN32_FIND_DATAA fd; HANDLE h = FindFirstFileA(pat.c_str(), &fd);
    if (h == INVALID_HANDLE_VALUE) return;
    do {
        if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) continue;
        std::string name = fd.cFileName;
        size_t dot = name.rfind('.');
        if (dot != std::string::npos) name = name.substr(0, dot);
        rNames[name] = true;
    } while (FindNextFileA(h, &fd));
    FindClose(h);
}

size_t PromoteSystem::LoadAll(LuaRuntime& rL, const std::string& rRoot) {
    m_kAll.clear();
    std::map<std::string, bool> names;
    EnumeratePromote(rRoot + "\\LuaScript\\Promote",         ".lua", names);
    EnumeratePromote(rRoot + "\\ScenarioBookShelf\\Promote", ".ps",  names);
    for (std::map<std::string, bool>::iterator it = names.begin(); it != names.end(); ++it)
        Load(rL, rRoot, it->first);
    SHINELOG_INFO("PromoteSystem: %u scenarios loaded", (uint32)m_kAll.size());
    return m_kAll.size();
}

const PromoteDef* PromoteSystem::Find(const std::string& rName) const {
    std::map<std::string, PromoteDef>::const_iterator it = m_kAll.find(rName);
    return (it == m_kAll.end()) ? NULL : &it->second;
}
bool PromoteSystem::ShouldUseLua(const std::string& rName) const {
    const PromoteDef* p = Find(rName);
    return (p && p->bHasLua);
}
const ScenarioScript* PromoteSystem::GetScenario(const std::string& rName) const {
    return ScenarioBookShelf::Get().Find("Promote/" + rName);
}

} // namespace fiesta
