// Server/DataReader/SHN/ColorInfo.cpp
// Auto-generated: one-file-per-SHN split for ColorInfo.shn
#include "ColorInfo.h"
#include "../../Shared/ShineLogSystem.h"

namespace shine {

ColorInfoShn& ColorInfoShn::Get() { static ColorInfoShn s; return s; }

void ColorInfoShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("ColorInfo");
    if (!t) { SHINELOG_WARN("ColorInfo.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        ColorInfoRow rec;
        rec.uiEColorID = ShnGetU32(*t, _r, "eColorID");
        rec.uiColorR = (uint8)ShnGetU32(*t, _r, "ColorR");
        rec.uiColorG = (uint8)ShnGetU32(*t, _r, "ColorG");
        rec.uiColorB = (uint8)ShnGetU32(*t, _r, "ColorB");
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("ColorInfo.shn: %u rows", (uint32)m_kRows.size());
}

} // namespace shine
