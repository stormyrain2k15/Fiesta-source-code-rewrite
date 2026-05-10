// Server/DataReader/SHN/GBDiceDividind.cpp
// Auto-generated: one-file-per-SHN split for GBDiceDividind.shn
#include "GBDiceDividind.h"
#include "../../Shared/ShineLogSystem.h"

namespace fiesta {

GBDiceDividindShn& GBDiceDividindShn::Get() { static GBDiceDividindShn s; return s; }

void GBDiceDividindShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("GBDiceDividind");
    if (!t) { SHINELOG_WARN("GBDiceDividind.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        GBDiceDividindRow rec;
        rec.iDividendRate = (int16)ShnGetI32(*t, _r, "DividendRate");
        rec.iUnkCol1 = (int16)ShnGetI32(*t, _r, "UnkCol1");
        rec.iUnkCol2 = (int16)ShnGetI32(*t, _r, "UnkCol2");
        rec.iUnkCol3 = (int16)ShnGetI32(*t, _r, "UnkCol3");
        rec.iUnkCol4 = (int16)ShnGetI32(*t, _r, "UnkCol4");
        rec.iUnkCol5 = (int16)ShnGetI32(*t, _r, "UnkCol5");
        rec.iUnkCol6 = (int16)ShnGetI32(*t, _r, "UnkCol6");
        rec.iUnkCol7 = (int16)ShnGetI32(*t, _r, "UnkCol7");
        rec.iUnkCol8 = (int16)ShnGetI32(*t, _r, "UnkCol8");
        rec.iUnkCol9 = (int16)ShnGetI32(*t, _r, "UnkCol9");
        rec.iUnkCol10 = (int16)ShnGetI32(*t, _r, "UnkCol10");
        rec.iUnkCol11 = (int16)ShnGetI32(*t, _r, "UnkCol11");
        rec.iUnkCol12 = (int16)ShnGetI32(*t, _r, "UnkCol12");
        rec.iUnkCol13 = (int16)ShnGetI32(*t, _r, "UnkCol13");
        rec.iUnkCol14 = (int16)ShnGetI32(*t, _r, "UnkCol14");
        rec.uiAnyTriple = (uint8)ShnGetU32(*t, _r, "AnyTriple");
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("GBDiceDividind.shn: %u rows", (uint32)m_kRows.size());
}

} // namespace fiesta
