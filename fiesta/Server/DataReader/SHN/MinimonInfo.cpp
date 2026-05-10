// Server/DataReader/SHN/MinimonInfo.cpp
// Auto-generated: one-file-per-SHN split for MinimonInfo.shn
#include "MinimonInfo.h"
#include "../../Shared/ShineLogSystem.h"

namespace fiesta {

MinimonInfoShn& MinimonInfoShn::Get() { static MinimonInfoShn s; return s; }

void MinimonInfoShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("MinimonInfo");
    if (!t) { SHINELOG_WARN("MinimonInfo.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        MinimonInfoRow rec;
        rec.kItemIDX = ShnGetStr(*t, _r, "ItemIDX");
        rec.uiMinimonEquipPos = ShnGetU32(*t, _r, "MinimonEquipPos");
        rec.uiMinimonRole = ShnGetU32(*t, _r, "MinimonRole");
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("MinimonInfo.shn: %u rows", (uint32)m_kRows.size());
}

} // namespace fiesta
