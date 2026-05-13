// Server/Zone/InterBroadcastSinks.cpp
#include "InterBroadcastSinks.h"

namespace shine {

DailyResetSink& DailyResetSink::Get() { static DailyResetSink s; return s; }
void DailyResetSink::Register(DailyResetFn fn) { if (fn) m_kFns.push_back(fn); }
void DailyResetSink::OnDailyReset(uint32 uiDayKey) {
    for (size_t i = 0; i < m_kFns.size(); ++i) m_kFns[i](uiDayKey);
}

NpcScheduleSink& NpcScheduleSink::Get() { static NpcScheduleSink s; return s; }
void NpcScheduleSink::Register(NpcScheduleFn fn) { if (fn) m_kFns.push_back(fn); }
void NpcScheduleSink::OnTransition(uint32 uiNpcId, uint16 uiMapId, bool bSpawn) {
    for (size_t i = 0; i < m_kFns.size(); ++i) m_kFns[i](uiNpcId, uiMapId, bSpawn);
}

} // namespace shine
