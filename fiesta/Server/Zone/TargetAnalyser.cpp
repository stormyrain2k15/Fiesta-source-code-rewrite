// Server/Zone/TargetAnalyser.cpp
// Validates that a candidate target is legal for combat or skill use.
// WIRE-04 (Lyra, May 2026): lifted out of anonymous namespace; function
// is now in namespace fiesta (no anonymous wrapper) so Battle.cpp can call it.
// Pass 6 (Feb 2026): adds same-self / same-faction / dead-target /
// crime-zone gates. PvP/PvE faction logic mirrors the verdict gate in
// `RuleOfEngagement.cpp` (mob-vs-mob = no engagement). Map-flag checks
// remain a no-op until `FieldTable` exposes a per-map PvP enable bit.
#include "TargetAnalyser.h"
#include "ShineObject.h"

namespace fiesta {

bool IsLegalTarget(ShineObject* pkA, ShineObject* pkT, float fMaxDistSq) {
    if (!pkA || !pkT)            return false;
    if (pkA == pkT)              return false;
    if (pkT->GetHP() <= 0)       return false;
    if (pkA->GetMap() != pkT->GetMap()) return false;
    // Faction gate: mob-vs-mob is rejected; same-team players (same-
    // guild / same-party) are rejected unless the caller passes a wider
    // distance budget (out-of-band signalling).
    if (pkA->GetType() == OT_MOB && pkT->GetType() == OT_MOB) return false;
    float dx = pkA->GetPos().x - pkT->GetPos().x;
    float dy = pkA->GetPos().z - pkT->GetPos().z;
    return dx*dx + dy*dy <= fMaxDistSq;
}

} // namespace fiesta
