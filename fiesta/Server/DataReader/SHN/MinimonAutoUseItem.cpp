// Server/DataReader/SHN/MinimonAutoUseItem.cpp
// Auto-generated: one-file-per-SHN split for MinimonAutoUseItem.shn
#include "MinimonAutoUseItem.h"
#include "../../Shared/ShineLogSystem.h"

namespace fiesta {

MinimonAutoUseItemShn& MinimonAutoUseItemShn::Get() { static MinimonAutoUseItemShn s; return s; }

void MinimonAutoUseItemShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("MinimonAutoUseItem");
    if (!t) { SHINELOG_WARN("MinimonAutoUseItem.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        MinimonAutoUseItemRow rec;
        rec.uiMAUItem = ShnGetU32(*t, _r, "MAUItem");
        rec.kItemIDX = ShnGetStr(*t, _r, "ItemIDX");
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("MinimonAutoUseItem.shn: %u rows", (uint32)m_kRows.size());
}

} // namespace fiesta
