// Server/Zone/ItemActionTable.cpp
// ItemAction.shn -- each consumable's effect descriptor (heal HP, cast
// AbState, port to map, learn skill, summon mob). Effect dispatch is in
// ItemActions.cpp.
#include "../DataReader/ShnRegistry.h"
namespace shine {
class ItemActionTable {
public:
    static ItemActionTable& Get() { static ItemActionTable s; return s; }
    bool Load() { return ShnRegistry::Get().GetTable("ItemAction") != NULL; }
};
} // namespace shine
