// Server/DataReader/SHN/SetItem.cpp
// Auto-generated: one-file-per-SHN split for SetItem.shn
#include "SetItem.h"
#include "../../Shared/ShineLogSystem.h"

namespace shine {

SetItemShn& SetItemShn::Get() { static SetItemShn s; return s; }

void SetItemShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("SetItem");
    if (!t) { SHINELOG_WARN("SetItem.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        SetItemRow rec;
        rec.kIndex = ShnGetStr(*t, _r, "Index");
        rec.uiPiece = (uint8)ShnGetU32(*t, _r, "Piece");
        rec.kEffect = ShnGetStr(*t, _r, "Effect");
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("SetItem.shn: %u rows", (uint32)m_kRows.size());
}

} // namespace shine
