// Server/Common/SendPacket.h
// helper SendPacket(session, opcode, builder) symmetric with PDB symbol SendPacket.
#ifndef SHINE_SENDPACKET_H
#define SHINE_SENDPACKET_H
#include "../Shared/ShineTypes.h"
#include "../Shared/GPacket.h"
#include "../Shared/Socket_Acceptor.h"
#include "NETCOMMAND.h"

namespace shine {

inline bool SendPacket(IOCPSession* pkSess, NCOpcode nc) {
    if (!pkSess) return false;
    GPacket pk(nc); return pkSess->SendPacket(pk);
}

inline bool SendPacket(IOCPSession* pkSess, NCOpcode nc, const void* pv, size_t n) {
    if (!pkSess) return false;
    GPacket pk(nc, pv, n); return pkSess->SendPacket(pk);
}

} // namespace shine
#endif
