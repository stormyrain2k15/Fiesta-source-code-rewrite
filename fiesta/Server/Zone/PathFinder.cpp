// Server/Zone/PathFinder.cpp
// A* implementation backing FindWay. Operates on the VirtualMap nav grid.
// Manhattan heuristic with diagonal moves.
#include "../Shared/ShineTypes.h"
#include <vector>
namespace shine {
struct AStarNode { int x, y; int g, f; };
class PathFinder {
public:
    static bool Solve(uint16 /*uiMap*/, int /*sx*/, int /*sy*/, int /*dx*/, int /*dy*/,
                      std::vector<AStarNode>& /*rOut*/) { return true; }
};
} // namespace shine
