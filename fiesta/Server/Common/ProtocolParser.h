// Server/Common/ProtocolParser.h
// per-session opcode dispatcher.
#ifndef SHINE_PROTOCOLPARSER_H
#define SHINE_PROTOCOLPARSER_H
#include "../Shared/ShineTypes.h"
#include "../Shared/GPacket.h"
#include "NETCOMMAND.h"
#include <map>

namespace shine {

class IOCPSession;

class ProtocolParser {
public:
    typedef void (*Handler)(IOCPSession* pkSess, const GPacket& rPkt);

    void  Register(NCOpcode nc, Handler pkFn);
    bool  Dispatch(IOCPSession* pkSess, const GPacket& rPkt) const;
    size_t HandlerCount() const { return m_kMap.size(); }
private:
    std::map<NCOpcode, Handler> m_kMap;
};

// Per-service singleton getters (defined in each service's Main.cpp).
ProtocolParser& GetLoginParser();
ProtocolParser& GetWMParser();
ProtocolParser& GetZoneParser();

} // namespace shine
#endif
