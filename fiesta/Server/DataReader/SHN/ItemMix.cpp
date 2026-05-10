// Server/DataReader/SHN/ItemMix.cpp
// Auto-generated: one-file-per-SHN split for ItemMix.shn
#include "ItemMix.h"
#include "../../Shared/ShineLogSystem.h"

namespace fiesta {

ItemMixShn& ItemMixShn::Get() { static ItemMixShn s; return s; }

void ItemMixShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("ItemMix");
    if (!t) { SHINELOG_WARN("ItemMix.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        ItemMixRow rec;
        rec.kRaw_ItemIDX = ShnGetStr(*t, _r, "Raw_ItemIDX");
        rec.kMix_ItemIDX = ShnGetStr(*t, _r, "Mix_ItemIDX");
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("ItemMix.shn: %u rows", (uint32)m_kRows.size());
}

} // namespace fiesta
