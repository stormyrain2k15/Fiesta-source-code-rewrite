// Server/DataReader/SHN/MiniHouseObjAni.cpp
// Auto-generated: one-file-per-SHN split for MiniHouseObjAni.shn
#include "MiniHouseObjAni.h"
#include "../../Shared/ShineLogSystem.h"

namespace shine {

MiniHouseObjAniShn& MiniHouseObjAniShn::Get() { static MiniHouseObjAniShn s; return s; }

void MiniHouseObjAniShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("MiniHouseObjAni");
    if (!t) { SHINELOG_WARN("MiniHouseObjAni.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        MiniHouseObjAniRow rec;
        rec.uiHandle = (uint16)ShnGetU32(*t, _r, "Handle");
        rec.uiItemID = (uint16)ShnGetU32(*t, _r, "ItemID");
        rec.uiAniGroupIDMaxNum = (uint8)ShnGetU32(*t, _r, "AniGroupIDMaxNum");
        rec.uiAniGroupID = (uint16)ShnGetU32(*t, _r, "AniGroupID");
        rec.uiEventCode = ShnGetU32(*t, _r, "EventCode");
        rec.uiNextAniHandle = (uint16)ShnGetU32(*t, _r, "NextAniHandle");
        rec.uiActorMaxNum = (uint8)ShnGetU32(*t, _r, "ActorMaxNum");
        rec.uiActor01 = ShnGetU32(*t, _r, "Actor01");
        rec.uiActor02 = ShnGetU32(*t, _r, "Actor02");
        rec.uiActor03 = ShnGetU32(*t, _r, "Actor03");
        rec.uiActeeMaxNum = (uint8)ShnGetU32(*t, _r, "ActeeMaxNum");
        rec.uiActee01 = ShnGetU32(*t, _r, "Actee01");
        rec.uiActee02 = ShnGetU32(*t, _r, "Actee02");
        rec.uiActee03 = ShnGetU32(*t, _r, "Actee03");
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("MiniHouseObjAni.shn: %u rows", (uint32)m_kRows.size());
}

} // namespace shine
