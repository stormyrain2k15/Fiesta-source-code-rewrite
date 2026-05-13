// Server/DataReader/SHN/ActionEffectItem.cpp
// Auto-generated: one-file-per-SHN split for ActionEffectItem.shn
#include "ActionEffectItem.h"
#include "../../Shared/ShineLogSystem.h"

namespace shine {

ActionEffectItemShn& ActionEffectItemShn::Get() { static ActionEffectItemShn s; return s; }

void ActionEffectItemShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("ActionEffectItem");
    if (!t) { SHINELOG_WARN("ActionEffectItem.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        ActionEffectItemRow rec;
        rec.uiSHINE_ITEM_ID = (uint16)ShnGetU32(*t, _r, "SHINE_ITEM_ID");
        rec.uiItemActionID = (uint16)ShnGetU32(*t, _r, "ItemActionID");
        rec.uiEnchantMaxLot = (uint8)ShnGetU32(*t, _r, "EnchantMaxLot");
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("ActionEffectItem.shn: %u rows", (uint32)m_kRows.size());
}

} // namespace shine
