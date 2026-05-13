// Server/Shared/GPacket.cpp
#include "GPacket.h"
#include <string.h>

namespace shine {

GPacket::GPacket() : m_uiOpcode(0) {}
GPacket::GPacket(NCOpcode nc) : m_uiOpcode(nc) {}
GPacket::GPacket(NCOpcode nc, const void* p, size_t n) : m_uiOpcode(nc) {
    if (p && n) m_kBody.WriteBytes(p, n);
}

void GPacket::Pack(PacketBuffer& rOut) const {
    uint16 uiLen = (uint16)(4 + m_kBody.Size());
    rOut.WriteU16(uiLen);
    rOut.WriteU16(m_uiOpcode);
    if (m_kBody.Size()) rOut.WriteBytes(m_kBody.Data(), m_kBody.Size());
}

bool GPacket::Unpack(PacketBuffer& rIn, GPacket& rOut, size_t& rConsumed) {
    rConsumed = 0;
    if (rIn.Size() < 4) return false;
    const uint8* p = rIn.Data();
    uint16 uiLen = (uint16)p[0] | ((uint16)p[1] << 8);
    if (uiLen < 4) { rConsumed = rIn.Size(); return false; } // bad frame -- caller resets
    if (rIn.Size() < uiLen) return false;
    rOut.m_uiOpcode = (uint16)p[2] | ((uint16)p[3] << 8);
    rOut.m_kBody.Clear();
    if (uiLen > 4) rOut.m_kBody.WriteBytes(p + 4, uiLen - 4);
    rConsumed = uiLen;
    return true;
}

} // namespace shine
