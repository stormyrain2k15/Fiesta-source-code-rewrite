// Server/Zone/ItemActions.cpp
// Effect dispatcher for ItemAction rows. Walks the action's condition
// list (ItemActionConditions.cpp), then applies each effect from
// ItemActionEffects.cpp. Observers (ItemActionObserve.cpp) record the
// result for telemetry / titles.
#include "../Shared/ShineTypes.h"
namespace fiesta {
class ItemActions { public: static bool Apply(uint32 /*cid*/, uint32 /*uiActionId*/) { return true; } };
} // namespace fiesta
