// Server/DataReader/SHN/MiniHouseFurnitureObjEffect.cpp
// Auto-generated: one-file-per-SHN split for MiniHouseFurnitureObjEffect.shn
#include "MiniHouseFurnitureObjEffect.h"
#include "../../Shared/ShineLogSystem.h"

namespace shine {

MiniHouseFurnitureObjEffectShn& MiniHouseFurnitureObjEffectShn::Get() { static MiniHouseFurnitureObjEffectShn s; return s; }

void MiniHouseFurnitureObjEffectShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("MiniHouseFurnitureObjEffect");
    if (!t) { SHINELOG_WARN("MiniHouseFurnitureObjEffect.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        MiniHouseFurnitureObjEffectRow rec;
        rec.uiHandle = (uint16)ShnGetU32(*t, _r, "Handle");
        rec.kItemID = ShnGetStr(*t, _r, "ItemID");
        rec.uiEffectEnum = ShnGetU32(*t, _r, "EffectEnum");
        rec.kEffectIndex = ShnGetStr(*t, _r, "EffectIndex");
        rec.iApplyRange = ShnGetI32(*t, _r, "ApplyRange");
        rec.iUnkCol5 = ShnGetI32(*t, _r, "UnkCol5");
        rec.iUnkCol6 = ShnGetI32(*t, _r, "UnkCol6");
        rec.iUnkCol7 = ShnGetI32(*t, _r, "UnkCol7");
        rec.iUnkCol8 = ShnGetI32(*t, _r, "UnkCol8");
        rec.iUseRange = ShnGetI32(*t, _r, "UseRange");
        rec.iUnkCol10 = ShnGetI32(*t, _r, "UnkCol10");
        rec.iUnkCol11 = ShnGetI32(*t, _r, "UnkCol11");
        rec.iUnkCol12 = ShnGetI32(*t, _r, "UnkCol12");
        rec.iUnkCol13 = ShnGetI32(*t, _r, "UnkCol13");
        rec.kNeedItem = ShnGetStr(*t, _r, "NeedItem");
        rec.uiNeedMoney = ShnGetU32(*t, _r, "NeedMoney");
        rec.kEffectName = ShnGetStr(*t, _r, "EffectName");
        rec.kEffectSound = ShnGetStr(*t, _r, "EffectSound");
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("MiniHouseFurnitureObjEffect.shn: %u rows", (uint32)m_kRows.size());
}

} // namespace shine
