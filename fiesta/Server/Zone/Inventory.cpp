// Server/Zone/Inventory.cpp
#include "Inventory.h"
#include "../DataReader/Schemas.h"
#include "../DataReader/Tables.h"  // back-compat aliases

namespace fiesta {

Inventory::Inventory() : m_iMoney(0) {}

bool Inventory::Add(const ShineItem& k) { m_kItems.push_back(k); return true; }
bool Inventory::Remove(uint32 id) {
    for (size_t i = 0; i < m_kItems.size(); ++i)
        if (m_kItems[i].uiItemId == id) { m_kItems.erase(m_kItems.begin() + i); return true; }
    return false;
}
bool Inventory::Move(uint16 a, uint16 b) {
    ShineItem* pa = NULL, *pb = NULL;
    for (size_t i = 0; i < m_kItems.size(); ++i) {
        if (m_kItems[i].uiSlot == a) pa = &m_kItems[i];
        if (m_kItems[i].uiSlot == b) pb = &m_kItems[i];
    }
    if (pa) pa->uiSlot = b;
    if (pb) pb->uiSlot = a;
    return pa != NULL;
}
bool Inventory::Equip(uint32 id) {
    for (size_t i = 0; i < m_kItems.size(); ++i)
        if (m_kItems[i].uiItemId == id) { m_kItems[i].bEquipped = 1; return true; }
    return false;
}
bool Inventory::Unequip(uint32 id) {
    for (size_t i = 0; i < m_kItems.size(); ++i)
        if (m_kItems[i].uiItemId == id) { m_kItems[i].bEquipped = 0; return true; }
    return false;
}

bool ItemAuthority::CanEquip(ShinePlayer* pk, const ShineItem& k) {
    if (!pk) return false;
    const ItemInfoRow* p = ITableBase<ItemInfoRow>::ms_pkTable
                           ? ITableBase<ItemInfoRow>::ms_pkTable->Find(k.uiInxName) : NULL;
    (void)p;  // schema fields wire in pass 2 (NeededLevel / NeededClass).
    return true; // permissive while bind isn't pinned

}

bool ItemAuthority::CanUse(ShinePlayer* pk, const ShineItem& k) { return CanEquip(pk, k); }

uint16 EquipEnumChanger::ToEquipSlot(ItemID) { return 0; } // EV_VERIFY

} // namespace fiesta
