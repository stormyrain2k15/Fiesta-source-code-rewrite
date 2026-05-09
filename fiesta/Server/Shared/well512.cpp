// Server/Shared/well512.cpp
// 01 Foundation -- WELL512 PRNG impl.
// EVIDENCE: PDB_CONFIRMED
#include "well512.h"
#include <time.h>

namespace fiesta {

well512::well512() : m_uiIndex(0) {
    Seed((uint32)time(NULL) ^ (uint32)GetCurrentThreadId());
}

void well512::Seed(uint32 uiSeed) {
    m_uiIndex = 0;
    uint32 x = uiSeed ? uiSeed : 0x12345678u;
    for (int i = 0; i < 16; ++i) {
        x ^= x << 13; x ^= x >> 17; x ^= x << 5;
        m_auiState[i] = x;
    }
}

uint32 well512::Next() {
    uint32 a = m_auiState[m_uiIndex];
    uint32 c = m_auiState[(m_uiIndex + 13) & 15];
    uint32 b = a ^ c ^ (a << 16) ^ (c << 15);
    c = m_auiState[(m_uiIndex + 9) & 15];
    c ^= (c >> 11);
    a = m_auiState[m_uiIndex] = b ^ c;
    uint32 d = a ^ ((a << 5) & 0xDA442D24UL);
    m_uiIndex = (m_uiIndex + 15) & 15;
    a = m_auiState[m_uiIndex];
    m_auiState[m_uiIndex] = a ^ b ^ d ^ (a << 2) ^ (b << 18) ^ (c << 28);
    return m_auiState[m_uiIndex];
}

uint32 well512::NextRange(uint32 uiMaxExclusive) {
    if (uiMaxExclusive == 0) return 0;
    return Next() % uiMaxExclusive;
}

double well512::NextDouble() {
    return (double)Next() / 4294967296.0;
}

} // namespace fiesta
