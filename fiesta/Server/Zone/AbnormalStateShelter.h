// Server/Zone/AbnormalStateShelter.h
// WIRE-16 (Lyra, May 2026)
#ifndef FIESTA_ZONE_ABNORMALSTATESHELTER_H
#define FIESTA_ZONE_ABNORMALSTATESHELTER_H
#include "../Shared/ShineTypes.h"
namespace fiesta {
class AbnormalStateShelter {
public:
    // Persist a remaining-time AbState to CharDB on logout/death.
    static void Save(uint32 cid, uint32 uiAbStateId, uint32 uiRemainMs);
    // Load all saved AbStates for a character on login.
    static void Load(uint32 cid);
};
} // namespace fiesta
#endif
