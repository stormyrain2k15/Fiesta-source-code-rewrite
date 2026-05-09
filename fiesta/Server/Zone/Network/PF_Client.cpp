// Server/Zone/Network/PF_Client.cpp
// Protocol family: Zone <-> game client. Hands off to ZoneHandlers.cpp's
// dispatcher. Kept here so the canonical PF_* layout matches WM.
#include "../ZoneServer.h"
namespace fiesta { void PF_Client_RegisterAll() { /* delegated to ZoneHandlers::RegisterAll */ } }
