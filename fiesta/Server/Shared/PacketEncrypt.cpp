// Server/Shared/PacketEncrypt.cpp
// Provisional symmetric stream cipher. Local tunable constants only;
// no extra config layer per spec rule 04. Rotate constants here when a
// runtime capture pins down the original cipher schedule.
#include "PacketEncrypt.h"

namespace shine {

// Local provisional constants -- edit in-place to match capture.
static const uint32 kProvSchedMul = 0x9E3779B9u;
static const uint32 kProvSchedAdd = 0x7F4A7C15u;
static const uint32 kProvCtrInit  = 0x12345678u;

PacketEncrypt::PacketEncrypt() : m_uiCounter(0) {
    for (int i = 0; i < 256; ++i) m_auiKeySched[i] = 0;
}

void PacketEncrypt::Schedule(uint32 uiSeed) {
    uint32 x = uiSeed ? uiSeed : 1u;
    for (int i = 0; i < 256; ++i) {
        x = x * kProvSchedMul + kProvSchedAdd + (uint32)i;
        x ^= (x >> 13); x ^= (x << 17); x ^= (x >> 5);
        m_auiKeySched[i] = x;
    }
    m_uiCounter = kProvCtrInit ^ uiSeed;
}

void PacketEncrypt::Init(uint32 uiSeed) { Schedule(uiSeed); }

void PacketEncrypt::Encrypt(uint8* pBuf, size_t uiLen) {
    for (size_t i = 0; i < uiLen; ++i) {
        uint32 k = m_auiKeySched[(m_uiCounter + (uint32)i) & 0xFF];
        pBuf[i] = (uint8)(pBuf[i] ^ (uint8)(k & 0xFF));
        if (((i + 1) & 0x3F) == 0) m_uiCounter += 0x1Fu;
    }
    m_uiCounter += (uint32)uiLen;
}

void PacketEncrypt::Decrypt(uint8* pBuf, size_t uiLen) { Encrypt(pBuf, uiLen); }

} // namespace shine
