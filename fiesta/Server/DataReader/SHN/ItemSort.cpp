// Server/DataReader/SHN/ItemSort.cpp
// Auto-generated: one-file-per-SHN split for ItemSort.shn
#include "ItemSort.h"
#include "../../Shared/ShineLogSystem.h"

namespace fiesta {

ItemSortShn& ItemSortShn::Get() { static ItemSortShn s; return s; }

void ItemSortShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("ItemSort");
    if (!t) { SHINELOG_WARN("ItemSort.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        ItemSortRow rec;
        rec.kItemSort_Index = ShnGetStr(*t, _r, "ItemSort_Index");
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("ItemSort.shn: %u rows", (uint32)m_kRows.size());
}

} // namespace fiesta
