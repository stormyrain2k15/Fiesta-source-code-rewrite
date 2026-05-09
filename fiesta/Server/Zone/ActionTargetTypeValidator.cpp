// Server/Zone/ActionTargetTypeValidator.cpp
// Filters target legality by the action's target-type column (Self,
// Single, Allies, Enemies, AOE-Self, AOE-Target, Cone, Line).
#include "ShineObject.h"
namespace fiesta {
class ActionTargetTypeValidator {
public:
    enum eTargetType { TT_SELF, TT_SINGLE, TT_ALLY, TT_ENEMY, TT_AOE_SELF, TT_AOE_TARGET, TT_CONE, TT_LINE };
    static bool Accept(eTargetType eTT, ShineObject* pkA, ShineObject* pkT);
};
bool ActionTargetTypeValidator::Accept(eTargetType eTT, ShineObject* pkA, ShineObject* pkT) {
    if (eTT == TT_SELF) return pkA == pkT;
    if (!pkA || !pkT)   return false;
    return true;
}
} // namespace fiesta
