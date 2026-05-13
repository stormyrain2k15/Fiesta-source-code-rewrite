// Server/Zone/Inventory.h
// Inventory / Equipment / item authority.
//                                  EquipEnumChanger
#ifndef SHINE_ZONE_INVENTORY_H
#define SHINE_ZONE_INVENTORY_H
#include "../Shared/ShineTypes.h"
#include <string>
#include <vector>

namespace shine {

class ShinePlayer;

struct ShineItem {
    ShineItem() : uiItemId(0), uiDbItemKey(0), uiInxName(0), uiSlot(0),
                  uiQty(0), uiEndure(0), uiEnchant(0), bEquipped(0)
    { for (int i = 0; i < 5; ++i) aRandomOption[i] = 0; }
    uint32 uiItemId;
    uint64 uiDbItemKey;  // server-side IDENTITY from p_Item_Create; used as the
                         // delete / set-option key in subsequent CharDB calls
    ItemID uiInxName;
    uint16 uiSlot;       // 0..N inventory slot
    uint16 uiQty;
    uint16 uiEndure;
    uint16 uiEnchant;
    uint8  bEquipped;
    // Resolved at insert time when the item is created from a drop / quest
    // reward where only the string item-index is known. Empty after the
    // ItemInfo lookup has converted it to `uiInxName`.
    std::string kItemIndex;
    // Up to 5 random-option roll values (Str / Con / Dex / Int / Men).
    // 0 = no roll. Filled by `DropResolver`; consumed by stat compositor
    // and the equip summary builder.
    uint16 aRandomOption[5];
};

class Inventory {
public:
    Inventory();
    void   SetOwner(CharID c) { m_uiOwner = c; }
    CharID GetOwner() const   { return m_uiOwner; }
    bool   Add    (const ShineItem& kItem);
    bool   Remove (uint32 uiItemId);
    bool   Move   (uint16 uiFromSlot, uint16 uiToSlot);
    bool   Equip  (uint32 uiItemId);
    bool   Unequip(uint32 uiItemId);
    const std::vector<ShineItem>& All() const { return m_kItems; }
    int64  Money() const { return m_iMoney; }
    void   AddMoney(int64 v) { m_iMoney += v; if (m_iMoney < 0) m_iMoney = 0; }
private:
    std::vector<ShineItem> m_kItems;
    int64                  m_iMoney;
    CharID                 m_uiOwner;
};

class ItemAuthority {
public:
    static bool CanEquip(ShinePlayer* pk, const ShineItem& kItem);
    static bool CanUse  (ShinePlayer* pk, const ShineItem& kItem);
    static bool CanSell (const ShineItem& kItem);
};

class EquipEnumChanger {
public:
    static uint16 ToEquipSlot(ItemID uiInxName);
};

} // namespace shine
#endif
