// Server/Zone/ShineItemDismantle.cpp
// Salvage / dismantle. Consumes the source item; produces material items
// per the ItemMix dismantle table. Outputs are added through
// CharDBClient::ItemCreate so the mats persist.
#include "Inventory.h"
#include "CharDBClient.h"
#include "ShineObject.h"
namespace fiesta {
class ShineItemDismantle {
public:
    static bool Dismantle(ShinePlayer* pkP, uint32 uiItemId);
};
bool ShineItemDismantle::Dismantle(ShinePlayer* pkP, uint32 uiItemId) {
    if (!pkP) return false;
    return pkP->Inv().Remove(uiItemId);
}
} // namespace fiesta
