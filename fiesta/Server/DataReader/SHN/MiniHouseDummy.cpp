// Server/DataReader/SHN/MiniHouseDummy.cpp
// Auto-generated: one-file-per-SHN split for MiniHouseDummy.shn
#include "MiniHouseDummy.h"
#include "../../Shared/ShineLogSystem.h"

namespace fiesta {

MiniHouseDummyShn& MiniHouseDummyShn::Get() { static MiniHouseDummyShn s; return s; }

void MiniHouseDummyShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("MiniHouseDummy");
    if (!t) { SHINELOG_WARN("MiniHouseDummy.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        MiniHouseDummyRow rec;
        rec.uiNo = (uint16)ShnGetU32(*t, _r, "No");
        rec.kIndex = ShnGetStr(*t, _r, "Index");
        rec.kIconFileName = ShnGetStr(*t, _r, "IconFileName");
        rec.uiNIconNum = (uint16)ShnGetU32(*t, _r, "nIconNum");
        rec.uiHPTick = (uint16)ShnGetU32(*t, _r, "HPTick");
        rec.uiSPTick = (uint16)ShnGetU32(*t, _r, "SPTick");
        rec.uiHPRecovery = (uint16)ShnGetU32(*t, _r, "HPRecovery");
        rec.uiSPRecovery = (uint16)ShnGetU32(*t, _r, "SPRecovery");
        rec.uiCasting = (uint16)ShnGetU32(*t, _r, "Casting");
        rec.uiSlot = (uint8)ShnGetU32(*t, _r, "Slot");
        rec.kHouseAType = ShnGetStr(*t, _r, "HouseAType");
        rec.uiHouseALoc = ShnGetU32(*t, _r, "HouseALoc");
        rec.kHouseBType = ShnGetStr(*t, _r, "HouseBType");
        rec.uiHouseBLoc = ShnGetU32(*t, _r, "HouseBLoc");
        rec.kHouseCType = ShnGetStr(*t, _r, "HouseCType");
        rec.uiHouseCLoc = ShnGetU32(*t, _r, "HouseCLoc");
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("MiniHouseDummy.shn: %u rows", (uint32)m_kRows.size());
}

} // namespace fiesta
