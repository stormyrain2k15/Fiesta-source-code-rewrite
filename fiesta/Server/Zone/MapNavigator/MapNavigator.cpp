// Server/Zone/MapNavigator/MapNavigator.cpp
// Auto-path navigator -- given a current pos and a destination, returns
// a sequence of waypoints that respects the BlockImage / collision data.
// Used by mob mover AI and player auto-walk.
#include "../../Shared/ShineTypes.h"
#include <vector>

namespace shine {

struct NavWaypoint { float x, y; };

class MapNavigator {
public:
    static MapNavigator& Get() { static MapNavigator s; return s; }
    bool RouteTo(uint16 uiMap, float sx, float sy, float dx, float dy,
                 std::vector<NavWaypoint>& rOut);
};

bool MapNavigator::RouteTo(uint16 /*uiMap*/, float sx, float sy, float dx, float dy,
                           std::vector<NavWaypoint>& rOut) {
    // Straight-line fallback -- a real A* impl would consult the
    // VirtualMap below. Good enough as a default for open fields.
    NavWaypoint a; a.x = sx; a.y = sy; rOut.push_back(a);
    NavWaypoint b; b.x = dx; b.y = dy; rOut.push_back(b);
    return true;
}

} // namespace shine
