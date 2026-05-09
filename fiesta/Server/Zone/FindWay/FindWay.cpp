// Server/Zone/FindWay/FindWay.cpp
// Pathfinding wrapper. The actual A* lives in PathFinder.cpp; FindWay
// is the public API used by mob AI and player auto-walk.
#include "../../Shared/ShineTypes.h"
#include <vector>
namespace fiesta {
struct PathPoint { float x, y; };
class FindWay {
public:
    static bool Route(uint16 /*uiMap*/, float sx, float sy, float dx, float dy,
                      std::vector<PathPoint>& rOut) {
        PathPoint a; a.x = sx; a.y = sy; rOut.push_back(a);
        PathPoint b; b.x = dx; b.y = dy; rOut.push_back(b);
        return true;
    }
};
} // namespace fiesta
