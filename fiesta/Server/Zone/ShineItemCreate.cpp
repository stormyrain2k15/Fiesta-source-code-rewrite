// Server/Zone/ShineItemCreate.cpp
// Create a new item instance and write it to CharDB. Returns the
// CharDB-assigned identity (uiDbItemKey) on the async completion.
#include "Inventory.h"
#include "CharDBClient.h"
namespace fiesta {
class ShineItemCreate {
public:
    static bool Create(uint32 cid, uint32 uiItemId, uint16 uiCount, int32 iEnchant) {
        return CharDBClient::Get().ItemCreate(cid, uiItemId, uiCount, iEnchant);
    }
};
} // namespace fiesta
