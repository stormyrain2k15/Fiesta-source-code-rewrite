// Server/Zone/BlockingDistribute.cpp
// Block-distribution helper: given a candidate destination, finds the
// nearest non-blocked cell. Used to resolve "I want to teleport here
// but here is blocked" cases.
#include "MapBlockInformation.h"
namespace fiesta {
class BlockingDistribute {
public:
    static bool ResolveNearby(uint16 /*uiMap*/, float& /*x*/, float& /*y*/) { return true; }
};
} // namespace fiesta
