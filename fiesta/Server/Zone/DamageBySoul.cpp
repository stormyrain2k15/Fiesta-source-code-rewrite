// Server/Zone/DamageBySoul.cpp
// SoulStone-charge damage modifier. Each accumulated soul charge boosts
// the next damaging skill until consumed.
#include "../Shared/ShineTypes.h"
namespace shine {
class DamageBySoul {
public:
    static int32 ScalerX100(uint32 uiSouls) { return 100 + (int32)(uiSouls * 5); }
};
} // namespace shine
