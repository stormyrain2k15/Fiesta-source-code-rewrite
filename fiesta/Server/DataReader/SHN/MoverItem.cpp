// Server/DataReader/SHN/MoverItem.cpp
// Auto-generated: one-file-per-SHN split for MoverItem.shn
#include "MoverItem.h"
#include "../../Shared/ShineLogSystem.h"

namespace fiesta {

MoverItemShn& MoverItemShn::Get() { static MoverItemShn s; return s; }

void MoverItemShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("MoverItem");
    if (!t) { SHINELOG_WARN("MoverItem.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        MoverItemRow rec;
        rec.kMoverIDX = ShnGetStr(*t, _r, "MoverIDX");
        rec.kItemIDX = ShnGetStr(*t, _r, "ItemIDX");
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("MoverItem.shn: %u rows", (uint32)m_kRows.size());
}

} // namespace fiesta
