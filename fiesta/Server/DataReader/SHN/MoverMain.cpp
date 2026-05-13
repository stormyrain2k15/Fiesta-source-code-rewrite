// Server/DataReader/SHN/MoverMain.cpp
// Auto-generated: one-file-per-SHN split for MoverMain.shn
#include "MoverMain.h"
#include "../../Shared/ShineLogSystem.h"

namespace shine {

MoverMainShn& MoverMainShn::Get() { static MoverMainShn s; return s; }

void MoverMainShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("MoverMain");
    if (!t) { SHINELOG_WARN("MoverMain.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        MoverMainRow rec;
        rec.uiMoverID = ShnGetU32(*t, _r, "MoverID");
        rec.kMoverIDX = ShnGetStr(*t, _r, "MoverIDX");
        rec.uiCastingTime = ShnGetU32(*t, _r, "CastingTime");
        rec.uiCoolTime = ShnGetU32(*t, _r, "CoolTime");
        rec.uiRunSpeed = (uint16)ShnGetU32(*t, _r, "RunSpeed");
        rec.uiWalkSpeed = (uint16)ShnGetU32(*t, _r, "WalkSpeed");
        rec.uiDurationHour = (uint16)ShnGetU32(*t, _r, "DurationHour");
        rec.uiMaxCharSlot = (uint8)ShnGetU32(*t, _r, "MaxCharSlot");
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("MoverMain.shn: %u rows", (uint32)m_kRows.size());
}

} // namespace shine
