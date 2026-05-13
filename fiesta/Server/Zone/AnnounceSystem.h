// Server/Zone/AnnounceSystem.h
// Public entry point for zone-wide announcements -- system messages,
// PineScript broadcasts, GMEvent banners.
#ifndef SHINE_ZONE_ANNOUNCESYSTEM_H
#define SHINE_ZONE_ANNOUNCESYSTEM_H
#include "../Shared/ShineTypes.h"
#include <string>

namespace shine {

class AnnounceSystem {
public:
    // uiLevel: 0=normal banner, 1=warning, 2=critical (drives client-side
    // colour and SFX). Text is written into NC_CHAT_NORMAL_CMD bodies and
    // fan-out to every connected player on this Zone.
    static void Broadcast(uint8 uiLevel, const std::string& rText);
};

} // namespace shine
#endif
