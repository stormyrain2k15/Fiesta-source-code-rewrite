// Server/Zone/TargetAnalyser.cpp
// Validates that a candidate target is legal. Class declared in Battle.h.
#include "ShineObject.h"
#include "Battle.h"
namespace fiesta { namespace {
inline bool IsLegalTarget(ShineObject* pkA, ShineObject* pkT, float fMaxDistSq) {
    if (!pkA || !pkT || pkT->GetHP() <= 0) return false;
    float dx = pkA->GetX() - pkT->GetX();
    float dy = pkA->GetZ() - pkT->GetZ();
    return dx*dx + dy*dy <= fMaxDistSq;
}
}} // anonymous
