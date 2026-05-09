// Server/Shared/CToken.cpp
#include "CToken.h"
#include "well512.h"
#include "GTimer.h"
#include <string.h>

namespace fiesta {

CToken::CToken() { Reset(); }

void CToken::Reset() {
    m_uiAccount = 0; m_uiWorldId = 0; m_uiIssuedMs = 0;
    memset(m_aSecret, 0, 16);
}

void CToken::Generate(AccountID aid, uint32 uiWorldId) {
    static well512 s_kRng;
    m_uiAccount = aid; m_uiWorldId = uiWorldId; m_uiIssuedMs = GTimer::NowMillis();
    for (int i = 0; i < 16; i += 4) {
        uint32 r = s_kRng.Next();
        m_aSecret[i+0] = (uint8)(r); m_aSecret[i+1] = (uint8)(r>>8);
        m_aSecret[i+2] = (uint8)(r>>16); m_aSecret[i+3] = (uint8)(r>>24);
    }
}

bool CToken::Verify(AccountID aid, uint32 uiWorldId, const uint8 aSecret[16]) const {
    if (aid != m_uiAccount || uiWorldId != m_uiWorldId) return false;
    // Constant-time compare.
    uint8 d = 0;
    for (int i = 0; i < 16; ++i) d |= (uint8)(m_aSecret[i] ^ aSecret[i]);
    return d == 0;
}

} // namespace fiesta
