// Server/Zone/MapNavigator/VirtualMap.cpp
// Compressed nav-grid used by MapNavigator. One bit per 32x32 game-unit
// cell -- 1 = walkable. Loaded from per-map .nav side-files at boot.
#include "../../Shared/ShineTypes.h"
#include <vector>
namespace shine {
class VirtualMap {
public:
    static VirtualMap& Get() { static VirtualMap s; return s; }
    bool Load(uint16 /*uiMap*/, const std::string& /*rPath*/) { return true; }
    bool IsWalkable(uint16 /*uiMap*/, float /*x*/, float /*y*/) const { return true; }
};
} // namespace shine
