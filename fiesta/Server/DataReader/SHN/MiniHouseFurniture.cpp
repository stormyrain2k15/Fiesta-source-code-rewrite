// Server/DataReader/SHN/MiniHouseFurniture.cpp
// Auto-generated: one-file-per-SHN split for MiniHouseFurniture.shn
#include "MiniHouseFurniture.h"
#include "../../Shared/ShineLogSystem.h"

namespace shine {

MiniHouseFurnitureShn& MiniHouseFurnitureShn::Get() { static MiniHouseFurnitureShn s; return s; }

void MiniHouseFurnitureShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("MiniHouseFurniture");
    if (!t) { SHINELOG_WARN("MiniHouseFurniture.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        MiniHouseFurnitureRow rec;
        rec.uiHandle = (uint16)ShnGetU32(*t, _r, "Handle");
        rec.kItemID = ShnGetStr(*t, _r, "ItemID");
        rec.kFurnitureType = ShnGetStr(*t, _r, "FurnitureType");
        rec.kInvenType = ShnGetStr(*t, _r, "InvenType");
        rec.uiGameType = ShnGetU32(*t, _r, "GameType");
        rec.uiCanSet = (uint8)ShnGetU32(*t, _r, "CanSet");
        rec.kBackimage = ShnGetStr(*t, _r, "Backimage");
        rec.uiWALL = (uint8)ShnGetU32(*t, _r, "WALL");
        rec.uiBOTTOM = (uint8)ShnGetU32(*t, _r, "BOTTOM");
        rec.uiCEILING = (uint8)ShnGetU32(*t, _r, "CEILING");
        rec.uiIsAnimation = (uint8)ShnGetU32(*t, _r, "IsAnimation");
        rec.uiWeight = (uint16)ShnGetU32(*t, _r, "Weight");
        rec.uiKeepTime_Hour = (uint16)ShnGetU32(*t, _r, "KeepTime_Hour");
        rec.uiKeepTime_Endure = (uint16)ShnGetU32(*t, _r, "KeepTime_Endure");
        rec.uiGrip = (uint8)ShnGetU32(*t, _r, "Grip");
        rec.uiMaxSlot = (uint8)ShnGetU32(*t, _r, "MaxSlot");
        rec.uiMHEmotionID = (uint8)ShnGetU32(*t, _r, "MHEmotionID");
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("MiniHouseFurniture.shn: %u rows", (uint32)m_kRows.size());
}

} // namespace shine
