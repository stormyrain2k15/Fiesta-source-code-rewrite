// Server/Zone/GMEventManager.cpp
// Zone-side GM event hook. Receives NC_INTER_BROADCAST_CMD with kind=2
// (start/end of a GM event) and applies the per-event modifiers (drop
// rate boost, exp boost, sysmsg banner).
#include "../Shared/ShineLogSystem.h"
namespace fiesta {
class GMEventManager_Zone {
public:
    static void OnEventBroadcast(uint32 uiEventNo, bool bStart) {
        SHINELOG_INFO("Zone GM event %u %s", uiEventNo, bStart ? "START" : "END");
    }
};
} // namespace fiesta
