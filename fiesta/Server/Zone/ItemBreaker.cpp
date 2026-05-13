// Server/Zone/ItemBreaker.cpp
// "Break on use" or "break on enchant fail" cleanup -- removes the row
// from inventory and persists the delete.
#include "Inventory.h"
#include "CharDBClient.h"
#include "ShineObject.h"
namespace shine {
class ItemBreaker {
public:
    static bool Break(ShinePlayer* pkP, uint32 uiItemId) {
        if (!pkP) return false;
        return pkP->Inv().Remove(uiItemId);
    }
};
} // namespace shine
