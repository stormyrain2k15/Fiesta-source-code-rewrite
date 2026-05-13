// Server/DataReader/SHN/KingdomQuestMap.cpp
// Auto-generated: one-file-per-SHN split for KingdomQuestMap.shn
#include "KingdomQuestMap.h"
#include "../../Shared/ShineLogSystem.h"

namespace shine {

KingdomQuestMapShn& KingdomQuestMapShn::Get() { static KingdomQuestMapShn s; return s; }

void KingdomQuestMapShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("KingdomQuestMap");
    if (!t) { SHINELOG_WARN("KingdomQuestMap.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        KingdomQuestMapRow rec;
        rec.uiNumOfMap = (uint8)ShnGetU32(*t, _r, "NumOfMap");
        rec.kBaseMap = ShnGetStr(*t, _r, "BaseMap");
        rec.kMap = ShnGetStr(*t, _r, "Map");
        rec.kUnkCol3 = ShnGetStr(*t, _r, "UnkCol3");
        rec.kUnkCol4 = ShnGetStr(*t, _r, "UnkCol4");
        rec.kUnkCol5 = ShnGetStr(*t, _r, "UnkCol5");
        rec.kUnkCol6 = ShnGetStr(*t, _r, "UnkCol6");
        rec.kUnkCol7 = ShnGetStr(*t, _r, "UnkCol7");
        rec.kUnkCol8 = ShnGetStr(*t, _r, "UnkCol8");
        rec.kUnkCol9 = ShnGetStr(*t, _r, "UnkCol9");
        rec.kUnkCol10 = ShnGetStr(*t, _r, "UnkCol10");
        rec.kUnkCol11 = ShnGetStr(*t, _r, "UnkCol11");
        rec.iClear = (int8)ShnGetI32(*t, _r, "Clear");
        rec.iUnkCol13 = (int8)ShnGetI32(*t, _r, "UnkCol13");
        rec.iUnkCol14 = (int8)ShnGetI32(*t, _r, "UnkCol14");
        rec.iUnkCol15 = (int8)ShnGetI32(*t, _r, "UnkCol15");
        rec.iUnkCol16 = (int8)ShnGetI32(*t, _r, "UnkCol16");
        rec.iUnkCol17 = (int8)ShnGetI32(*t, _r, "UnkCol17");
        rec.iUnkCol18 = (int8)ShnGetI32(*t, _r, "UnkCol18");
        rec.iUnkCol19 = (int8)ShnGetI32(*t, _r, "UnkCol19");
        rec.iUnkCol20 = (int8)ShnGetI32(*t, _r, "UnkCol20");
        rec.iUnkCol21 = (int8)ShnGetI32(*t, _r, "UnkCol21");
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("KingdomQuestMap.shn: %u rows", (uint32)m_kRows.size());
}

} // namespace shine
