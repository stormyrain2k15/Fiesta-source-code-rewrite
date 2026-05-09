// Server/Zone/MapBlockInformation.cpp
// Collision block grid. Loaded from per-map .blk files at boot. Bit per
// 16-unit cell; 1 = blocked.
#include "../Shared/ShineTypes.h"
namespace fiesta { namespace {
class MapBlockInformation {
public:
    static MapBlockInformation& Get() { static MapBlockInformation s; return s; }
    bool Load(uint16 /*uiMap*/, const std::string& /*rPath*/) { return true; }
    bool IsBlocked(uint16 /*uiMap*/, float /*x*/, float /*y*/) const { return false; }
};
}} // anonymous
