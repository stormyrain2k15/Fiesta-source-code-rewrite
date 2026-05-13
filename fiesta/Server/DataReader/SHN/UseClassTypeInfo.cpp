// Server/DataReader/SHN/UseClassTypeInfo.cpp
// Auto-generated: one-file-per-SHN split for UseClassTypeInfo.shn
#include "UseClassTypeInfo.h"
#include "../../Shared/ShineLogSystem.h"

namespace shine {

UseClassTypeInfoShn& UseClassTypeInfoShn::Get() { static UseClassTypeInfoShn s; return s; }

void UseClassTypeInfoShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("UseClassTypeInfo");
    if (!t) { SHINELOG_WARN("UseClassTypeInfo.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        UseClassTypeInfoRow rec;
        rec.uiUseClass = ShnGetU32(*t, _r, "UseClass");
        rec.uiFig = (uint8)ShnGetU32(*t, _r, "Fig");
        rec.uiCfig = (uint8)ShnGetU32(*t, _r, "Cfig");
        rec.uiWar = (uint8)ShnGetU32(*t, _r, "War");
        rec.uiGla = (uint8)ShnGetU32(*t, _r, "Gla");
        rec.uiKni = (uint8)ShnGetU32(*t, _r, "Kni");
        rec.uiCle = (uint8)ShnGetU32(*t, _r, "Cle");
        rec.uiHcle = (uint8)ShnGetU32(*t, _r, "Hcle");
        rec.uiPal = (uint8)ShnGetU32(*t, _r, "Pal");
        rec.uiHol = (uint8)ShnGetU32(*t, _r, "Hol");
        rec.uiGua = (uint8)ShnGetU32(*t, _r, "Gua");
        rec.uiArc = (uint8)ShnGetU32(*t, _r, "Arc");
        rec.uiHarc = (uint8)ShnGetU32(*t, _r, "Harc");
        rec.uiSco = (uint8)ShnGetU32(*t, _r, "Sco");
        rec.uiSha = (uint8)ShnGetU32(*t, _r, "Sha");
        rec.uiRan = (uint8)ShnGetU32(*t, _r, "Ran");
        rec.uiMag = (uint8)ShnGetU32(*t, _r, "Mag");
        rec.uiWmag = (uint8)ShnGetU32(*t, _r, "Wmag");
        rec.uiEnc = (uint8)ShnGetU32(*t, _r, "Enc");
        rec.uiWarl = (uint8)ShnGetU32(*t, _r, "Warl");
        rec.uiWiz = (uint8)ShnGetU32(*t, _r, "Wiz");
        rec.uiJok = (uint8)ShnGetU32(*t, _r, "Jok");
        rec.uiChs = (uint8)ShnGetU32(*t, _r, "Chs");
        rec.uiCru = (uint8)ShnGetU32(*t, _r, "Cru");
        rec.uiCls = (uint8)ShnGetU32(*t, _r, "Cls");
        rec.uiAss = (uint8)ShnGetU32(*t, _r, "Ass");
        rec.uiSen = (uint8)ShnGetU32(*t, _r, "Sen");
        rec.uiSav = (uint8)ShnGetU32(*t, _r, "Sav");
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("UseClassTypeInfo.shn: %u rows", (uint32)m_kRows.size());
}

} // namespace shine
