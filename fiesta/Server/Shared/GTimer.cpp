// Server/Shared/GTimer.cpp
// high-resolution timer impl.
#include "GTimer.h"

namespace shine {

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

} // namespace shine
