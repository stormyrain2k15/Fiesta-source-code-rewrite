// Server/Zone/Network/ZoneBaseSession.cpp
// Common ancestor for the various zone session types. Handles the encrypt
// handshake + opcode-family routing.
#include "../../Shared/IOCPManager.h"
namespace fiesta { class ZoneBaseSession : public IOCPSession {}; }
