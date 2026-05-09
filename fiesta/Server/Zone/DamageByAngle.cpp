// Server/Zone/DamageByAngle.cpp
// Angular damage modifier -- back-attack and side-attack get bonus
// damage. Reads the attacker's facing vs the target's body angle.
#include "../Shared/ShineTypes.h"
namespace fiesta {
class DamageByAngle {
public:
    // Returns a multiplier in 1/100 units. 100 = baseline. 130 = back-stab.
    static int32 ScalerX100(float fAttackerYaw, float fTargetYaw) {
        float d = fAttackerYaw - fTargetYaw;
        while (d >  3.14159265f) d -= 6.28318530f;
        while (d < -3.14159265f) d += 6.28318530f;
        if (d < -2.0f || d > 2.0f) return 130;
        if (d < -1.0f || d > 1.0f) return 110;
        return 100;
    }
};
} // namespace fiesta
