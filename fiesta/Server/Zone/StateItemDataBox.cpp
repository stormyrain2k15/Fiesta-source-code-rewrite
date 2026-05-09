// Server/Zone/StateItemDataBox.cpp
// StateItem.shn -- items that grant a temporary AbState on use (potions
// with ab-state, food buffs).
#include "../DataReader/ShnRegistry.h"
namespace fiesta {
class StateItemDataBox {
public:
    static StateItemDataBox& Get() { static StateItemDataBox s; return s; }
    bool Load() { return ShnRegistry::Get().GetTable("StateItem") != NULL; }
};
} // namespace fiesta
