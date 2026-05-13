// Server/Zone/ZoneAssetLoader.h
// 06+ -- Single boot-time pass that walks every per-folder data source under
// `Data\Shine\` and feeds each file to its existing parser / box.
// This was previously open-coded across half a dozen .cpp files (or worse,
// only loaded on first lookup). Centralising it gives us:
//   - One entry point in Main.cpp.
//   - Deterministic load order (alphabetical inside each folder).
//   - One clean log line per folder reporting "<count> files loaded".
//   - A single place to add new folder ingestion paths.
// Folders covered (with the parser each routes to):
//   AbState/*.dat                 -> AbStateBinaryBox        (binary descriptors)
//   AreaBMP/*.bmp                 -> AreaBMPBox              (1-bit AOE masks)
//   BlockInfo/*.{aid,sbi,shbd}    -> BlockInfoBox            (collision per map)
//   MobAttackSequence/*.txt       -> MobAttackSequenceBox    (attack pattern)
//   MobBehaviorDescript/*.txt     -> MobBehaviorScriptBox    (decision tree)
//   MobRegen/*.txt                -> MobRegenBox             (per-zone spawns)
//   MobRoam/*.txt                 -> MobRoamBox              (waypoints)
//   MobSetting/*.txt              -> MobSettingActionBox     (action table)
//   NPCItemList/*.txt             -> NPCItemListBox          (per-NPC merchant)
//   Script/*.txt                  -> ScriptStringBox         (KQ string scripts)
// Lua trees (LuaScript/{AIScript,KQ,ID,PetSystem,Promote,Tutorial}) are
// handed to ScriptLoader.
//                                       files and getting them being
//                                       used by the server" (2026-02 user note).
#ifndef SHINE_ZONE_ASSET_LOADER_H
#define SHINE_ZONE_ASSET_LOADER_H
#include "../Shared/ShineTypes.h"
#include <string>

namespace shine {

class LuaRuntime;

struct ZoneAssetCounts {
    size_t uiAbState;
    size_t uiAreaBMP;
    size_t uiBlockInfo;
    size_t uiMobAtkSeq;
    size_t uiMobBehavior;
    size_t uiMobRegen;
    size_t uiMobRoam;
    size_t uiMobSetting;
    size_t uiNpcItemList;
    size_t uiScript;
    size_t uiLua;
    ZoneAssetCounts()
        : uiAbState(0), uiAreaBMP(0), uiBlockInfo(0)
        , uiMobAtkSeq(0), uiMobBehavior(0), uiMobRegen(0), uiMobRoam(0)
        , uiMobSetting(0), uiNpcItemList(0), uiScript(0), uiLua(0) {}
};

class ZoneAssetLoader {
public:
    // Walk every folder under `rRoot` (which is `Data\Shine`) and load
    // its contents through the per-system box. Returns aggregated counts
    // for logging / diagnostics. Also pumps the shared `LuaRuntime` if
    // non-NULL.
    static ZoneAssetCounts LoadAll(const std::string& rRoot, LuaRuntime* pkLua);

    // Per-folder helpers (exposed mainly for tests / partial reload).
    static size_t WalkAbState        (const std::string& rRoot);
    static size_t WalkAreaBMP        (const std::string& rRoot);
    static size_t WalkBlockInfo      (const std::string& rRoot);
    static size_t WalkMobAttackSeq   (const std::string& rRoot);
    static size_t WalkMobBehavior    (const std::string& rRoot);
    static size_t WalkMobRegen       (const std::string& rRoot);
    static size_t WalkMobRoam        (const std::string& rRoot);
    static size_t WalkMobSetting     (const std::string& rRoot);
    static size_t WalkNPCItemList    (const std::string& rRoot);
    static size_t WalkScript         (const std::string& rRoot);
    static size_t WalkAllLua         (LuaRuntime& rL, const std::string& rRoot);

    // Convenience: scan `<rDir>\<rPattern>` and append filenames to rOut.
    // Each entry is the **basename** of the file, with the extension
    // intact. Skipped on FindFirstFile failure (returns 0).
    static size_t ListFiles(const std::string& rDir, const std::string& rPattern,
                            std::string* paOut, size_t uiCap);
};

// =============================================================================
// AbStateBinaryBox -- an in-memory cache for the binary AbState/*.dat
// files. Each file is a small length-prefixed blob (u32 count, then a
// `(u8 type, char[32] name)` per entry). The server only needs the name
// list to validate AbState references in skill rows; the per-frame
// rendering data is client-only.
// =============================================================================
class AbStateBinaryBox {
public:
    static AbStateBinaryBox& Get();
    bool Load(const std::string& rRoot, const std::string& rFileName);
    bool Has (const std::string& rIndexName) const;
    size_t Size() const { return m_kIndex.size(); }
private:
    AbStateBinaryBox() {}
    std::map<std::string, bool> m_kIndex;
};

} // namespace shine
#endif
