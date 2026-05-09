// Server/DataReader/AbStateInfoFile.cpp
#include "AbStateInfoFile.h"
#include "../Shared/ShineLogSystem.h"
#include <stdio.h>
#include <string.h>

namespace fiesta {

static bool SlurpFile(const std::string& rPath, std::vector<uint8>& rOut) {
    FILE* fp = NULL; fopen_s(&fp, rPath.c_str(), "rb");
    if (!fp) return false;
    fseek(fp, 0, SEEK_END); long n = ftell(fp); fseek(fp, 0, SEEK_SET);
    if (n < 0) { fclose(fp); return false; }
    rOut.resize((size_t)n);
    if (n > 0) fread(&rOut[0], 1, (size_t)n, fp);
    fclose(fp);
    return true;
}
static uint32 RdU32(const uint8* p) {
    return (uint32)p[0] | ((uint32)p[1]<<8) | ((uint32)p[2]<<16) | ((uint32)p[3]<<24);
}
static std::string RdAscii(const uint8* p, size_t n) {
    size_t e = 0; while (e < n && p[e] != 0) ++e;
    return std::string((const char*)p, e);
}

// AbStateInfo.dat -- per the captured hex prefix the file starts with:
//   0a 00 00 00          ; count = 0x0A
//   01 00                ; mainStateId = 1, then padding/pad
//   "StaHardSkin\0..."   ; English name
//   <padded zeros>
//   <Korean label bytes...>
// We parse it as: u32 count; then for each entry { u8 id, u8 pad, char[32]
// english, char[32] koreanLabel }. If layout doesn't fit cleanly we surface
// the unparsed bytes via kRest and continue.
bool LoadAbStateInfo(const std::string& rPath, AbStateMaster& rOut) {
    std::vector<uint8> kBuf; if (!SlurpFile(rPath, kBuf) || kBuf.size() < 4) return false;
    rOut.kEntries.clear(); rOut.kRest.clear();
    uint32 n = RdU32(&kBuf[0]);
    if (n > 256) { SHINELOG_WARN("AbStateInfo: implausible count=%u in %s", n, rPath.c_str()); return false; }
    size_t off = 4;
    const size_t kEntry = 1 + 1 + 32 + 32;     // {id, pad, en[32], ko[32]}
    for (uint32 i = 0; i < n && off + kEntry <= kBuf.size(); ++i) {
        AbStateMasterEntry e;
        e.uiMainStateId = kBuf[off];
        e.kInxName = RdAscii(&kBuf[off + 2], 32);
        e.kLabel   = RdAscii(&kBuf[off + 2 + 32], 32);
        rOut.kEntries.push_back(e);
        off += kEntry;
    }
    if (off < kBuf.size()) rOut.kRest.assign(kBuf.begin() + off, kBuf.end());
    return true;
}

// StaXxx.dat -- u32 count; rows follow until EOF. We auto-detect stride by
// dividing remaining bytes by count; valid strides are 24 / 28 / 32. Each row
// is exposed as up to 7 raw u32s.
bool LoadAbStateTimeline(const std::string& rPath, AbStateTimeline& rOut) {
    std::vector<uint8> kBuf; if (!SlurpFile(rPath, kBuf) || kBuf.size() < 4) return false;
    rOut.kRows.clear();
    rOut.uiRecCount = RdU32(&kBuf[0]);
    rOut.uiRowBytes = 0;
    size_t avail = kBuf.size() - 4;
    if (rOut.uiRecCount == 0 || rOut.uiRecCount > 65536) return false;
    // Detect stride by exact division.
    static const uint32 kCandidates[] = { 28, 32, 24, 20, 16 };
    for (size_t i = 0; i < sizeof(kCandidates) / sizeof(*kCandidates); ++i) {
        if ((size_t)rOut.uiRecCount * kCandidates[i] <= avail) {
            rOut.uiRowBytes = kCandidates[i];
            break;
        }
    }
    if (!rOut.uiRowBytes) return false;
    const uint8* p = &kBuf[4];
    for (uint32 i = 0; i < rOut.uiRecCount; ++i) {
        AbStateTimelineRow row;
        memset(row.aField, 0, sizeof(row.aField));
        uint32 nU32 = rOut.uiRowBytes / 4;
        if (nU32 > 7) nU32 = 7;
        for (uint32 k = 0; k < nU32; ++k) row.aField[k] = RdU32(p + 4 * k);
        rOut.kRows.push_back(row);
        p += rOut.uiRowBytes;
    }
    return true;
}

} // namespace fiesta
