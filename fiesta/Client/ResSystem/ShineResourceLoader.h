// Client/ResSystem/ShineResourceLoader.h
// Unified resource loader for the Shine client.
// Checks PE embedded resources first, falls back to disk only if not found.
// This is the single choke point all data loading goes through on the client.
//
// Load order:
//   1. PE resource section (embedded at build time via ShineResources.rc)
//   2. Disk path from ShineConfig (fallback for dev/debug builds only)
//
// In a shipping build the disk fallback should be disabled entirely by
// defining SHINE_EMBED_ONLY. Dev builds leave it on so you can iterate
// without rebuilding the exe every time.
//
// SHN resources:   type = "SHN",  name = stem (e.g. "ItemInfo")
// Action DAT:      type = "DAT",  name = InxName (e.g. "Weasel")
#ifndef SHINE_CLIENT_RESSYSTEM_SHINERESOURCELOADER_H
#define SHINE_CLIENT_RESSYSTEM_SHINERESOURCELOADER_H

#include "PEResourceReader.h"
#include "../../Server/Shared/ShineTypes.h"
#include <string>
#include <vector>

namespace shine {

class ShineResourceLoader {
public:
    static ShineResourceLoader& Get();

    // Load a .shine file by stem name.
    // Returns raw bytes into rOut. Checks PE first, then DataDir on disk.
    bool LoadShine(const std::string& rStem, std::vector<uint8>& rOut);

    // Load an action .dat file by InxName.
    // Returns raw bytes into rOut. Checks PE first, then ResAction on disk.
    bool LoadActionDat(const std::string& rInxName, std::vector<uint8>& rOut);

    // Check if a .shine is available (either PE or disk)
    bool ShineExists(const std::string& rStem) const;

    // Check if an action dat is available
    bool ActionDatExists(const std::string& rInxName) const;

    // Stats for boot log
    uint32 GetPEHits()   const { return m_uiPEHits; }
    uint32 GetDiskHits() const { return m_uiDiskHits; }
    uint32 GetMisses()   const { return m_uiMisses; }

private:
    ShineResourceLoader() : m_uiPEHits(0), m_uiDiskHits(0), m_uiMisses(0) {}

    bool LoadFromDisk(const std::string& rPath, std::vector<uint8>& rOut);

    uint32 m_uiPEHits;
    uint32 m_uiDiskHits;
    uint32 m_uiMisses;
};

} // namespace shine
#endif // SHINE_CLIENT_RESSYSTEM_SHINERESOURCELOADER_H
