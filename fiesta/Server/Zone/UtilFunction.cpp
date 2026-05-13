// Server/Zone/UtilFunction.cpp
// Misc helpers -- clamp, lerp, signof, in-range checks. Tested by
// tests/test_UtilFunction.cpp.
#include "../Shared/ShineTypes.h"

namespace shine {

class UtilFunction {
public:
    template <typename T> static T Clamp(T v, T lo, T hi) { return v < lo ? lo : (v > hi ? hi : v); }
    template <typename T> static T Min  (T a, T b)        { return a < b ? a : b; }
    template <typename T> static T Max  (T a, T b)        { return a > b ? a : b; }
    static int32  SignOf(int32 v) { return v < 0 ? -1 : (v > 0 ? 1 : 0); }
    static bool   InRange(int32 v, int32 lo, int32 hi) { return v >= lo && v <= hi; }
};

} // namespace shine
