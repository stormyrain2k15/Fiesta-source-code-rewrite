// Server/Zone/Inventory.cpp
#include "Inventory.h"
#include "GroupTables.h"
#include "ShineObject.h"
#include "CharDBClient.h"
#include "../Shared/ShineLogSystem.h"

namespace shine {

Inventory::Inventory() : m_iMoney(0), m_uiOwner(0) {}

bool Inventory::Add(const ShineItem& k) {
    // Any-free-slot allocator. If the caller passed a sentinel (0 or 0xFFFF)
    // OR the requested slot is already in use, remap to the first free slot.
    ShineItem rec = k;
    bool needsAlloc = (rec.uiSlot == 0 || rec.uiSlot == 0xFFFF);
    if (!needsAlloc) {
        for (size_t i = 0; i < m_kItems.size(); ++i) {
            if (m_kItems[i].uiSlot == rec.uiSlot) { needsAlloc = true; break; }
        }
    }
    if (needsAlloc) {
        bool aUsed[256]; for (int i = 0; i < 256; ++i) aUsed[i] = false;
        for (size_t i = 0; i < m_kItems.size(); ++i) {
            if (m_kItems[i].uiSlot < 256) aUsed[m_kItems[i].uiSlot] = true;
        }
        rec.uiSlot = 0;
        for (int i = 0; i < 256; ++i) { if (!aUsed[i]) { rec.uiSlot = (uint16)i; break; } }
    }
    if (rec.uiItemId == 0) {
        static uint32 s_uiNextId = 1;
        rec.uiItemId = s_uiNextId++;
    }
    rec.uiDbItemKey = 0;        // populated by OnItemCreateResponse
    m_kItems.push_back(rec);

    // Persist the new stack to World00_Character via the CharDB exe
    // (p_Item_Create). The exe fills the SQL-generated ItemKey into
    // uiDbItemKey via the CharDBClient response handler -- our local
    // uiItemId is the in-process correlation only.
    if (m_uiOwner != 0) {
        const ItemInfoRow* p = ItemTables::Get().FindItem((uint32)rec.uiInxName);
        std::string inx = p ? p->kInxName : rec.kItemIndex;
        CharDBClient::Get().ItemCreate(
            m_uiOwner, (uint32)rec.uiInxName, rec.uiQty,
            rec.uiSlot, rec.uiEndure, inx);
        // Latch the pending row so the response can attach the SQL key.
        CharDBClient::Get().NoteItemPending(m_uiOwner, rec.uiSlot, &m_kItems.back());
    }
    return true;
}

bool Inventory::Remove(uint32 id) {
    for (size_t i = 0; i < m_kItems.size(); ++i) {
        if (m_kItems[i].uiItemId == id) {
            // CharDB delete takes the SQL-generated key. If the response
            // hasn't arrived yet (uiDbItemKey == 0) we drop the persistent
            // delete on the floor; the row will be reconciled during the
            // next character logout via p_Char_Logout's full inventory
            // diff against tItem.
            if (m_uiOwner != 0 && m_kItems[i].uiDbItemKey != 0)
                CharDBClient::Get().ItemDelete(m_kItems[i].uiDbItemKey);
            m_kItems.erase(m_kItems.begin() + i);
            return true;
        }
    }
    return false;
}
bool Inventory::Move(uint16 a, uint16 b) {
    ShineItem* pa = NULL; ShineItem* pb = NULL;
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
        if (m_kItems[i].uiItemId == id) {
            m_kItems[i].bEquipped = 1;
            // PutOnBelonged: bind-on-equip. Stamps an "@bound:" prefix on
            // the item-index so subsequent sell / trade gates reject it,
            // and persists the option flag (type=BOUND) to the DB so the
            // bind survives a logout.
            const ItemInfoRow* p = ItemTables::Get().FindItem((uint32)m_kItems[i].uiInxName);
            bool wasUnbound = m_kItems[i].kItemIndex.compare(0, 7, "@bound:") != 0;
            if (p && p->bPutOnBelonged && wasUnbound) {
                m_kItems[i].kItemIndex = std::string("@bound:") + m_kItems[i].kItemIndex;
                if (m_uiOwner != 0 && m_kItems[i].uiDbItemKey != 0) {
                    CharDBClient::Get().ItemSetOption(
                        m_kItems[i].uiDbItemKey, /*type=BOUND*/1, 1);
                }
            }
            return true;
        }
    return false;
}
bool Inventory::Unequip(uint32 id) {
    for (size_t i = 0; i < m_kItems.size(); ++i)
        if (m_kItems[i].uiItemId == id) { m_kItems[i].bEquipped = 0; return true; }
    return false;
}

static const ItemInfoRow* RowOf(const ShineItem& k) {
    return ItemTables::Get().FindItem((uint32)k.uiInxName);
}

static bool IsBound(const ShineItem& k, const ItemInfoRow* p) {
    if (!p) return false;
    if (p->bBelonged) return true;
    if (p->bPutOnBelonged && k.kItemIndex.compare(0, 7, "@bound:") == 0) return true;
    return false;
}

static bool ClassAllowed(const ItemInfoRow* p, uint16 uiClass) {
    if (!p || p->uiClassMask == 0) return true;
    if (uiClass == 0) return false;
    uint32 bit = (uint32)(uiClass - 1);
    if (bit >= 25) return false;
    return (p->uiClassMask & (1u << bit)) != 0;
}

bool ItemAuthority::CanEquip(ShinePlayer* pk, const ShineItem& k) {
    if (!pk) return false;
    const ItemInfoRow* p = RowOf(k);
    if (!p) return false;
    if (p->uiEquip == 0)                                       return false;
    if (p->uiDemandLv > 0 && pk->GetLevel() < p->uiDemandLv)   return false;
    if (!ClassAllowed(p, pk->GetClass()))                      return false;
    return true;
}

bool ItemAuthority::CanUse(ShinePlayer* pk, const ShineItem& k) {
    if (!pk) return false;
    const ItemInfoRow* p = RowOf(k);
    if (!p) return false;
    if (p->uiDemandLv > 0 && pk->GetLevel() < p->uiDemandLv)   return false;
    if (p->uiEquip != 0) return CanEquip(pk, k);
    return true;
}

bool ItemAuthority::CanSell(const ShineItem& k) {
    const ItemInfoRow* p = RowOf(k); if (!p) return true;
    if (p->bNoSell)    return false;
    if (IsBound(k, p)) return false;
    return true;
}

// Decodes ItemInfo.Equip into the server-side equip slot. Equip is a
// bitmask telling the doll where the item snaps; lowest set bit is the
// canonical slot, 0 means non-equippable.
uint16 EquipEnumChanger::ToEquipSlot(ItemID uiInxName) {
    const ItemInfoRow* p = ItemTables::Get().FindItem((uint32)uiInxName);
    if (!p || p->uiEquip == 0) return 0;
    uint32 m = p->uiEquip;
    for (uint16 i = 0; i < 32; ++i) { if (m & (1u << i)) return (uint16)(i + 1); }
    return 0;
}

} // namespace shine
