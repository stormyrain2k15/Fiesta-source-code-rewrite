// Server/DataReader/SHN/ItemAction.cpp
// Auto-generated: one-file-per-SHN split for ItemAction.shn
#include "ItemAction.h"
#include "../../Shared/ShineLogSystem.h"

namespace fiesta {

ItemActionShn& ItemActionShn::Get() { static ItemActionShn s; return s; }

void ItemActionShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("ItemAction");
    if (!t) { SHINELOG_WARN("ItemAction.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        ItemActionRow rec;
        rec.uiItemActionID = (uint16)ShnGetU32(*t, _r, "ItemActionID");
        rec.uiConditionID = (uint16)ShnGetU32(*t, _r, "ConditionID");
        rec.uiEffectID = (uint16)ShnGetU32(*t, _r, "EffectID");
        rec.uiCoolTime = ShnGetU32(*t, _r, "CoolTime");
        rec.uiCoolGroupActionID = (uint16)ShnGetU32(*t, _r, "CoolGroupActionID");
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("ItemAction.shn: %u rows", (uint32)m_kRows.size());
}

} // namespace fiesta
