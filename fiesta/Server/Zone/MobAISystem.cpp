// Server/Zone/MobAISystem.cpp
#include "MobAISystem.h"
#include "ScriptLoader.h"
#include "MobBehaviorScript.h"
#include "../Lua/LuaRuntime.h"
#include "../Shared/ShineLogSystem.h"
#include <windows.h>

namespace fiesta {

MobAISystem& MobAISystem::Get() { static MobAISystem s; return s; }

static void EnumerateLuaAi(const std::string& rDir, std::map<std::string, MobAiDef>& rOut) {
    std::string pat = rDir + "\\*.lua";
    WIN32_FIND_DATAA fd; HANDLE h = FindFirstFileA(pat.c_str(), &fd);
    if (h == INVALID_HANDLE_VALUE) return;
    do {
        if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) continue;
        std::string name = fd.cFileName;
        size_t dot = name.find('.');
        if (dot != std::string::npos) name = name.substr(0, dot);
        MobAiDef d; d.kSpecies = name; d.eKind = AI_LUA;
        d.kLuaSym = "AI_" + name;
        rOut[name] = d;
    } while (FindNextFileA(h, &fd));
    FindClose(h);
}

static void EnumeratePsAi(const std::string& rDir, std::map<std::string, MobAiDef>& rOut) {
    std::string pat = rDir + "\\*.ps";
    WIN32_FIND_DATAA fd; HANDLE h = FindFirstFileA(pat.c_str(), &fd);
    if (h == INVALID_HANDLE_VALUE) return;
    do {
        if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) continue;
        std::string name = fd.cFileName;
        size_t dot = name.find('.');
        if (dot != std::string::npos) name = name.substr(0, dot);
        if (_stricmp(name.c_str(), "DefaultBehavior") == 0) continue;
        // Prefer Lua if it's already registered.
        if (rOut.find(name) == rOut.end()) {
            MobAiDef d; d.kSpecies = name; d.eKind = AI_PS; d.kPsBlock = "main";
            rOut[name] = d;
        }
    } while (FindNextFileA(h, &fd));
    FindClose(h);
}

size_t MobAISystem::LoadAll(LuaRuntime& rL, const std::string& rRoot) {
    m_kAll.clear();

    // 1. Load every Lua AI overlay (LuaScript/AIScript/*.lua).
    ScriptLoader::LoadAIScript(rL, rRoot);

    // 2. Walk the disk and register species names from both folders.
    EnumerateLuaAi(rRoot + "\\LuaScript\\AIScript",       m_kAll);
    EnumeratePsAi (rRoot + "\\MobBehaviorDescript",       m_kAll);
    // 3. Make sure the default PS is parsed so AI_DEFAULT can resolve.
    MobBehaviorBox::Get().LoadDefault(rRoot);

    SHINELOG_INFO("MobAISystem: %u species registered", (uint32)m_kAll.size());
    return m_kAll.size();
}

const MobAiDef* MobAISystem::Find(const std::string& rSpecies) const {
    std::map<std::string, MobAiDef>::const_iterator it = m_kAll.find(rSpecies);
    return (it == m_kAll.end()) ? NULL : &it->second;
}

void MobAISystem::RegisterKqOverride(const std::string& rSpecies, const std::string& rKq) {
    std::map<std::string, MobAiDef>::iterator it = m_kAll.find(rSpecies);
    if (it == m_kAll.end()) {
        MobAiDef d; d.kSpecies = rSpecies; d.eKind = AI_PS; d.kPsBlock = "main";
        d.kKqOverride = rKq;
        m_kAll[rSpecies] = d;
    } else {
        it->second.kKqOverride = rKq;
    }
}

eMobAiKind MobAISystem::ResolvedKind(const std::string& rSpecies) const {
    const MobAiDef* p = Find(rSpecies);
    return p ? p->eKind : AI_DEFAULT;
}

} // namespace fiesta
