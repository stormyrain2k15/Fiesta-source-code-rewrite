// Server/Zone/ShineRadian.cpp
// Radian / yaw helpers -- normalize-to-(-pi,pi), diff, lerp.
#include <math.h>
#include "../Shared/ShineTypes.h"
namespace shine {
class ShineRadian {
public:
    static float Normalize(float r) {
        while (r >  3.14159265f) r -= 6.28318530f;
        while (r < -3.14159265f) r += 6.28318530f;
        return r;
    }
    static float Diff(float a, float b) { return Normalize(a - b); }
};
} // namespace shine
