// Server/DataReader/SHN/MoverAbility.cpp
// Auto-generated: one-file-per-SHN split for MoverAbility.shn
#include "MoverAbility.h"
#include "../../Shared/ShineLogSystem.h"

namespace fiesta {

MoverAbilityShn& MoverAbilityShn::Get() { static MoverAbilityShn s; return s; }

void MoverAbilityShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("MoverAbility");
    if (!t) { SHINELOG_WARN("MoverAbility.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        MoverAbilityRow rec;
        rec.kMoverIDX = ShnGetStr(*t, _r, "MoverIDX");
        rec.uiMoverLv = (uint8)ShnGetU32(*t, _r, "MoverLv");
        rec.uiHP = ShnGetU32(*t, _r, "HP");
        rec.uiWCMin = ShnGetU32(*t, _r, "WCMin");
        rec.uiWCMax = ShnGetU32(*t, _r, "WCMax");
        rec.uiMAMin = ShnGetU32(*t, _r, "MAMin");
        rec.uiMAMax = ShnGetU32(*t, _r, "MAMax");
        rec.uiAC = (uint16)ShnGetU32(*t, _r, "AC");
        rec.uiMR = (uint16)ShnGetU32(*t, _r, "MR");
        rec.uiTH = (uint16)ShnGetU32(*t, _r, "TH");
        rec.uiTB = (uint16)ShnGetU32(*t, _r, "TB");
        rec.kResIndex = ShnGetStr(*t, _r, "ResIndex");
        rec.uiAbsoluteSize = ShnGetU32(*t, _r, "AbsoluteSize");
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("MoverAbility.shn: %u rows", (uint32)m_kRows.size());
}

} // namespace fiesta
