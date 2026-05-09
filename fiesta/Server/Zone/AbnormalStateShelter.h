// Server/Zone/AbnormalStateShelter.h
// Shelter persists an active AbState ledger across logout / death /
// link-loss. The save eligibility for each row is gated through
// `AbStateSaveTypeInfo.shn` -- a row's `AbStateSaveType` column points
// at one of those entries, which carries three booleans:
//
//   IsSaveLink   -- persist on link-loss / disconnect
//   IsSaveDie    -- persist on death (otherwise dropped on respawn)
//   IsSaveLogoff -- persist on clean logout
//
// `Save()` walks the ledger and only persists rows whose save-type
// matches the trigger; `Load()` issues the async query on login and
// the response handler hydrates the ledger back.
#ifndef FIESTA_ZONE_ABNORMALSTATESHELTER_H
#define FIESTA_ZONE_ABNORMALSTATESHELTER_H
#include "../Shared/ShineTypes.h"
namespace fiesta {

class ShinePlayer;

enum eAbStateShelterTrigger {
    SHELTER_LINK   = 0,
    SHELTER_DIE    = 1,
    SHELTER_LOGOFF = 2
};

class AbnormalStateShelter {
public:
    // Persist an active row directly. Used by tests and the GM debug
    // panel; the production callers use `OnTrigger` below.
    static void Save(uint32 cid, uint32 uiAbStateId, uint32 uiRemainMs);
    // Persist every still-active row whose save-type permits the
    // trigger; the per-row remain-ms is computed from the ledger expiry.
    static void OnTrigger(ShinePlayer* pkPlayer, eAbStateShelterTrigger eTrig);
    // Async load all saved rows for a cid; the ack handler hydrates
    // the ledger when it returns.
    static void Load(uint32 cid);
    // Hook used by the CharDB ack handler -- pushes one (ab,strength,
    // remainMs) tuple into the player's ledger.
    static bool ApplyLoaded(ShinePlayer* pkPlayer, uint32 uiAbStateId,
                            uint32 uiStrength, uint32 uiRemainMs);
};

} // namespace fiesta
#endif
