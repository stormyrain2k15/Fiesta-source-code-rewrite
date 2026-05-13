// Server/DataReader/SHN/NPCViewInfo.cpp
// Auto-generated: one-file-per-SHN split for NPCViewInfo.shn
#include "NPCViewInfo.h"
#include "../../Shared/ShineLogSystem.h"

namespace shine {

NPCViewInfoShn& NPCViewInfoShn::Get() { static NPCViewInfoShn s; return s; }

void NPCViewInfoShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("NPCViewInfo");
    if (!t) { SHINELOG_WARN("NPCViewInfo.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        NPCViewInfoRow rec;
        rec.uiTypeIndex = (uint16)ShnGetU32(*t, _r, "TypeIndex");
        rec.uiClass = ShnGetU32(*t, _r, "Class");
        rec.uiGender = (uint16)ShnGetU32(*t, _r, "Gender");
        rec.uiFaceShape = (uint8)ShnGetU32(*t, _r, "FaceShape");
        rec.uiHairType = (uint8)ShnGetU32(*t, _r, "HairType");
        rec.uiHairColor = (uint8)ShnGetU32(*t, _r, "HairColor");
        rec.uiBaseActionCode = ShnGetU32(*t, _r, "BaseActionCode");
        rec.uiPeriodActionCode = ShnGetU32(*t, _r, "PeriodActionCode");
        rec.uiActionDelayTime = ShnGetU32(*t, _r, "ActionDelayTime");
        rec.uiBUseEventAction = (uint8)ShnGetU32(*t, _r, "bUseEventAction");
        rec.kEqu_RightHand = ShnGetStr(*t, _r, "Equ_RightHand");
        rec.kEqu_LeftHand = ShnGetStr(*t, _r, "Equ_LeftHand");
        rec.kEqu_Body = ShnGetStr(*t, _r, "Equ_Body");
        rec.kEqu_Leg = ShnGetStr(*t, _r, "Equ_Leg");
        rec.kEqu_Shoes = ShnGetStr(*t, _r, "Equ_Shoes");
        rec.kEqu_AccBody = ShnGetStr(*t, _r, "Equ_AccBody");
        rec.kEqu_AccLeg = ShnGetStr(*t, _r, "Equ_AccLeg");
        rec.kEqu_AccShoes = ShnGetStr(*t, _r, "Equ_AccShoes");
        rec.kEqu_AccMouth = ShnGetStr(*t, _r, "Equ_AccMouth");
        rec.kEqu_AccHeadA = ShnGetStr(*t, _r, "Equ_AccHeadA");
        rec.kEqu_AccEye = ShnGetStr(*t, _r, "Equ_AccEye");
        rec.kEqu_AccHead = ShnGetStr(*t, _r, "Equ_AccHead");
        rec.kEqu_AccLeftHand = ShnGetStr(*t, _r, "Equ_AccLeftHand");
        rec.kEqu_AccRightHand = ShnGetStr(*t, _r, "Equ_AccRightHand");
        rec.kEqu_AccBack = ShnGetStr(*t, _r, "Equ_AccBack");
        rec.kEqu_AccWeast = ShnGetStr(*t, _r, "Equ_AccWeast");
        rec.kEqu_AccHip = ShnGetStr(*t, _r, "Equ_AccHip");
        rec.kEqu_MiniMon = ShnGetStr(*t, _r, "Equ_MiniMon");
        rec.kEqu_MiniMon_R = ShnGetStr(*t, _r, "Equ_MiniMon_R");
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("NPCViewInfo.shn: %u rows", (uint32)m_kRows.size());
}

} // namespace shine
