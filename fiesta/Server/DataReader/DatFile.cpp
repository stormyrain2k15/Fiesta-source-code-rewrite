// Server/DataReader/DatFile.cpp
#include "DatFile.h"
#include "../Shared/ShineLogSystem.h"
#include <stdio.h>
#include <string.h>

namespace fiesta {

DatFile::DatFile() : m_eKind(DAT_KIND_ACTION) {}

static std::string ReadCString(const uint8* p, size_t len) {
    // Stop at the first NUL or at the MSVC 0xCD heap-fill marker.
    size_t e = 0;
    while (e < len && p[e] != 0 && p[e] != 0xCD) ++e;
    return std::string((const char*)p, e);
}

bool DatFile::LoadFromFile(const std::string& rPath, DatKind eKind) {
    FILE* fp = NULL; fopen_s(&fp, rPath.c_str(), "rb");
    if (!fp) return false;
    fseek(fp, 0, SEEK_END); long sz = ftell(fp); fseek(fp, 0, SEEK_SET);
    if (sz < 4) { fclose(fp); return false; }
    std::vector<uint8> kBuf((size_t)sz);
    fread(&kBuf[0], 1, (size_t)sz, fp);
    fclose(fp);

    m_eKind = eKind;
    m_kEntries.clear();

    const uint8* p = &kBuf[0];
    size_t n = kBuf.size();
    if (n < 4) return false;
    uint32 uiCount = (uint32)p[0] | ((uint32)p[1]<<8) | ((uint32)p[2]<<16) | ((uint32)p[3]<<24);
    size_t entrySize = DatFile::EntrySizeFor(eKind);
    size_t expected = 4 + (size_t)uiCount * entrySize;
    if (expected != n) {
        SHINELOG_WARN("DatFile %s: count=%u kind=%d expected %u bytes, got %u",
                      rPath.c_str(), uiCount, (int)eKind,
                      (uint32)expected, (uint32)n);
        // Continue tolerantly: clamp count to what fits.
        if (entrySize == 0) return false;
        uiCount = (uint32)((n - 4) / entrySize);
    }
    m_kEntries.reserve(uiCount);

    // Within an entry: first 2 bytes = uiKind, then up to 64 bytes of clip name
    // (zero-padded with 0xCD trailing fill), then payload tail.
    const size_t kClipFieldOffset = 2;
    const size_t kClipFieldLen    = 64;
    for (uint32 i = 0; i < uiCount; ++i) {
        const uint8* e = p + 4 + (size_t)i * entrySize;
        DatEntry de;
        de.uiKind = (uint16)e[0] | ((uint16)e[1] << 8);
        if (entrySize >= kClipFieldOffset + kClipFieldLen) {
            de.kClipName = ReadCString(e + kClipFieldOffset, kClipFieldLen);
        } else if (entrySize > kClipFieldOffset) {
            de.kClipName = ReadCString(e + kClipFieldOffset, entrySize - kClipFieldOffset);
        }
        size_t payloadOff = kClipFieldOffset + kClipFieldLen;
        if (payloadOff < entrySize) {
            de.kPayload.assign(e + payloadOff, e + entrySize);
        }
        m_kEntries.push_back(de);
    }
    SHINELOG_INFO("DatFile loaded %s kind=%d entries=%u entrySize=%u",
                  rPath.c_str(), (int)eKind, (uint32)m_kEntries.size(), (uint32)entrySize);
    return true;
}

// ---------------- ActionDatBox ----------------
ActionDatBox& ActionDatBox::Get() { static ActionDatBox s; return s; }

const DatFile* ActionDatBox::Load(const std::string& rRoot, const std::string& rName) {
    std::string p = rRoot + "\\Action\\" + rName + ".dat";
    DatFile* pk = new DatFile();
    if (!pk->LoadFromFile(p, DAT_KIND_ACTION)) { delete pk; return NULL; }
    m_kAll[rName] = pk;
    return pk;
}

const DatFile* ActionDatBox::Find(const std::string& rName) const {
    std::map<std::string, DatFile*>::const_iterator it = m_kAll.find(rName);
    return (it == m_kAll.end()) ? NULL : it->second;
}

void ActionDatBox::Clear() {
    for (std::map<std::string, DatFile*>::iterator it = m_kAll.begin(); it != m_kAll.end(); ++it)
        delete it->second;
    m_kAll.clear();
}

} // namespace fiesta
