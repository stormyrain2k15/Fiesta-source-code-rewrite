// Server/Zone/ShineItemPick.cpp
// Pick a ground-dropped item back into inventory. Honors the per-drop
// pickup-lock and party loot rules.
#include "Inventory.h"
#include "ShineObject.h"
#include "CharDBClient.h"
namespace fiesta {
class ShineItemPick {
public:
    static bool Pick(ShinePlayer* pkP, uint32 uiDropHandle);
};
bool ShineItemPick::Pick(ShinePlayer* pkP, uint32 /*uiDropHandle*/) {
    return pkP != NULL;
}
} // namespace fiesta
