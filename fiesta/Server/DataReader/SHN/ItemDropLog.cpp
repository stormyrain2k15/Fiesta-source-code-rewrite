// Server/DataReader/SHN/ItemDropLog.cpp
// Auto-generated: one-file-per-SHN split for ItemDropLog.shn
#include "ItemDropLog.h"
#include "../../Shared/ShineLogSystem.h"

namespace fiesta {

ItemDropLogShn& ItemDropLogShn::Get() { static ItemDropLogShn s; return s; }

void ItemDropLogShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("ItemDropLog");
    if (!t) { SHINELOG_WARN("ItemDropLog.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        ItemDropLogRow rec;
        rec.uiItemID = ShnGetU32(*t, _r, "ItemID");
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("ItemDropLog.shn: %u rows", (uint32)m_kRows.size());
}

} // namespace fiesta
