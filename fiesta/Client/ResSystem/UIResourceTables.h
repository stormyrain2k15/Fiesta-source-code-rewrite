// Client/ResSystem/UIResourceTables.h
// public-name mirror tables (Recv_NC_* / Send_NC_*). The client-side string
// resources expose human-readable names that map to the canonical NC_* opcodes
// in Server/Common/NETCOMMAND.h. Used by tooltips, debug logs, packet inspector.
#ifndef SHINE_CLIENT_UIRESOURCETABLES_H
#define SHINE_CLIENT_UIRESOURCETABLES_H
#include "../../Server/Common/NETCOMMAND.h"
#include <string>

namespace shine {

// Returns "NC_USER_LOGIN_REQ" for 0x0F05, etc. Useful for the in-client packet log.
const char* NCName(NCOpcode nc);

// Inverse: parse "NC_USER_LOGIN_REQ" -> 0x0F05 if known.
NCOpcode    ParseNCName(const std::string& rName);

} // namespace shine
#endif
