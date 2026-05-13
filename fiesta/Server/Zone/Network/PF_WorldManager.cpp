// Server/Zone/Network/PF_WorldManager.cpp
// Protocol family: Zone <-> WM. Inbound: OPTool fanout broadcasts
// (NC_INTER_OPTOOL_*_PUSH), GM event broadcasts, KQ state, friend deltas,
// chat fanout. Outbound: zone-register at boot + heartbeat every 5s.
#include "../WMClient.h"
namespace shine { void PF_WorldManager_OnConnect() {} }
