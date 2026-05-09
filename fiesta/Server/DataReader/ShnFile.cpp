// Server/DataReader/ShnFile.cpp
#include "ShnFile.h"
#include "../Shared/ShineLogSystem.h"
#include <stdio.h>
#include <string.h>

namespace fiesta {

ShnFile::ShnFile() : m_uiHeader(0), m_uiDefaultRecLen(0) {
    memset(m_aCryptHeader, 0, sizeof(m_aCryptHeader));
}

// Symmetric XOR cipher with a rolling key seeded from the data length and
// position-derived constants. Encrypt and decrypt run the same operation.
void ShnFile::Crypt(uint8* p, size_t n) {
    if (!p || n == 0) return;
    uint8 num = (uint8)n;                    // initial key = low byte of length
    for (size_t k = n; k > 0; --k) {
        size_t i = k - 1;
        p[i] = (uint8)(p[i] ^ num);
        uint8 num3 = (uint8)((uint8)i & 0x0F);
        num3 = (uint8)(num3 + 0x55u);
        num3 = (uint8)(num3 ^ (uint8)(((uint8)i) * 11u));
        num3 = (uint8)(num3 ^ num);
        num3 = (uint8)(num3 ^ 0xAAu);
        num = num3;
    }
}

// Helpers for in-memory cursor reads on the decrypted body.
namespace {
    struct Cursor {
        const uint8* p; size_t n; size_t k;
        Cursor(const uint8* a, size_t b) : p(a), n(b), k(0) {}
        bool   ok(size_t need) const { return (k + need) <= n; }
        uint8  u8 () { return ok(1) ? p[k++] : (uint8)0; }
        int8   i8 () { return (int8)u8(); }
        uint16 u16() { if (!ok(2)) return 0; uint16 v = (uint16)p[k] | ((uint16)p[k+1]<<8); k+=2; return v; }
        int16  i16() { return (int16)u16(); }
        uint32 u32() { if (!ok(4)) return 0; uint32 v = (uint32)p[k] | ((uint32)p[k+1]<<8) | ((uint32)p[k+2]<<16) | ((uint32)p[k+3]<<24); k+=4; return v; }
        int32  i32() { return (int32)u32(); }
        float  f32() { uint32 u = u32(); float f; memcpy(&f, &u, 4); return f; }
        std::string fixedString(size_t len) {
            if (!ok(len)) return std::string();
            const uint8* s = p + k; k += len;
            // strip trailing zero padding for display
            size_t e = 0; while (e < len && s[e] != 0) ++e;
            return std::string((const char*)s, e);
        }
        std::string nullString() {
            std::string out;
            while (ok(1)) {
                uint8 b = p[k++];
                if (b == 0) break;
                out.push_back((char)b);
            }
            return out;
        }
    };
}

bool ShnFile::LoadFromFile(const std::string& rPath) {
    FILE* fp = NULL; fopen_s(&fp, rPath.c_str(), "rb");
    if (!fp) return false;
    fseek(fp, 0, SEEK_END); long sz = ftell(fp); fseek(fp, 0, SEEK_SET);
    if (sz < 0x24) { fclose(fp); return false; }
    std::vector<uint8> kBuf((size_t)sz);
    fread(&kBuf[0], 1, (size_t)sz, fp);
    fclose(fp);
    return LoadFromMemory(&kBuf[0], kBuf.size());
}

bool ShnFile::LoadFromMemory(const uint8* p, size_t n) {
    if (!p || n < 0x24) return false;
    memcpy(m_aCryptHeader, p, 0x20);
    uint32 uiTotal = (uint32)p[0x20] | ((uint32)p[0x21]<<8) | ((uint32)p[0x22]<<16) | ((uint32)p[0x23]<<24);
    if (uiTotal < 0x24 || (size_t)uiTotal > n) {
        SHINELOG_ERROR("ShnFile: bad total len %u (file %u)", uiTotal, (uint32)n);
        return false;
    }
    size_t bodyLen = (size_t)(uiTotal - 0x24);
    std::vector<uint8> kBody(bodyLen);
    if (bodyLen) memcpy(&kBody[0], p + 0x24, bodyLen);
    Crypt(bodyLen ? &kBody[0] : NULL, bodyLen);
    return ParseDecrypted(bodyLen ? &kBody[0] : NULL, bodyLen);
}

bool ShnFile::ParseDecrypted(const uint8* body, size_t n) {
    Cursor r(body, n);
    if (!r.ok(16)) return false;
    m_uiHeader        = r.u32();
    uint32 uiRecCount = r.u32();
    m_uiDefaultRecLen = r.u32();
    uint32 uiColCount = r.u32();
    if (uiColCount > 4096 || uiRecCount > (1u<<24)) return false; // sanity caps

    m_kColumns.clear(); m_kColumns.reserve(uiColCount);
    int unkIdx = 0;
    uint32 uiSumLen = 2; // matches editor's num2 starting at 2 for the rowSize prefix
    for (uint32 i = 0; i < uiColCount; ++i) {
        if (!r.ok(0x30 + 4 + 4)) return false;
        ShnColumn c;
        c.kName  = r.fixedString(0x30);
        c.uiType = r.u32();
        c.iLength= r.i32();
        if (c.kName.empty()) {
            char b[32]; sprintf_s(b, sizeof(b), "UnkCol%d", unkIdx++);
            c.kName = b;
        }
        m_kColumns.push_back(c);
        uiSumLen += (uint32)(c.iLength > 0 ? c.iLength : 0);
    }
    if (uiSumLen != m_uiDefaultRecLen) {
        SHINELOG_WARN("ShnFile: row length mismatch (sum=%u declared=%u) -- continuing", uiSumLen, m_uiDefaultRecLen);
    }

    m_kRows.clear(); m_kRows.reserve(uiRecCount);
    for (uint32 i = 0; i < uiRecCount; ++i) {
        if (!r.ok(2)) break;
        (void)r.u16(); // rowSize prefix (re-derived on save)
        std::vector<ShnValue> row; row.reserve(m_kColumns.size());
        for (size_t c = 0; c < m_kColumns.size(); ++c) {
            const ShnColumn& col = m_kColumns[c];
            ShnValue v; v.uiType = col.uiType; v.iVal = 0; v.fVal = 0.0f;
            switch (col.uiType) {
                case SHN_T_U8:       v.iVal = r.u8(); break;
                case SHN_T_U16:      v.iVal = r.u16(); break;
                case SHN_T_U32:      v.iVal = r.u32(); break;
                case SHN_T_F32:      v.fVal = r.f32(); break;
                case SHN_T_STR_FIX1: v.kStr = r.fixedString((size_t)(col.iLength > 0 ? col.iLength : 0)); break;
                case SHN_T_U32_B:    v.iVal = r.u32(); break;
                case SHN_T_U8_B:     v.iVal = r.u8(); break;
                case SHN_T_I16:      v.iVal = r.i16(); break;
                case SHN_T_U8_C:     v.iVal = r.u8(); break;
                case SHN_T_U32_C:    v.iVal = r.u32(); break;
                case SHN_T_I8:       v.iVal = r.i8(); break;
                case SHN_T_I16_B:    v.iVal = r.i16(); break;
                case SHN_T_I32:      v.iVal = r.i32(); break;
                case SHN_T_STR_FIX2: v.kStr = r.fixedString((size_t)(col.iLength > 0 ? col.iLength : 0)); break;
                case SHN_T_STR_VAR:  v.kStr = r.nullString(); break;
                case SHN_T_U32_D:    v.iVal = r.u32(); break;
                default:
                    SHINELOG_WARN("ShnFile: unknown column type 0x%X (col %s) -- aborting row", col.uiType, col.kName.c_str());
                    return false;
            }
            row.push_back(v);
        }
        m_kRows.push_back(row);
    }
    SHINELOG_INFO("ShnFile parsed: cols=%u rows=%u recLen=%u",
                  (uint32)m_kColumns.size(), (uint32)m_kRows.size(), m_uiDefaultRecLen);
    return true;
}

void ShnFile::ExportAsStringRows(std::vector<std::vector<std::string> >& rOut) const {
    rOut.clear();
    // Header row
    std::vector<std::string> hdr;
    for (size_t c = 0; c < m_kColumns.size(); ++c) hdr.push_back(m_kColumns[c].kName);
    rOut.push_back(hdr);
    char buf[64];
    for (size_t i = 0; i < m_kRows.size(); ++i) {
        const std::vector<ShnValue>& kr = m_kRows[i];
        std::vector<std::string> sr; sr.reserve(kr.size());
        for (size_t c = 0; c < kr.size(); ++c) {
            const ShnValue& v = kr[c];
            switch (v.uiType) {
                case SHN_T_F32:
                    sprintf_s(buf, sizeof(buf), "%g", v.fVal); sr.push_back(buf); break;
                case SHN_T_STR_FIX1:
                case SHN_T_STR_FIX2:
                case SHN_T_STR_VAR:
                    sr.push_back(v.kStr); break;
                case SHN_T_I8: case SHN_T_I16: case SHN_T_I16_B: case SHN_T_I32:
                    sprintf_s(buf, sizeof(buf), "%lld", (long long)v.iVal); sr.push_back(buf); break;
                default:
                    sprintf_s(buf, sizeof(buf), "%llu", (unsigned long long)(uint64)v.iVal); sr.push_back(buf); break;
            }
        }
        rOut.push_back(sr);
    }
}

} // namespace fiesta
