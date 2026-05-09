// Server/Zone/InterBroadcastSinks.h
// Zone-side fan-in for the WM-driven NC_INTER_BROADCAST_CMD envelopes
// (kind=3 daily reset, kind=4 NPC schedule). Each sink is a process-wide
// singleton so any subsystem can register a callback and the WMClient
// dispatcher can fire all of them with one call. The interface is kept
// small on purpose -- there's no priority / unregister surface because
// every consumer outlives the sink (process lifetime).
#ifndef FIESTA_ZONE_INTERBROADCASTSINKS_H
#define FIESTA_ZONE_INTERBROADCASTSINKS_H
#include "../Shared/ShineTypes.h"
#include <vector>

namespace fiesta {

typedef void (*DailyResetFn)(uint32 uiDayKey);
typedef void (*NpcScheduleFn)(uint32 uiNpcId, uint16 uiMapId, bool bSpawn);

class DailyResetSink {
public:
    static DailyResetSink& Get();
    void Register(DailyResetFn fn);
    void OnDailyReset(uint32 uiDayKey);
private:
    std::vector<DailyResetFn> m_kFns;
};

class NpcScheduleSink {
public:
    static NpcScheduleSink& Get();
    void Register(NpcScheduleFn fn);
    void OnTransition(uint32 uiNpcId, uint16 uiMapId, bool bSpawn);
private:
    std::vector<NpcScheduleFn> m_kFns;
};

} // namespace fiesta
#endif
