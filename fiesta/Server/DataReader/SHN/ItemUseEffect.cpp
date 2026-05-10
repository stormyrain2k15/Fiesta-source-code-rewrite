// Server/DataReader/SHN/ItemUseEffect.cpp
// Auto-generated: one-file-per-SHN split for ItemUseEffect.shn
#include "ItemUseEffect.h"
#include "../../Shared/ShineLogSystem.h"

namespace fiesta {

ItemUseEffectShn& ItemUseEffectShn::Get() { static ItemUseEffectShn s; return s; }

void ItemUseEffectShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("ItemUseEffect");
    if (!t) { SHINELOG_WARN("ItemUseEffect.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        ItemUseEffectRow rec;
        rec.kItemIndex = ShnGetStr(*t, _r, "ItemIndex");
        rec.uiUseEffectA = ShnGetU32(*t, _r, "UseEffectA");
        rec.uiUseValueA = (uint16)ShnGetU32(*t, _r, "UseValueA");
        rec.uiUseEffectB = ShnGetU32(*t, _r, "UseEffectB");
        rec.uiUseValueB = (uint16)ShnGetU32(*t, _r, "UseValueB");
        rec.uiUseEffectC = ShnGetU32(*t, _r, "UseEffectC");
        rec.uiUseValueC = (uint16)ShnGetU32(*t, _r, "UseValueC");
        rec.kUseAbstateName = ShnGetStr(*t, _r, "UseAbstateName");
        rec.uiUseAbstateIndex = ShnGetU32(*t, _r, "UseAbstateIndex");
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("ItemUseEffect.shn: %u rows", (uint32)m_kRows.size());
}

} // namespace fiesta
