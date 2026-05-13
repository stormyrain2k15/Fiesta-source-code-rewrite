// Server/Common/ProtocolParser.cpp
#include "ProtocolParser.h"
#include "../Shared/ShineLogSystem.h"

namespace shine {

void ProtocolParser::Register(NCOpcode nc, Handler pk) {
    if (!pk) return;
    m_kMap[nc] = pk;
}

bool ProtocolParser::Dispatch(IOCPSession* pkSess, const GPacket& rPkt) const {
    std::map<NCOpcode, Handler>::const_iterator it = m_kMap.find(rPkt.GetOpcode());
    if (it == m_kMap.end()) {
        SHINELOG_WARN("ProtocolParser: no handler for NC=0x%04X size=%u",
                      rPkt.GetOpcode(), (uint32)rPkt.Body().Size());
        return false;
    }
    it->second(pkSess, rPkt);
    return true;
}

} // namespace shine
