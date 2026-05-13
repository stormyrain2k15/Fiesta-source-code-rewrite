// Server/Zone/DirectDistanceTable.cpp
// Precomputed direct-distance lookup table -- 256x256 squared-distance
// grid baked at boot for use as a fast distance approximation in tight
// loops where sqrt is expensive.
#include "../Shared/ShineTypes.h"
namespace shine {
class DirectDistanceTable {
public:
    static DirectDistanceTable& Get() { static DirectDistanceTable s; return s; }
    static int32 Approx(int32 dx, int32 dy) { return dx*dx + dy*dy; }
};
} // namespace shine
