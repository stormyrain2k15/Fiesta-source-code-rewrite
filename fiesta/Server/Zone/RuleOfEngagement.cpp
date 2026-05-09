// Server/Zone/RuleOfEngagement.cpp
// PvP/PvE engagement rules. Class declared in Battle.h; this file owns
// the canonical filename + a simple verdict helper in an anonymous
// namespace to avoid ODR.
#include "ShineObject.h"
#include "Battle.h"
namespace fiesta { namespace {
inline int VerdictDefault(ShineObject* pkA, ShineObject* pkT) {
    if (!pkA || !pkT || pkA == pkT) return 0;
    if (pkA->GetType() == OT_MOB    && pkT->GetType() == OT_MOB)    return 0;
    if (pkA->GetType() == OT_PLAYER && pkT->GetType() == OT_PLAYER) return 2;
    return 1;
}
}} // anonymous
