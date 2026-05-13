// Server/DataReader/SHN/BMP.cpp
// Auto-generated: one-file-per-SHN split for BMP.shn
#include "BMP.h"
#include "../../Shared/ShineLogSystem.h"

namespace shine {

BMPShn& BMPShn::Get() { static BMPShn s; return s; }

void BMPShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("BMP");
    if (!t) { SHINELOG_WARN("BMP.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        BMPRow rec;
        rec.kBMPIndex = ShnGetStr(*t, _r, "BMPIndex");
        rec.kFileName = ShnGetStr(*t, _r, "FileName");
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("BMP.shn: %u rows", (uint32)m_kRows.size());
}

} // namespace shine
