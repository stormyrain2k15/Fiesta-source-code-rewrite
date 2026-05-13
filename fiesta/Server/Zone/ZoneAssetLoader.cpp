// Server/Zone/ZoneAssetLoader.cpp
#include "ZoneAssetLoader.h"
#include "AreaBMP.h"
#include "MobAttackSequence.h"
#include "MobBehaviorScript.h"
#include "MobRegenTable.h"
#include "MobRoamTable.h"
#include "MobSettingActionTable.h"
#include "NPCItemListTable.h"
#include "ScriptStringTable.h"
#include "ScriptLoader.h"
#include "MobAISystem.h"
#include "InstanceSystem.h"
#include "PromoteSystem.h"
#include "MapField.h"
#include "GroupTables.h"
#include "../DataReader/BlockInfoFile.h"
#include "../Shared/ShineLogSystem.h"
#include "../../Lua/LuaRuntime.h"
#include <windows.h>
#include <stdio.h>

namespace shine {

// =============================================================================
//  AbStateBinaryBox
// =============================================================================
AbStateBinaryBox& AbStateBinaryBox::Get() { static AbStateBinaryBox s; return s; }

bool AbStateBinaryBox::Load(const std::string& rRoot, const std::string& rFile) {
    std::string p = rRoot + "\\AbState\\" + rFile;
    FILE* fp = NULL; fopen_s(&fp, p.c_str(), "rb");
    if (!fp) return false;
    // Format observed:
    //   uint32 count
    //   { uint8 type, char[32] name (null-padded) } * count
    uint32 cnt = 0;
    if (fread(&cnt, 4, 1, fp) != 1) { fclose(fp); return false; }
    // Sanity guard against malformed file -- the largest .dat in this drop is
    // ~50 entries so anything > 4096 is bogus.
    if (cnt > 4096) { fclose(fp); return false; }
    for (uint32 i = 0; i < cnt; ++i) {
        uint8 type = 0;
        char  name[33];
        memset(name, 0, sizeof(name));
        if (fread(&type, 1, 1, fp) != 1)        break;
        if (fread(name, 32, 1, fp) != 1)        break;
        m_kIndex[std::string(name)] = true;
    }
    fclose(fp);
    return true;
}
bool AbStateBinaryBox::Has(const std::string& rIx) const {
    return m_kIndex.find(rIx) != m_kIndex.end();
}

// =============================================================================
//  Internal: directory walk (Win32 FindFirstFile).
// =============================================================================
namespace {

void Lower(std::string& s) {
    for (size_t i = 0; i < s.size(); ++i) s[i] = (char)tolower((unsigned char)s[i]);
}

bool EndsWith(const std::string& s, const char* sfx) {
    size_t n = strlen(sfx);
    if (s.size() < n) return false;
    for (size_t i = 0; i < n; ++i) {
        char a = (char)tolower((unsigned char)s[s.size() - n + i]);
        char b = (char)tolower((unsigned char)sfx[i]);
        if (a != b) return false;
    }
    return true;
}

// Walk a single folder for files matching `rPattern` (e.g. "*.txt") and call
// `pCb(basenameWithoutExt, basenameWithExt)` for each. Returns the count
// processed.
typedef void (*PerFileCb)(const std::string& rRoot,
                          const std::string& rNameNoExt,
                          const std::string& rNameWithExt,
                          void* pkUser);

size_t WalkFolder(const std::string& rRoot, const char* szSub, const char* szPattern,
                  PerFileCb pCb, void* pkUser) {
    std::string dir = rRoot + "\\" + szSub;
    std::string pat = dir + "\\" + szPattern;
    WIN32_FIND_DATAA fd;
    HANDLE h = FindFirstFileA(pat.c_str(), &fd);
    if (h == INVALID_HANDLE_VALUE) return 0;
    std::vector<std::string> kFiles;
    do {
        if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) continue;
        kFiles.push_back(fd.cFileName);
    } while (FindNextFileA(h, &fd));
    FindClose(h);
    // Stable alphabetical order.
    for (size_t i = 1; i < kFiles.size(); ++i)
        for (size_t j = i; j > 0 && kFiles[j] < kFiles[j-1]; --j)
            std::swap(kFiles[j], kFiles[j-1]);
    size_t n = 0;
    for (size_t i = 0; i < kFiles.size(); ++i) {
        std::string with    = kFiles[i];
        std::string noExt   = with;
        size_t dot = noExt.rfind('.');
        if (dot != std::string::npos) noExt = noExt.substr(0, dot);
        pCb(rRoot, noExt, with, pkUser);
        ++n;
    }
    return n;
}

// ----- Per-folder callback wrappers ----------------------------------------

void CbAbState(const std::string& rR, const std::string&, const std::string& rN, void*) {
    AbStateBinaryBox::Get().Load(rR, rN);
}
void CbAreaBMP(const std::string& rR, const std::string& rNoExt, const std::string&, void*) {
    AreaBMPBox::Get().Load(rR, rNoExt);
}
void CbBlockInfo(const std::string& rR, const std::string& rNoExt, const std::string& rN, void*) {
    std::string p = rR + "\\BlockInfo\\" + rN;
    if      (EndsWith(rN, ".shbd")) {
        BlockGrid g;
        if (LoadShbd(p, g)) {
            // Resolve the MapID from MapInfo.shn by the file's basename.
            // BlockInfo file names mirror Map.MapName (e.g. "Adl", "AdlF").
            const LegacyMapInfoRow* pkM = MapTables::Get().FindByName(rNoExt);
            if (pkM) {
                Field* pkF = MapDataBox::Get().GetField((MapID)pkM->uiID);
                if (pkF) {
                    pkF->Blocks().uiW = (uint16)g.uiWidth;
                    pkF->Blocks().uiH = (uint16)g.uiHeight;
                    pkF->Blocks().kBlocked.swap(g.kCells);
                }
            }
        }
    }
    else if (EndsWith(rN, ".aid"))  { AidFile  a; LoadAid (p, a); }
    else if (EndsWith(rN, ".sbi"))  { SbiFile  s; LoadSbi (p, s); }
}
void CbMobAtkSeq(const std::string& rR, const std::string& rNoExt, const std::string&, void*) {
    MobAttackSequenceBox::Get().Load(rR, rNoExt);
}
void CbMobBehavior(const std::string& rR, const std::string& rNoExt, const std::string&, void*) {
    // Default behaviour file lives in MobBehaviorDescript/<KQ or "Default">.txt;
    // the existing API only loads Default + LoadKQ. Wire by name: anything
    // other than "Default" goes through LoadKQ.
    if (rNoExt == "Default") MobBehaviorBox::Get().LoadDefault(rR);
    else                     MobBehaviorBox::Get().LoadKQ(rR, rNoExt);
}
void CbMobRegen(const std::string& rR, const std::string& rNoExt, const std::string&, void*) {
    MobRegenBox::Get().Load(rR, rNoExt);
}
void CbMobRoam(const std::string& rR, const std::string& rNoExt, const std::string&, void*) {
    MobRoamBox::Get().Load(rR, rNoExt);
}
void CbMobSetting(const std::string& rR, const std::string& rNoExt, const std::string&, void*) {
    MobSettingActionBox::Get().Load(rR, rNoExt);
}
void CbNpcItemList(const std::string& rR, const std::string& rNoExt, const std::string&, void*) {
    NPCItemListBox::Get().Load(rR, rNoExt);
}

} // namespace

// =============================================================================
//  Public walkers
// =============================================================================
size_t ZoneAssetLoader::WalkAbState(const std::string& rR) {
    return WalkFolder(rR, "AbState", "*.dat", &CbAbState, NULL);
}
size_t ZoneAssetLoader::WalkAreaBMP(const std::string& rR) {
    return WalkFolder(rR, "AreaBMP", "*.bmp", &CbAreaBMP, NULL);
}
size_t ZoneAssetLoader::WalkBlockInfo(const std::string& rR) {
    size_t n = 0;
    n += WalkFolder(rR, "BlockInfo", "*.shbd", &CbBlockInfo, NULL);
    n += WalkFolder(rR, "BlockInfo", "*.aid",  &CbBlockInfo, NULL);
    n += WalkFolder(rR, "BlockInfo", "*.sbi",  &CbBlockInfo, NULL);
    return n;
}
size_t ZoneAssetLoader::WalkMobAttackSeq(const std::string& rR) {
    return WalkFolder(rR, "MobAttackSequence", "*.txt", &CbMobAtkSeq, NULL);
}
size_t ZoneAssetLoader::WalkMobBehavior(const std::string& rR) {
    return WalkFolder(rR, "MobBehaviorDescript", "*.txt", &CbMobBehavior, NULL);
}
size_t ZoneAssetLoader::WalkMobRegen(const std::string& rR) {
    return WalkFolder(rR, "MobRegen", "*.txt", &CbMobRegen, NULL);
}
size_t ZoneAssetLoader::WalkMobRoam(const std::string& rR) {
    return WalkFolder(rR, "MobRoam", "*.txt", &CbMobRoam, NULL);
}
size_t ZoneAssetLoader::WalkMobSetting(const std::string& rR) {
    return WalkFolder(rR, "MobSetting\\Action", "*.txt", &CbMobSetting, NULL);
}
size_t ZoneAssetLoader::WalkNPCItemList(const std::string& rR) {
    return WalkFolder(rR, "NPCItemList", "*.txt", &CbNpcItemList, NULL);
}
size_t ZoneAssetLoader::WalkScript(const std::string& rR) {
    // ScriptStringBox::LoadAll itself walks Script/*.txt internally; here
    // we simply trigger it and report the count of files visible to the
    // ingest folder. The Box is keyed by index, not file, so the success
    // signal is a bool -- we mirror it to the file count for logging.
    if (!ScriptStringBox::Get().LoadAll(rR)) return 0;
    std::string names[256];
    return ListFiles(rR + "\\Script", "*.txt", names, 256);
}
size_t ZoneAssetLoader::WalkAllLua(LuaRuntime& rL, const std::string& rR) {
    size_t n = 0;
    if (ScriptLoader::LoadCommon  (rL, rR))     ++n;
    n += ScriptLoader::LoadAIScript(rL, rR);
    if (ScriptLoader::LoadPetSystem(rL, rR))    ++n;
    if (ScriptLoader::LoadTutorial (rL, rR))    ++n;
    n += ScriptLoader::LoadAllKQ      (rL, rR);
    n += ScriptLoader::LoadAllInstances(rL, rR);
    // Promote: the data drop contains Job2_Forest and Job2_Gamb.
    static const char* kPromoteNames[] = { "Job2_Forest", "Job2_Gamb" };
    for (size_t i = 0; i < sizeof(kPromoteNames)/sizeof(*kPromoteNames); ++i)
        if (ScriptLoader::LoadPromote(rL, rR, kPromoteNames[i])) ++n;
    // Hand the now-populated Lua VM to MobAISystem so it can register
    // every per-species AI overlay and its PS-fallback companion.
    n += MobAISystem::Get().LoadAll(rL, rR);
    // InstanceSystem and PromoteSystem own their own registries on top of
    // the Lua VM; the script files are already loaded above, this just
    // creates the lookup index so runtime callers can dispatch by name.
    n += InstanceSystem::Get().LoadAll(rL, rR);
    n += PromoteSystem ::Get().LoadAll(rL, rR);
    return n;
}

size_t ZoneAssetLoader::ListFiles(const std::string& rDir, const std::string& rPattern,
                                  std::string* paOut, size_t uiCap) {
    if (!paOut || uiCap == 0) return 0;
    std::string pat = rDir + "\\" + rPattern;
    WIN32_FIND_DATAA fd;
    HANDLE h = FindFirstFileA(pat.c_str(), &fd);
    if (h == INVALID_HANDLE_VALUE) return 0;
    size_t n = 0;
    do {
        if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) continue;
        if (n < uiCap) paOut[n++] = fd.cFileName;
    } while (FindNextFileA(h, &fd));
    FindClose(h);
    return n;
}

// =============================================================================
//  LoadAll
// =============================================================================
ZoneAssetCounts ZoneAssetLoader::LoadAll(const std::string& rRoot, LuaRuntime* pkLua) {
    ZoneAssetCounts c;
    c.uiAbState     = WalkAbState        (rRoot);
    c.uiAreaBMP     = WalkAreaBMP        (rRoot);
    c.uiBlockInfo   = WalkBlockInfo      (rRoot);
    c.uiMobAtkSeq   = WalkMobAttackSeq   (rRoot);
    c.uiMobBehavior = WalkMobBehavior    (rRoot);
    c.uiMobRegen    = WalkMobRegen       (rRoot);
    c.uiMobRoam     = WalkMobRoam        (rRoot);
    c.uiMobSetting  = WalkMobSetting     (rRoot);
    c.uiNpcItemList = WalkNPCItemList    (rRoot);
    c.uiScript      = WalkScript         (rRoot);
    c.uiLua         = pkLua ? WalkAllLua(*pkLua, rRoot) : 0;

    SHINELOG_INFO("ZoneAssets: AbState=%u AreaBMP=%u BlockInfo=%u "
                  "MobAtkSeq=%u MobBehavior=%u MobRegen=%u MobRoam=%u "
                  "MobSetting=%u NPCItemList=%u Script=%u Lua=%u",
                  (uint32)c.uiAbState,    (uint32)c.uiAreaBMP,    (uint32)c.uiBlockInfo,
                  (uint32)c.uiMobAtkSeq,  (uint32)c.uiMobBehavior,(uint32)c.uiMobRegen,
                  (uint32)c.uiMobRoam,    (uint32)c.uiMobSetting, (uint32)c.uiNpcItemList,
                  (uint32)c.uiScript,     (uint32)c.uiLua);
    return c;
}

} // namespace shine
