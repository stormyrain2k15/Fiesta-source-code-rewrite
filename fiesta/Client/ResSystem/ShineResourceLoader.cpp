// Client/ResSystem/ShineResourceLoader.cpp
#include "ShineResourceLoader.h"
#include "../Engine/ShineConfig.h"
#include "../../Server/Shared/ShineLogSystem.h"
#include <fstream>

namespace fiesta {

ShineResourceLoader& ShineResourceLoader::Get() {
    static ShineResourceLoader s;
    return s;
}

bool ShineResourceLoader::LoadShine(const std::string& rStem, std::vector<uint8>& rOut) {
    // 1. Check PE resource section
    DWORD dwSize = 0;
    const void* pData = PEResourceReader::Find(rStem.c_str(), "SHN", dwSize);
    if (pData && dwSize > 0) {
        rOut.assign((const uint8*)pData, (const uint8*)pData + dwSize);
        ++m_uiPEHits;
        SHINELOG_DEBUG("ShineResourceLoader: PE hit SHN '%s' (%u bytes)",
                       rStem.c_str(), dwSize);
        return true;
    }

#ifndef SHINE_EMBED_ONLY
    // 2. Disk fallback (dev/debug builds only)
    const ShineConfig& cfg = ShineConfig::Get();
    std::string kPath = cfg.kDataDir + "\\" + rStem + ".shine";
    if (LoadFromDisk(kPath, rOut)) {
        ++m_uiDiskHits;
        SHINELOG_DEBUG("ShineResourceLoader: disk hit SHN '%s'", rStem.c_str());
        return true;
    }
#endif

    ++m_uiMisses;
    SHINELOG_WARN("ShineResourceLoader: SHN '%s' not found (PE or disk)", rStem.c_str());
    return false;
}

bool ShineResourceLoader::LoadActionDat(const std::string& rInxName,
                                         std::vector<uint8>& rOut) {
    // 1. Check PE resource section
    DWORD dwSize = 0;
    const void* pData = PEResourceReader::Find(rInxName.c_str(), "DAT", dwSize);
    if (pData && dwSize > 0) {
        rOut.assign((const uint8*)pData, (const uint8*)pData + dwSize);
        ++m_uiPEHits;
        SHINELOG_DEBUG("ShineResourceLoader: PE hit DAT '%s' (%u bytes)",
                       rInxName.c_str(), dwSize);
        return true;
    }

#ifndef SHINE_EMBED_ONLY
    // 2. Disk fallback
    const ShineConfig& cfg = ShineConfig::Get();
    std::string kPath = cfg.kResAction + "\\" + rInxName + ".dat";
    if (LoadFromDisk(kPath, rOut)) {
        ++m_uiDiskHits;
        SHINELOG_DEBUG("ShineResourceLoader: disk hit DAT '%s'", rInxName.c_str());
        return true;
    }
#endif

    ++m_uiMisses;
    SHINELOG_WARN("ShineResourceLoader: DAT '%s' not found (PE or disk)", rInxName.c_str());
    return false;
}

bool ShineResourceLoader::ShineExists(const std::string& rStem) const {
    if (PEResourceReader::Exists(rStem.c_str(), "SHN")) return true;
#ifndef SHINE_EMBED_ONLY
    const ShineConfig& cfg = ShineConfig::Get();
    std::string kPath = cfg.kDataDir + "\\" + rStem + ".shine";
    return GetFileAttributesA(kPath.c_str()) != INVALID_FILE_ATTRIBUTES;
#else
    return false;
#endif
}

bool ShineResourceLoader::ActionDatExists(const std::string& rInxName) const {
    if (PEResourceReader::Exists(rInxName.c_str(), "DAT")) return true;
#ifndef SHINE_EMBED_ONLY
    const ShineConfig& cfg = ShineConfig::Get();
    std::string kPath = cfg.kResAction + "\\" + rInxName + ".dat";
    return GetFileAttributesA(kPath.c_str()) != INVALID_FILE_ATTRIBUTES;
#else
    return false;
#endif
}

bool ShineResourceLoader::LoadFromDisk(const std::string& rPath,
                                        std::vector<uint8>& rOut) {
    std::ifstream fin(rPath.c_str(), std::ios::binary);
    if (!fin.is_open()) return false;
    fin.seekg(0, std::ios::end);
    size_t uiSize = (size_t)fin.tellg();
    fin.seekg(0);
    if (uiSize == 0) return false;
    rOut.resize(uiSize);
    fin.read((char*)rOut.data(), (std::streamsize)uiSize);
    return fin.good() || fin.eof();
}

} // namespace fiesta
