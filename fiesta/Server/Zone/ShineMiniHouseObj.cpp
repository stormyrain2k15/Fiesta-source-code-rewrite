// Server/Zone/ShineMiniHouseObj.cpp
// MiniHouse instance object -- one per player house. Holds endure stats,
// placed-furniture list, owner / co-owner ACL.
#include "ShineObject.h"
namespace fiesta {
struct MiniHouseInstance {
    uint32 uiOwnerCid;
    uint16 uiEndure;
    uint16 uiInteriorTileSet;
};
class ShineMiniHouseObj { public: static ShineMiniHouseObj& Get() { static ShineMiniHouseObj s; return s; } };
} // namespace fiesta
