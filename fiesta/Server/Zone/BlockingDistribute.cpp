// Server/Zone/BlockingDistribute.cpp
// Block-distribution helper: given a candidate destination, finds the
// nearest non-blocked cell. Used to resolve "I want to teleport here
// but here is blocked" cases.
#include "MapField.h"

namespace fiesta {

class BlockingDistribute {
public:
    // Spiral out from (x,y) by world units until a walkable cell is
    // found. Mutates x/y to the resolved coordinate. Returns false if
    // the search radius exhausts without finding open ground.
    static bool ResolveNearby(Field& rField, float& x, float& y) {
        if (!rField.Blocks().IsBlockedWorld(x, y)) return true;
        const int32 kStep    = kMapBlockCellSize;
        const int32 kMaxRing = 32;          // ~512 world units max
        for (int32 ring = 1; ring <= kMaxRing; ++ring) {
            for (int32 dx = -ring; dx <= ring; ++dx) {
                for (int32 dy = -ring; dy <= ring; ++dy) {
                    // Only walk the outer ring -- inner rings already
                    // tested.
                    if (dx > -ring && dx < ring && dy > -ring && dy < ring) continue;
                    float tx = x + (float)(dx * kStep);
                    float ty = y + (float)(dy * kStep);
                    if (!rField.Blocks().IsBlockedWorld(tx, ty)) {
                        x = tx; y = ty; return true;
                    }
                }
            }
        }
        return false;
    }
};

} // namespace fiesta
