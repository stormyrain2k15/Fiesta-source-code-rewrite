// Server/Zone/ShineItemReloc.cpp
// Move an item from one slot to another within the inventory grid.
// Wrapper over Inventory::Move.
#include "Inventory.h"
#include "ShineObject.h"
namespace shine {
class ShineItemReloc {
public:
    static bool Move(ShinePlayer* pkP, uint16 uiFrom, uint16 uiTo) {
        return pkP && pkP->Inv().Move(uiFrom, uiTo);
    }
};
} // namespace shine
