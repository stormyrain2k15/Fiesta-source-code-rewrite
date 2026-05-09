// Server/Zone/GMEventManager.cpp
// Zone-side GM event hook. Receives NC_INTER_BROADCAST_CMD with kind=2
// (start/end of a GM event from WorldManager::GMEventManager) and
// applies the per-event modifiers via LiveOpsBoosts (drop rate, exp
// rate, money rate, sysmsg banner).
//
// Wire-format of the body, written by WM/GMEventManager.cpp:
//   uint8  kind     == 2
//   uint32 eventNo  -- matches GMEvent.shn EventNo column
//   uint8  action   -- 1 = start, 0 = end
// (consumed in WMClient.cpp / OnInterBroadcast which forwards eventNo +
// bStart to GMEventManager_Zone::OnEventBroadcast.)
#include "GMEventManager.h"
#include "AnnounceSystem.h"
#include "LiveOpsBoosts.h"
#include "../DataReader/ShnRegistry.h"
#include "../Shared/ShineLogSystem.h"

namespace fiesta {

namespace {
    // Map HHMM start/end pair to a seconds duration. Wrap-around (end<
    // start) means "spans midnight" so we treat it as the time from
    // start to next-day end.
    uint32 ResolveDurationSec(uint32 uiEventNo) {
        const ShnFile* pkT = ShnRegistry::Get().GetTable("GMEvent");
        if (!pkT) return 3600;
        for (uint32 i = 0; i < pkT->RecordCount(); ++i) {
            int32 iEventNo = ShnGetI32(*pkT, i, "EventNo");
            if ((uint32)iEventNo != uiEventNo) continue;
            int32 iStart = ShnGetI32(*pkT, i, "StartTime");
            int32 iEnd   = ShnGetI32(*pkT, i, "EndTime");
            int32 iStartS = (iStart / 100) * 3600 + (iStart % 100) * 60;
            int32 iEndS   = (iEnd   / 100) * 3600 + (iEnd   % 100) * 60;
            int32 iDur    = iEndS - iStartS;
            if (iDur <= 0) iDur += 24 * 3600;
            return (uint32)iDur;
        }
        return 3600;
    }
}

void GMEventManager_Zone::OnEventBroadcast(uint32 uiEventNo, bool bStart) {
    const uint32 uiDurationSec = ResolveDurationSec(uiEventNo);

    if (bStart) {
        if (!LiveOpsBoosts::Get().StartEvent(uiEventNo, uiDurationSec)) {
            // Unrecognised event id: still emit a banner so the GM crew
            // sees the broadcast landed on this zone.
            char szBuf[128];
            wsprintfA(szBuf, "GM event %u has begun.", uiEventNo);
            AnnounceSystem::Broadcast(0, std::string(szBuf));
            SHINELOG_INFO("Zone GM event %u START (no boost binding)", uiEventNo);
        }
    } else {
        if (!LiveOpsBoosts::Get().StopEvent(uiEventNo)) {
            char szBuf[128];
            wsprintfA(szBuf, "GM event %u has ended.", uiEventNo);
            AnnounceSystem::Broadcast(0, std::string(szBuf));
            SHINELOG_INFO("Zone GM event %u END (no active boost)", uiEventNo);
        }
    }
}

} // namespace fiesta

