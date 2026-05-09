// Server/Zone/SubAbstatePriority.cpp
// SubAbState priority resolver: when applying an AbState, check whether
// the new state outranks any existing state of the same family.
// WIRE-05 (Lyra, May 2026): lifted out of anonymous namespace; wired
// to AbnormalState::ShouldStack().
// Pass 6 (Feb 2026): looks up the SubAbState.shn `Priority` column when
// available (the data sample uses it to encode "higher = stronger");
// falls back to the documented "later-id wins" convention if the table
// or the row isn't loaded.
#include "SubAbstatePriority.h"
#include "../DataReader/ShnRegistry.h"
#include "../DataReader/ShnFile.h"

namespace fiesta {

static int32 LookupPriority(uint32 uiAb) {
    const ShnFile* t = ShnRegistry::Get().GetTable("SubAbState");
    if (!t) return -1;
    for (uint32 i = 0; i < t->RecordCount(); ++i) {
        if (ShnGetU32(*t, i, "ID") == uiAb)
            return (int32)ShnGetU32(*t, i, "Priority");
    }
    return -1;
}

// Returns true if the new abstate id should replace the existing one.
// Priority sourcing order:
//   1. SubAbState.shn `Priority` column for the matching `ID` row.
//   2. Fallback: numeric id ordering ("later-added rows are stronger").
// Tunable via the SubAbState.shn data drop -- no constant in code.
bool SubAbstatePriority::ShouldReplace(uint32 uiNew, uint32 uiOld) {
    int32 pNew = LookupPriority(uiNew);
    int32 pOld = LookupPriority(uiOld);
    if (pNew >= 0 && pOld >= 0) return pNew >= pOld;
    return uiNew >= uiOld;
}

} // namespace fiesta
