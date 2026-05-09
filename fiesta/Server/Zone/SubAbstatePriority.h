// Server/Zone/SubAbstatePriority.h
// WIRE-05 (Lyra, May 2026)
#ifndef FIESTA_ZONE_SUBABSTATEPRIORITY_H
#define FIESTA_ZONE_SUBABSTATEPRIORITY_H
#include "../Shared/ShineTypes.h"

namespace fiesta {

class SubAbstatePriority {
public:
    // Returns true if uiNew should replace uiOld when both target
    // the same sub-effect slot.
    static bool ShouldReplace(uint32 uiNew, uint32 uiOld);
};

} // namespace fiesta
#endif
