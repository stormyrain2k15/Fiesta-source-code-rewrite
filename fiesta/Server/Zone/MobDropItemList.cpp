// Server/Zone/MobDropItemList.cpp
// Per-mob drop configuration as currently composed at runtime. Pulls
// from MobDropItem.shn at boot and exposes per-mob lookup; the actual
// roll happens in DropResolver.cpp.
#include "../DataReader/ShnRegistry.h"
namespace shine {
class MobDropItemList {
public:
    static MobDropItemList& Get() { static MobDropItemList s; return s; }
    bool Load() { return ShnRegistry::Get().GetTable("MobDropItem") != NULL; }
};
} // namespace shine
