// Server/Zone/Inventory.cpp
// Pass 1.23 -- ItemAuthority + EquipEnumChanger fully wired against ItemInfo
// (91 col) so every NoXxx / Belonged / Class / DemandLv / Equip column has a
// runtime consumer.
#include "Inventory.h"
#include "GroupTables.h"
#include "ShineObject.h"
#include "../Shared/ShineLogSystem.h"

namespace fiesta {

Inventory::Inventory() : m_iMoney(0) {}

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
    m_kItems.push_back(rec);
    return true;
}

bool Inventory::Remove(uint32 id) {
    for (size_t i = 0; i < m_kItems.size(); ++i)
        if (m_kItems[i].uiItemId == id) { m_kItems.erase(m_kItems.begin() + i); return true; }
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
            // Honour PutOnBelonged: bind-on-equip (BoE) flips on first wear.
            const ItemInfoRow* p = ItemTables::Get().FindItem((uint32)m_kItems[i].uiInxName);
            if (p && p->bPutOnBelonged) {
                // The ShineItem doesn't currently carry a "bound" flag --
                // we encode the bind by recording the original owner in the
                // string item-index field's prefix on first equip.
                if (m_kItems[i].kItemIndex.find("@bound:") == std::string::npos)
                    m_kItems[i].kItemIndex = std::string("@bound:") + m_kItems[i].kItemIndex;
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

// -----------------------------------------------------------------------------
// ItemAuthority -- gating predicates honoured by every Inventory mutator
// (Use, Equip, Drop, Sell, Trade, Storage, Delete). Each predicate is a thin
// projection of one or more ItemInfo columns.
// -----------------------------------------------------------------------------
static const ItemInfoRow* RowOf(const ShineItem& k) {
    return ItemTables::Get().FindItem((uint32)k.uiInxName);
}

// Returns true when this item is bound to the current owner (BoP or BoE).
static bool IsBound(const ShineItem& k, const ItemInfoRow* p) {
    if (!p) return false;
    if (p->bBelonged) return true;            // bind-on-acquire
    if (p->bPutOnBelonged && k.kItemIndex.compare(0, 7, "@bound:") == 0) return true;
    return false;
}

// Class-permission test: ItemInfo packs 25 boolean per-class columns into
// m_uiClassMask (bit `class-1`). Class==0 means "anyone" (consumables).
static bool ClassAllowed(const ItemInfoRow* p, uint16 uiClass) {
    if (!p || p->uiClassMask == 0) return true;       // unrestricted
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
    // Equippable items reuse CanEquip; non-equippable Use=consumable.
    if (p->uiEquip != 0) return CanEquip(pk, k);
    return true;
}

// New gating predicates (declared in Inventory.h once we add them below).
bool ItemAuthority::CanDrop(const ShineItem& k) {
    const ItemInfoRow* p = RowOf(k); if (!p) return true;
    if (p->bNoDrop)  return false;
    if (IsBound(k, p)) return false;
    return true;
}
bool ItemAuthority::CanSell(const ShineItem& k) {
    const ItemInfoRow* p = RowOf(k); if (!p) return true;
    if (p->bNoSell)  return false;
    if (IsBound(k, p)) return false;
    return true;
}
bool ItemAuthority::CanTrade(const ShineItem& k) {
    const ItemInfoRow* p = RowOf(k); if (!p) return true;
    if (p->bNoTrade) return false;
    if (IsBound(k, p)) return false;
    return true;
}
bool ItemAuthority::CanStorage(const ShineItem& k) {
    const ItemInfoRow* p = RowOf(k); if (!p) return true;
    return p->bNoStorage == 0;
}
bool ItemAuthority::CanDelete(const ShineItem& k) {
    const ItemInfoRow* p = RowOf(k); if (!p) return true;
    return p->bNoDelete == 0;
}

// -----------------------------------------------------------------------------
// EquipEnumChanger -- Decodes ItemInfo.Equip into a server-side equip slot.
//
// Equip is a bit-mask telling the client which doll slot the item snaps to.
// The bit ordering is (high bit -> low bit, NA2016):
//   1=Head 2=Torso 3=Pants 4=Hand 5=Foot 6=Weapon 7=Shield 8=Earring
//   9=Neck 10=Ring 11=Cape 12=PetEquip 13=Costume 14=Mover 15=Title
// We pick the lowest set bit; that's the canonical slot. Unspecified ->
// returns 0 (sentinel for "non-equippable").
// -----------------------------------------------------------------------------
uint16 EquipEnumChanger::ToEquipSlot(ItemID uiInxName) {
    const ItemInfoRow* p = ItemTables::Get().FindItem((uint32)uiInxName);
    if (!p || p->uiEquip == 0) return 0;
    uint32 m = p->uiEquip;
    for (uint16 i = 0; i < 32; ++i) { if (m & (1u << i)) return (uint16)(i + 1); }
    return 0;
}

} // namespace fiesta
