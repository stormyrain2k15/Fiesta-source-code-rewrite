// Server/DataReader/SHN/HairInfo.cpp
// Auto-generated: one-file-per-SHN split for HairInfo.shn
#include "HairInfo.h"
#include "../../Shared/ShineLogSystem.h"

namespace fiesta {

HairInfoShn& HairInfoShn::Get() { static HairInfoShn s; return s; }

void HairInfoShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("HairInfo");
    if (!t) { SHINELOG_WARN("HairInfo.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        HairInfoRow rec;
        rec.uiID = (uint8)ShnGetU32(*t, _r, "ID");
        rec.kIndexName = ShnGetStr(*t, _r, "IndexName");
        rec.kHairName = ShnGetStr(*t, _r, "HairName");
        rec.uiGrade = (uint8)ShnGetU32(*t, _r, "Grade");
        rec.uiFighter = ShnGetU32(*t, _r, "fighter");
        rec.uiArcher = ShnGetU32(*t, _r, "archer");
        rec.uiCleric = ShnGetU32(*t, _r, "cleric");
        rec.uiMage = ShnGetU32(*t, _r, "mage");
        rec.uiJoker = ShnGetU32(*t, _r, "Joker");
        rec.uiSentinel = ShnGetU32(*t, _r, "Sentinel");
        rec.uiUcIsLink_Front = (uint8)ShnGetU32(*t, _r, "ucIsLink_Front");
        rec.kAcModelName_Front = ShnGetStr(*t, _r, "acModelName_Front");
        rec.kFrontTex = ShnGetStr(*t, _r, "FrontTex");
        rec.uiUcIsLink_Bottom = (uint8)ShnGetU32(*t, _r, "ucIsLink_Bottom");
        rec.kAcModelName_Bottom = ShnGetStr(*t, _r, "acModelName_Bottom");
        rec.kBottomTex = ShnGetStr(*t, _r, "BottomTex");
        rec.uiUcIsLink_Top = (uint8)ShnGetU32(*t, _r, "ucIsLink_Top");
        rec.kAcModelName_Top = ShnGetStr(*t, _r, "acModelName_Top");
        rec.kTopTex = ShnGetStr(*t, _r, "TopTex");
        rec.uiException1 = ShnGetU32(*t, _r, "Exception1");
        rec.uiException2 = ShnGetU32(*t, _r, "Exception2");
        rec.uiUcIsLink_Acc = (uint8)ShnGetU32(*t, _r, "ucIsLink_Acc");
        rec.kAcModelName_Acc = ShnGetStr(*t, _r, "acModelName_Acc");
        rec.kAcc1Tex = ShnGetStr(*t, _r, "Acc1Tex");
        rec.uiUcIsLink_Acc2 = (uint8)ShnGetU32(*t, _r, "ucIsLink_Acc2");
        rec.kAcModelName_Acc2 = ShnGetStr(*t, _r, "acModelName_Acc2");
        rec.kAcc2Tex = ShnGetStr(*t, _r, "Acc2Tex");
        rec.uiUcIsLink_Acc3 = (uint8)ShnGetU32(*t, _r, "ucIsLink_Acc3");
        rec.kAcModelName_Acc3 = ShnGetStr(*t, _r, "acModelName_Acc3");
        rec.kAcc3Tex = ShnGetStr(*t, _r, "Acc3Tex");
        m_kById[rec.uiID] = m_kRows.size();
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("HairInfo.shn: %u rows", (uint32)m_kRows.size());
}

const HairInfoRow* HairInfoShn::FindById(uint32 uiID) const {
    std::map<uint32, size_t>::const_iterator it = m_kById.find(uiID);
    return (it == m_kById.end()) ? NULL : &m_kRows[it->second];
}

} // namespace fiesta
