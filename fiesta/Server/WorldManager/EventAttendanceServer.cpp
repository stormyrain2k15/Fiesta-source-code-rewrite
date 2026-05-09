// Server/WorldManager/EventAttendanceServer.cpp
// Daily login-reward authority. The day-key rolls at local midnight; on
// each player login the WM marks attendance via WMCharDBClient::EventAttend
// (-> p_EventAttendance_Mark on CharDB). The reward grant itself is read
// off EventAttendance.shn in-zone, but the "did I claim today?" flag is
// world-global so it survives map / zone changes.
#include "WMServices.h"
#include "WMCharDBClient.h"
#include "../Shared/GTimer.h"
#include "../Shared/ShineLogSystem.h"

namespace fiesta {

EventAttendanceServer& EventAttendanceServer::Get() { static EventAttendanceServer s; return s; }

static uint32 LocalDayKey() {
    SYSTEMTIME st; GetLocalTime(&st);
    return (uint32)st.wYear * 10000u + (uint32)st.wMonth * 100u + (uint32)st.wDay;
}

void EventAttendanceServer::OnLogin(CharID c) {
    uint32 uiToday = LocalDayKey();
    if (m_uiCurrentDay == 0) m_uiCurrentDay = uiToday;
    // Mark today's attendance for this character. Day index inside the
    // 30-slot calendar is derived from day-of-month modulo 30.
    SYSTEMTIME st; GetLocalTime(&st);
    uint8 uiDay = (uint8)((st.wDay - 1) % 30);
    WMCharDBClient::Get().EventAttendanceMark(c, uiDay);
}

void EventAttendanceServer::Tick() {
    uint32 uiToday = LocalDayKey();
    if (uiToday != m_uiCurrentDay) {
        m_uiCurrentDay = uiToday;
        SHINELOG_INFO("EventAttendance day rolled to %u", uiToday);
    }
}

} // namespace fiesta
