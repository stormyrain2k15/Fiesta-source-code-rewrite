// Server/Zone/Network/ZoneBaseSessionManager.cpp
// Per-zone session registry -- one entry per session type (Client,
// WMClient, CharDBClient, GameLogClient, OPToolPeer, ZonePeer).
#include "../../Shared/IOCPManager.h"
namespace fiesta { class ZoneBaseSessionManager { public: static ZoneBaseSessionManager& Get(){ static ZoneBaseSessionManager s; return s; } }; }
