// Server/DataReader/SHN/ItemServerEquipTypeInfo.cpp
// Auto-generated: one-file-per-SHN split for ItemServerEquipTypeInfo.shn
#include "ItemServerEquipTypeInfo.h"
#include "../../Shared/ShineLogSystem.h"

namespace fiesta {

ItemServerEquipTypeInfoShn& ItemServerEquipTypeInfoShn::Get() { static ItemServerEquipTypeInfoShn s; return s; }

void ItemServerEquipTypeInfoShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("ItemServerEquipTypeInfo");
    if (!t) { SHINELOG_WARN("ItemServerEquipTypeInfo.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        ItemServerEquipTypeInfoRow rec;
        rec.uiISET_Index = ShnGetU32(*t, _r, "ISET_Index");
        rec.uiEqu_Neckles = (uint8)ShnGetU32(*t, _r, "Equ_Neckles");
        rec.uiEqu_Head = (uint8)ShnGetU32(*t, _r, "Equ_Head");
        rec.uiEqu_Ear = (uint8)ShnGetU32(*t, _r, "Equ_Ear");
        rec.uiEqu_RightHand = (uint8)ShnGetU32(*t, _r, "Equ_RightHand");
        rec.uiEqu_Body = (uint8)ShnGetU32(*t, _r, "Equ_Body");
        rec.uiEqu_LeftHand = (uint8)ShnGetU32(*t, _r, "Equ_LeftHand");
        rec.uiEqu_RingA = (uint8)ShnGetU32(*t, _r, "Equ_RingA");
        rec.uiEqu_Pant = (uint8)ShnGetU32(*t, _r, "Equ_Pant");
        rec.uiEqu_RingB = (uint8)ShnGetU32(*t, _r, "Equ_RingB");
        rec.uiEqu_Boot = (uint8)ShnGetU32(*t, _r, "Equ_Boot");
        rec.uiEqu_AccBoot = (uint8)ShnGetU32(*t, _r, "Equ_AccBoot");
        rec.uiEqu_AccPant = (uint8)ShnGetU32(*t, _r, "Equ_AccPant");
        rec.uiEqu_AccBody = (uint8)ShnGetU32(*t, _r, "Equ_AccBody");
        rec.uiEqu_AccMouth = (uint8)ShnGetU32(*t, _r, "Equ_AccMouth");
        rec.uiEqu_AccEye = (uint8)ShnGetU32(*t, _r, "Equ_AccEye");
        rec.uiEqu_AccHead = (uint8)ShnGetU32(*t, _r, "Equ_AccHead");
        rec.uiEqu_AccLeftHand = (uint8)ShnGetU32(*t, _r, "Equ_AccLeftHand");
        rec.uiEqu_AccRightHand = (uint8)ShnGetU32(*t, _r, "Equ_AccRightHand");
        rec.uiEqu_AccBack = (uint8)ShnGetU32(*t, _r, "Equ_AccBack");
        rec.uiEqu_CosEff = (uint8)ShnGetU32(*t, _r, "Equ_CosEff");
        rec.uiEqu_AccHip = (uint8)ShnGetU32(*t, _r, "Equ_AccHip");
        rec.uiEqu_MiniMon = (uint8)ShnGetU32(*t, _r, "Equ_MiniMon");
        rec.uiEqu_MiniMon_R = (uint8)ShnGetU32(*t, _r, "Equ_MiniMon_R");
        rec.uiEqu_AccShield = (uint8)ShnGetU32(*t, _r, "Equ_AccShield");
        rec.uiEqu_Bracelet = (uint8)ShnGetU32(*t, _r, "Equ_Bracelet");
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("ItemServerEquipTypeInfo.shn: %u rows", (uint32)m_kRows.size());
}

} // namespace fiesta
