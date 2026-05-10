// Server/DataReader/SHN/MiniHouse.cpp
// Auto-generated: one-file-per-SHN split for MiniHouse.shn
#include "MiniHouse.h"
#include "../../Shared/ShineLogSystem.h"

namespace fiesta {

MiniHouseShn& MiniHouseShn::Get() { static MiniHouseShn s; return s; }

void MiniHouseShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("MiniHouse");
    if (!t) { SHINELOG_WARN("MiniHouse.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        MiniHouseRow rec;
        rec.uiHandle = (uint16)ShnGetU32(*t, _r, "Handle");
        rec.kItemID = ShnGetStr(*t, _r, "ItemID");
        rec.kDummyType = ShnGetStr(*t, _r, "DummyType");
        rec.kBackimage = ShnGetStr(*t, _r, "Backimage");
        rec.uiKeepTime_Hour = (uint16)ShnGetU32(*t, _r, "KeepTime_Hour");
        rec.uiHPTick = (uint16)ShnGetU32(*t, _r, "HPTick");
        rec.uiSPTick = (uint16)ShnGetU32(*t, _r, "SPTick");
        rec.uiHPRecovery = (uint16)ShnGetU32(*t, _r, "HPRecovery");
        rec.uiSPRecovery = (uint16)ShnGetU32(*t, _r, "SPRecovery");
        rec.uiCasting = (uint16)ShnGetU32(*t, _r, "Casting");
        rec.uiSlot = (uint8)ShnGetU32(*t, _r, "Slot");
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("MiniHouse.shn: %u rows", (uint32)m_kRows.size());
}

} // namespace fiesta
