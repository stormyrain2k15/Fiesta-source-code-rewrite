// Server/DataReader/SHN/ItemOptions.cpp
// Auto-generated: one-file-per-SHN split for ItemOptions.shn
#include "ItemOptions.h"
#include "../../Shared/ShineLogSystem.h"

namespace shine {

ItemOptionsShn& ItemOptionsShn::Get() { static ItemOptionsShn s; return s; }

void ItemOptionsShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("ItemOptions");
    if (!t) { SHINELOG_WARN("ItemOptions.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        ItemOptionsRow rec;
        rec.uiOptionDegree = (uint16)ShnGetU32(*t, _r, "OptionDegree");
        rec.uiType = ShnGetU32(*t, _r, "Type");
        rec.uiInterval = (uint8)ShnGetU32(*t, _r, "Interval");
        rec.iRate = (int16)ShnGetI32(*t, _r, "Rate");
        rec.iUnkCol4 = (int16)ShnGetI32(*t, _r, "UnkCol4");
        rec.iUnkCol5 = (int16)ShnGetI32(*t, _r, "UnkCol5");
        rec.iUnkCol6 = (int16)ShnGetI32(*t, _r, "UnkCol6");
        rec.iUnkCol7 = (int16)ShnGetI32(*t, _r, "UnkCol7");
        rec.iUnkCol8 = (int16)ShnGetI32(*t, _r, "UnkCol8");
        rec.iUnkCol9 = (int16)ShnGetI32(*t, _r, "UnkCol9");
        rec.iUnkCol10 = (int16)ShnGetI32(*t, _r, "UnkCol10");
        rec.iUnkCol11 = (int16)ShnGetI32(*t, _r, "UnkCol11");
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("ItemOptions.shn: %u rows", (uint32)m_kRows.size());
}

} // namespace shine
