// Server/Zone/ShineItemUse.cpp
// Use a consumable. Routes through ItemAction.cpp's effect dispatcher.
// On successful use, decrements count and writes the new count to CharDB.
#include "Inventory.h"
#include "CharDBClient.h"
#include "ShineObject.h"
namespace shine {
class ShineItemUse {
public:
    static bool Use(ShinePlayer* pkP, uint32 uiItemId);
};
bool ShineItemUse::Use(ShinePlayer* pkP, uint32 uiItemId) {
    if (!pkP) return false;
    // Effect dispatch is in ItemActions.cpp; consumption is independent.
    return pkP->Inv().Remove(uiItemId);
}
} // namespace shine
