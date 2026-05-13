// Server/DataReader/BlockInfoFile.cpp
#include "BlockInfoFile.h"
#include "../Shared/ShineLogSystem.h"
#include <stdio.h>
#include <string.h>

namespace shine {

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

bool LoadShbd(const std::string& rPath, BlockGrid& rOut) {
    std::vector<uint8> kBuf; if (!SlurpFile(rPath, kBuf) || kBuf.size() < 8) return false;
    rOut.uiWidth  = RdU32(&kBuf[0]);
    rOut.uiHeight = RdU32(&kBuf[4]);
    size_t need = (size_t)rOut.uiWidth * (size_t)rOut.uiHeight;
    size_t avail = kBuf.size() - 8;
    if (need == 0 || need > avail) {
        // Hard refuse rather than silently producing a wrong-size grid:
        // a mis-sized grid causes IsBlockedCell/IsBlockedWorld to either
        // out-of-bounds or to read the wrong row, and a mob can phase
        // through walls. Fail loud and let the caller log + skip.
        SHINELOG_WARN("Shbd: %s header geometry %ux%u != payload %u bytes -- "
                      "refusing load (file is malformed or geometry is encoded "
                      "differently than the documented one-byte-per-cell layout).",
                      rPath.c_str(), rOut.uiWidth, rOut.uiHeight, (uint32)avail);
        rOut.uiWidth = rOut.uiHeight = 0;
        rOut.kCells.clear();
        return false;
    }
    rOut.kCells.assign(kBuf.begin() + 8, kBuf.begin() + 8 + need);
    return true;
}

bool LoadAid(const std::string& rPath, AidFile& rOut) {
    std::vector<uint8> kBuf; if (!SlurpFile(rPath, kBuf) || kBuf.size() < 4) return false;
    uint32 n = RdU32(&kBuf[0]);
    if (n > 1024) { SHINELOG_WARN("Aid: implausible entryCount=%u in %s", n, rPath.c_str()); return false; }
    rOut.kEntries.clear(); rOut.kPayload.clear();
    size_t off = 4;
    for (uint32 i = 0; i < n && off + 32 <= kBuf.size(); ++i) {
        AidEntry e; e.kName = RdAscii(&kBuf[off], 32);
        rOut.kEntries.push_back(e);
        off += 32;
    }
    if (off < kBuf.size())
        rOut.kPayload.assign(kBuf.begin() + off, kBuf.end());
    return true;
}

bool LoadSbi(const std::string& rPath, SbiFile& rOut) {
    std::vector<uint8> kBuf; if (!SlurpFile(rPath, kBuf) || kBuf.size() < 4) return false;
    uint32 n = RdU32(&kBuf[0]);
    if (n > 4096) { SHINELOG_WARN("Sbi: implausible entryCount=%u in %s", n, rPath.c_str()); return false; }
    rOut.kEntries.clear(); rOut.kRest.clear();
    size_t off = 4;
    // Each entry: 32 bytes name + 5 * uint32 fields = 52 bytes.
    const size_t kEntrySize = 32 + 20;
    for (uint32 i = 0; i < n && off + kEntrySize <= kBuf.size(); ++i) {
        SbiEntry e;
        e.kName = RdAscii(&kBuf[off], 32);
        for (int k = 0; k < 5; ++k)
            e.aField[k] = RdU32(&kBuf[off + 32 + 4 * k]);
        rOut.kEntries.push_back(e);
        off += kEntrySize;
    }
    if (off < kBuf.size())
        rOut.kRest.assign(kBuf.begin() + off, kBuf.end());
    return true;
}

} // namespace shine
