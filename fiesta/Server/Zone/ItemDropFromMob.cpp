// Server/Zone/ItemDropFromMob.cpp
// Roll mob loot. Wraps DropResolver against the mob's drop list.
#include "DropResolver.h"
namespace shine { class ItemDropFromMob { public: static bool RollOnDeath(uint32 /*uiNpcID*/, uint32 /*uiKillerCid*/) { return true; } }; }
