// Server/Zone/GMEventManager.h
// Zone-side hook for GM event broadcasts received from WorldManager.
#ifndef FIESTA_ZONE_GMEVENTMANAGER_H
#define FIESTA_ZONE_GMEVENTMANAGER_H
#include "../Shared/ShineTypes.h"

namespace fiesta {

class GMEventManager_Zone {
public:
    // Fired by WMClient on NC_INTER_BROADCAST_CMD kind=2.
    // bStart=true for window enter, false for window exit.
    static void OnEventBroadcast(uint32 uiEventNo, bool bStart);
};

} // namespace fiesta
#endif
