// Server/DataReader/SHN/ItemDismantle.cpp
// Auto-generated: one-file-per-SHN split for ItemDismantle.shn
#include "ItemDismantle.h"
#include "../../Shared/ShineLogSystem.h"

namespace fiesta {

ItemDismantleShn& ItemDismantleShn::Get() { static ItemDismantleShn s; return s; }

void ItemDismantleShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("ItemDismantle");
    if (!t) { SHINELOG_WARN("ItemDismantle.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        ItemDismantleRow rec;
        rec.uiID = (uint8)ShnGetU32(*t, _r, "ID");
        rec.uiGrade = (uint8)ShnGetU32(*t, _r, "Grade");
        rec.iArmor = ShnGetI32(*t, _r, "Armor");
        rec.iUnkCol3 = ShnGetI32(*t, _r, "UnkCol3");
        rec.iUnkCol4 = ShnGetI32(*t, _r, "UnkCol4");
        rec.iUnkCol5 = ShnGetI32(*t, _r, "UnkCol5");
        rec.iUnkCol6 = ShnGetI32(*t, _r, "UnkCol6");
        rec.iBoot = ShnGetI32(*t, _r, "Boot");
        rec.iUnkCol8 = ShnGetI32(*t, _r, "UnkCol8");
        rec.iUnkCol9 = ShnGetI32(*t, _r, "UnkCol9");
        rec.iUnkCol10 = ShnGetI32(*t, _r, "UnkCol10");
        rec.iUnkCol11 = ShnGetI32(*t, _r, "UnkCol11");
        rec.iShield = ShnGetI32(*t, _r, "Shield");
        rec.iUnkCol13 = ShnGetI32(*t, _r, "UnkCol13");
        rec.iUnkCol14 = ShnGetI32(*t, _r, "UnkCol14");
        rec.iUnkCol15 = ShnGetI32(*t, _r, "UnkCol15");
        rec.iUnkCol16 = ShnGetI32(*t, _r, "UnkCol16");
        rec.iWeapon = ShnGetI32(*t, _r, "Weapon");
        rec.iUnkCol18 = ShnGetI32(*t, _r, "UnkCol18");
        rec.iUnkCol19 = ShnGetI32(*t, _r, "UnkCol19");
        rec.iUnkCol20 = ShnGetI32(*t, _r, "UnkCol20");
        rec.iUnkCol21 = ShnGetI32(*t, _r, "UnkCol21");
        rec.iAmulet = ShnGetI32(*t, _r, "Amulet");
        rec.iUnkCol23 = ShnGetI32(*t, _r, "UnkCol23");
        rec.iUnkCol24 = ShnGetI32(*t, _r, "UnkCol24");
        rec.iUnkCol25 = ShnGetI32(*t, _r, "UnkCol25");
        rec.iUnkCol26 = ShnGetI32(*t, _r, "UnkCol26");
        m_kById[rec.uiID] = m_kRows.size();
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("ItemDismantle.shn: %u rows", (uint32)m_kRows.size());
}

const ItemDismantleRow* ItemDismantleShn::FindById(uint32 uiID) const {
    std::map<uint32, size_t>::const_iterator it = m_kById.find(uiID);
    return (it == m_kById.end()) ? NULL : &m_kRows[it->second];
}

} // namespace fiesta
