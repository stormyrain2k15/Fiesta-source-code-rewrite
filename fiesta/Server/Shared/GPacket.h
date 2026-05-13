// Server/Shared/GPacket.h
// GPacket = packet header + PacketBuffer payload.
// Wire frame:  [uint16 len][uint16 ncOpcode][payload bytes...]
#ifndef SHINE_GPACKET_H
#define SHINE_GPACKET_H
#include "PacketBuffer.h"

namespace shine {

class GPacket {
public:
    GPacket();
    explicit GPacket(NCOpcode nc);
    GPacket(NCOpcode nc, const void* p, size_t n);

    NCOpcode      GetOpcode() const { return m_uiOpcode; }
    void          SetOpcode(NCOpcode nc) { m_uiOpcode = nc; }
    PacketBuffer& Body() { return m_kBody; }
    const PacketBuffer& Body() const { return m_kBody; }

    // Build a complete on-wire frame.
    void Pack(PacketBuffer& rOut) const;
    // Parse one frame from buf; returns false if no full frame yet.
    static bool Unpack(PacketBuffer& rIn, GPacket& rOut, size_t& rConsumed);
private:
    NCOpcode     m_uiOpcode;
    PacketBuffer m_kBody;
};

} // namespace shine
#endif
