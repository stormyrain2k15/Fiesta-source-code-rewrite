// Server/Zone/ScriptLoader.cpp
#include "ScriptLoader.h"
#include "../../Lua/LuaRuntime.h"
#include "../Shared/ShineLogSystem.h"
#include <stdio.h>
#include <windows.h>

namespace fiesta {

// ---- Canonical names taken from the project owner's data archive listing.
//      Confirmed present in the Shine.zip drop (LuaScript/KQ/* and LuaScript/ID/*).
static const char* kKQNames[] = {
    "AntiHenis", "EmperorSlime", "GoldHill", "HMiniDragon",
    "KDArena", "KDCake", "KDEgg", "KDFargels", "KDMine",
    "KDSoccer", "KDSoccer_W", "KDSpring", "KDWater",
    "KingSlime", "Kingkong", "LegendOfBijou",
    "MaraPirate", "MiniDragon"
};
static const char* kInstanceNames[] = {
    "AdlF", "AdlFH", "Bla", "CrystalCH", "CrystalCastle",
    "GraveYard", "GraveYardH", "IyzelTower", "IyzelTowerH",
    "Leviathan", "LeviathanH", "SD_Vale01",
    "SecretLab", "SecretLabH", "Siren", "SirenH",
    "WarBL", "WarBLH", "WarH", "WarHH",
    "WarL", "WarLH", "WarN", "WarNH"
};

const std::vector<std::string>& ScriptLoader::KnownKQNames() {
    static std::vector<std::string> v;
    if (v.empty()) for (size_t i = 0; i < sizeof(kKQNames)/sizeof(*kKQNames); ++i)
        v.push_back(kKQNames[i]);
    return v;
}
const std::vector<std::string>& ScriptLoader::KnownInstanceNames() {
    static std::vector<std::string> v;
    if (v.empty()) for (size_t i = 0; i < sizeof(kInstanceNames)/sizeof(*kInstanceNames); ++i)
        v.push_back(kInstanceNames[i]);
    return v;
}

// ---- Helpers ----
static std::string Join(const std::string& a, const char* b) {
    if (a.empty()) return b;
    char back = a[a.size()-1];
    if (back == '/' || back == '\\') return a + b;
    return a + "\\" + b;
}

static bool FileExists(const std::string& rPath) {
    DWORD a = GetFileAttributesA(rPath.c_str());
    return a != INVALID_FILE_ATTRIBUTES && !(a & FILE_ATTRIBUTE_DIRECTORY);
}

// Scan a directory for .lua files and load each in alphabetical order.
static size_t LoadDirLuaFiles(LuaRuntime& rL, const std::string& rDir) {
    WIN32_FIND_DATAA fd;
    std::string pat = rDir + "\\*.lua";
    HANDLE h = FindFirstFileA(pat.c_str(), &fd);
    if (h == INVALID_HANDLE_VALUE) return 0;
    std::vector<std::string> kFiles;
    do {
        if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) continue;
        kFiles.push_back(rDir + "\\" + fd.cFileName);
    } while (FindNextFileA(h, &fd));
    FindClose(h);
    // Sort for deterministic load order.
    for (size_t i = 1; i < kFiles.size(); ++i)
        for (size_t j = i; j > 0 && kFiles[j] < kFiles[j-1]; --j) std::swap(kFiles[j], kFiles[j-1]);
    size_t loaded = 0;
    for (size_t i = 0; i < kFiles.size(); ++i)
        if (rL.DoFile(kFiles[i].c_str())) ++loaded;
    return loaded;
}

// Per-event canonical load order: Data/*.lua, SubFunc.lua, Progress.lua, Routine.lua.
static size_t LoadEvent(LuaRuntime& rL, const std::string& rEventDir) {
    size_t n = 0;
    n += LoadDirLuaFiles(rL, rEventDir + "\\Data");
    const char* kFiles[3] = { "SubFunc.lua", "Progress.lua", "Routine.lua" };
    for (int i = 0; i < 3; ++i) {
        std::string p = Join(rEventDir, kFiles[i]);
        if (FileExists(p) && rL.DoFile(p.c_str())) ++n;
    }
    return n;
}

bool ScriptLoader::LoadCommon(LuaRuntime& rL, const std::string& rRoot) {
    std::string p = rRoot + "\\LuaScript\\common.lua";
    if (!FileExists(p)) return false;
    return rL.DoFile(p.c_str());
}

bool ScriptLoader::LoadKQ(LuaRuntime& rL, const std::string& rRoot, const std::string& rName) {
    std::string dir = rRoot + "\\LuaScript\\KQ\\" + rName;
    size_t n = LoadEvent(rL, dir);
    SHINELOG_INFO("KQ '%s' loaded %u Lua files", rName.c_str(), (uint32)n);
    return n > 0;
}

bool ScriptLoader::LoadInstance(LuaRuntime& rL, const std::string& rRoot, const std::string& rName) {
    std::string dir = rRoot + "\\LuaScript\\ID\\" + rName;
    size_t n = LoadEvent(rL, dir);
    SHINELOG_INFO("Instance '%s' loaded %u Lua files", rName.c_str(), (uint32)n);
    return n > 0;
}

bool ScriptLoader::LoadPromote(LuaRuntime& rL, const std::string& rRoot, const std::string& rPath) {
    // rPath e.g. "Job2_Forest" -> LuaScript/Promote/Job2_Forest/Functions/*.lua
    std::string dir = rRoot + "\\LuaScript\\Promote\\" + rPath + "\\Functions";
    if (!FileExists(dir + "\\Routine.lua")) return false;
    const char* kFiles[3] = { "SubFunc.lua", "Progress.lua", "Routine.lua" };
    size_t n = 0;
    for (int i = 0; i < 3; ++i) {
        std::string p = Join(dir, kFiles[i]);
        if (FileExists(p) && rL.DoFile(p.c_str())) ++n;
    }
    SHINELOG_INFO("Promote '%s' loaded %u Lua files", rPath.c_str(), (uint32)n);
    return n > 0;
}

size_t ScriptLoader::LoadAllKQ(LuaRuntime& rL, const std::string& rRoot) {
    size_t total = 0;
    const std::vector<std::string>& v = KnownKQNames();
    for (size_t i = 0; i < v.size(); ++i)
        if (LoadKQ(rL, rRoot, v[i])) ++total;
    return total;
}

size_t ScriptLoader::LoadAllInstances(LuaRuntime& rL, const std::string& rRoot) {
    size_t total = 0;
    const std::vector<std::string>& v = KnownInstanceNames();
    for (size_t i = 0; i < v.size(); ++i)
        if (LoadInstance(rL, rRoot, v[i])) ++total;
    return total;
}

bool ScriptLoader::LoadTutorial(LuaRuntime& rL, const std::string& rRoot) {
    // LuaScript/Tutorial -- one or more .lua files (no SubFunc/Progress/Routine
    // structure; flat).
    std::string dir = rRoot + "\\LuaScript\\Tutorial";
    size_t n = LoadDirLuaFiles(rL, dir);
    SHINELOG_INFO("Tutorial loaded %u Lua files", (uint32)n);
    return n > 0;
}

bool ScriptLoader::LoadPetSystem(LuaRuntime& rL, const std::string& rRoot) {
    std::string dir = rRoot + "\\LuaScript\\PetSystem";
    size_t n = LoadDirLuaFiles(rL, dir);
    SHINELOG_INFO("PetSystem loaded %u Lua files", (uint32)n);
    return n > 0;
}

size_t ScriptLoader::LoadAIScript(LuaRuntime& rL, const std::string& rRoot) {
    std::string dir = rRoot + "\\LuaScript\\AIScript";
    size_t n = LoadDirLuaFiles(rL, dir);
    SHINELOG_INFO("AIScript loaded %u Lua files", (uint32)n);
    return n;
}

} // namespace fiesta
