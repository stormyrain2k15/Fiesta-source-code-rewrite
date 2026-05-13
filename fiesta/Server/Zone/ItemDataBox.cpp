// Server/Zone/ItemDataBox.cpp
// Decoded ItemInfo / ItemInfoServer SHN tables. Existing ItemSystems.cpp
// + ZoneAssetLoader.cpp already populate the Schemas.h structs and store
// them via ITableBase<ItemInfoRow>::ms_pkTable; this file is the canonical
// alias.
#include "../DataReader/ShnRegistry.h"
namespace shine {
class ItemDataBox {
public:
    static ItemDataBox& Get() { static ItemDataBox s; return s; }
    bool Load() {
        return ShnRegistry::Get().GetTable("ItemInfo")       != NULL
            && ShnRegistry::Get().GetTable("ItemInfoServer") != NULL;
    }
};
} // namespace shine
