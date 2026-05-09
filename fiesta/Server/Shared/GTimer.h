// Server/Shared/GTimer.h
// 01 Foundation -- high-resolution timer.
// EVIDENCE: PDB_CONFIRMED  symbol: GTimer
#ifndef FIESTA_GTIMER_H
#define FIESTA_GTIMER_H
#include "ShineTypes.h"

namespace fiesta {

class GTimer {
public:
    GTimer();
    void   Reset();                                 // EV_PDB
    uint64 GetMillis() const;                       // monotonic ms since Reset
    uint64 GetMicros() const;
    static uint64 NowMillis();                      // wall clock helper
private:
    LARGE_INTEGER m_kFreq;
    LARGE_INTEGER m_kStart;
};

} // namespace fiesta
#endif
