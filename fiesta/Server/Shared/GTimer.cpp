// Server/Shared/GTimer.cpp
// 01 Foundation -- high-resolution timer impl.
// EVIDENCE: PDB_CONFIRMED
#include "GTimer.h"

namespace fiesta {

GTimer::GTimer() {
    QueryPerformanceFrequency(&m_kFreq);
    Reset();
}

void GTimer::Reset() {
    QueryPerformanceCounter(&m_kStart);
}

uint64 GTimer::GetMillis() const {
    LARGE_INTEGER kNow; QueryPerformanceCounter(&kNow);
    return (uint64)((kNow.QuadPart - m_kStart.QuadPart) * 1000ULL / m_kFreq.QuadPart);
}

uint64 GTimer::GetMicros() const {
    LARGE_INTEGER kNow; QueryPerformanceCounter(&kNow);
    return (uint64)((kNow.QuadPart - m_kStart.QuadPart) * 1000000ULL / m_kFreq.QuadPart);
}

uint64 GTimer::NowMillis() {
    FILETIME ft; GetSystemTimeAsFileTime(&ft);
    ULARGE_INTEGER u; u.LowPart = ft.dwLowDateTime; u.HighPart = ft.dwHighDateTime;
    return (uint64)(u.QuadPart / 10000ULL); // 100ns ticks -> ms
}

} // namespace fiesta
