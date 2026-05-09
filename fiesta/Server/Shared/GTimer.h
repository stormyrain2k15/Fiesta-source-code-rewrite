// Server/Shared/GTimer.h
// high-resolution timer.
#ifndef FIESTA_GTIMER_H
#define FIESTA_GTIMER_H
#include "ShineTypes.h"

namespace fiesta {

class GTimer {
public:
    GTimer();
    void   Reset();
    uint64 GetMillis() const;                       // monotonic ms since Reset
    uint64 GetMicros() const;
    static uint64 NowMillis();                      // wall clock helper
private:
    LARGE_INTEGER m_kFreq;
    LARGE_INTEGER m_kStart;
};

} // namespace fiesta
#endif
