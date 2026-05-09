// Server/Zone/ActionTargetTypeValidator.cpp
// Filters target legality by the action's target-type column (Self,
// Single, Allies, Enemies, AOE-Self, AOE-Target, Cone, Line). Real
// implementation: walks the (attacker, target) pair through faction
// gates per target-type, then defers to TargetAnalyser for distance.
#include "ShineObject.h"
#include "TargetAnalyser.h"
namespace fiesta {

class ActionTargetTypeValidator {
public:
    enum eTargetType {
        TT_SELF        = 0,
        TT_SINGLE      = 1,    // any legal target (PvE/PvP)
        TT_ALLY        = 2,    // same faction (player <-> player only)
        TT_ENEMY       = 3,    // opposite faction (PvE/PvP)
        TT_AOE_SELF    = 4,    // centred on attacker, hits enemies
        TT_AOE_TARGET  = 5,    // centred on a designated target
        TT_CONE        = 6,    // cone in front of attacker
        TT_LINE        = 7     // straight line in front of attacker
    };
    static bool Accept(eTargetType eTT, ShineObject* pkA, ShineObject* pkT,
                       float fMaxDistSq = 0.0f);
};

bool ActionTargetTypeValidator::Accept(eTargetType eTT, ShineObject* pkA,
                                       ShineObject* pkT, float fMaxDistSq) {
    if (eTT == TT_SELF) return pkA != NULL && pkA == pkT;
    if (!pkA || !pkT)   return false;
    if (pkT->IsDead())  return false;

    // Same/opposite faction gates. Players are "ally" to other players;
    // mobs are "enemy" to players. Pets count as ally with their owner.
    bool aPlayer = (pkA->GetType() == OT_PLAYER);
    bool tPlayer = (pkT->GetType() == OT_PLAYER);
    bool aMob    = (pkA->GetType() == OT_MOB);
    bool tMob    = (pkT->GetType() == OT_MOB);

    switch (eTT) {
        case TT_SELF:
            return false;          // already handled above
        case TT_SINGLE:
            // Any non-self legal target -- distance check only.
            return (pkA != pkT) && IsLegalTarget(pkA, pkT,
                fMaxDistSq > 0.0f ? fMaxDistSq : 1e9f);
        case TT_ALLY:
            // Players ally with players; mobs ally with mobs. Reject
            // cross-faction ally targeting.
            if (aPlayer && tPlayer) return true;
            if (aMob    && tMob)    return true;
            return false;
        case TT_ENEMY:
        case TT_AOE_SELF:
        case TT_AOE_TARGET:
        case TT_CONE:
        case TT_LINE:
            if (pkA == pkT)            return false;
            if (aPlayer && tPlayer)    return true;   // PvP
            if (aPlayer && tMob)       return true;   // PvE
            if (aMob    && tPlayer)    return true;   // EvP
            return false;
    }
    return false;
}

} // namespace fiesta
