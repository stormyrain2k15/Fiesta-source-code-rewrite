// Server/Zone/SubAbstatePriority.cpp
// SubAbState priority resolver: when applying an AbState, check whether
// the new state outranks any existing state of the same family.
// WIRE-05 (Lyra, May 2026): lifted out of anonymous namespace; wired
// to AbnormalState::ShouldStack().
#include "SubAbstatePriority.h"

namespace fiesta {

// Returns true if the new abstate id should be applied over the old one.
// Rule: higher numeric id wins (later-added states in the SHN are stronger
// by convention). Ties: newer replaces older.
// TODO: replace with actual SubAbState.shn Priority field lookup once
// the priority column RE is complete.
bool SubAbstatePriority::ShouldReplace(uint32 uiNew, uint32 uiOld) {
    return uiNew >= uiOld;
}

} // namespace fiesta
