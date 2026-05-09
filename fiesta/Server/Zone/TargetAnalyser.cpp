// Server/Zone/TargetAnalyser.cpp
// Validates that a candidate target is legal for combat or skill use.
// WIRE-04 (Lyra, May 2026): lifted out of anonymous namespace; function
// is now in namespace fiesta (no anonymous wrapper) so Battle.cpp can call it.
#include "TargetAnalyser.h"
#include "ShineObject.h"

namespace fiesta {

bool IsLegalTarget(ShineObject* pkA, ShineObject* pkT, float fMaxDistSq) {
    if (!pkA || !pkT || pkT->GetHP() <= 0) return false;
    float dx = pkA->GetPos().x - pkT->GetPos().x;
    float dy = pkA->GetPos().z - pkT->GetPos().z;
    return dx*dx + dy*dy <= fMaxDistSq;
}

} // namespace fiesta
