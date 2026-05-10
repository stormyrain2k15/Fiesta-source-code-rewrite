// Server/DataReader/SHN/ChargedDeletableBuff.cpp
// Auto-generated: one-file-per-SHN split for ChargedDeletableBuff.shn
#include "ChargedDeletableBuff.h"
#include "../../Shared/ShineLogSystem.h"

namespace fiesta {

ChargedDeletableBuffShn& ChargedDeletableBuffShn::Get() { static ChargedDeletableBuffShn s; return s; }

void ChargedDeletableBuffShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("ChargedDeletableBuff");
    if (!t) { SHINELOG_WARN("ChargedDeletableBuff.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        ChargedDeletableBuffRow rec;
        rec.kCDI_IDX = ShnGetStr(*t, _r, "CDI_IDX");
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("ChargedDeletableBuff.shn: %u rows", (uint32)m_kRows.size());
}

} // namespace fiesta
