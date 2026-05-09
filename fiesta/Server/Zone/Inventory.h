// Server/Zone/Inventory.h
// 15 -- Inventory / Equipment / item authority.
// EVIDENCE: PDB_CONFIRMED  symbol: Inventory, ItemTotalInformation, ShineItem, ItemAuthority,
//                                  EquipEnumChanger
#ifndef FIESTA_ZONE_INVENTORY_H
#define FIESTA_ZONE_INVENTORY_H
#include "ShineObject.h"
#include <vector>

namespace fiesta {

struct ShineItem {
    uint32 uiItemId;
    ItemID uiInxName;
    uint16 uiSlot;       // 0..N inventory slot
    uint16 uiQty;
    uint16 uiEndure;
    uint16 uiEnchant;
    uint8  bEquipped;
};

class Inventory {
public:
    Inventory();
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
};

class ItemAuthority {
public:
    static bool CanEquip(ShinePlayer* pk, const ShineItem& kItem);
    static bool CanUse  (ShinePlayer* pk, const ShineItem& kItem);
};

class EquipEnumChanger {
public:
    static uint16 ToEquipSlot(ItemID uiInxName);
};

} // namespace fiesta
#endif
