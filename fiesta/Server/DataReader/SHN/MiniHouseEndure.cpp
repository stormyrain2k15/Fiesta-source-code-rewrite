// Server/DataReader/SHN/MiniHouseEndure.cpp
// Auto-generated: one-file-per-SHN split for MiniHouseEndure.shn
#include "MiniHouseEndure.h"
#include "../../Shared/ShineLogSystem.h"

namespace fiesta {

MiniHouseEndureShn& MiniHouseEndureShn::Get() { static MiniHouseEndureShn s; return s; }

void MiniHouseEndureShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("MiniHouseEndure");
    if (!t) { SHINELOG_WARN("MiniHouseEndure.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        MiniHouseEndureRow rec;
        rec.uiHandle = (uint16)ShnGetU32(*t, _r, "Handle");
        rec.iEndure = (int16)ShnGetI32(*t, _r, "Endure");
        rec.iUnkCol2 = (int16)ShnGetI32(*t, _r, "UnkCol2");
        rec.iUnkCol3 = (int16)ShnGetI32(*t, _r, "UnkCol3");
        rec.iUnkCol4 = (int16)ShnGetI32(*t, _r, "UnkCol4");
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("MiniHouseEndure.shn: %u rows", (uint32)m_kRows.size());
}

} // namespace fiesta
