// Server/DataReader/SHN/ItemServerEquipTypeInfo.h
// Auto-generated: one-file-per-SHN split for ItemServerEquipTypeInfo.shn
#ifndef FIESTA_DATAREADER_SHN_ITEMSERVEREQUIPTYPEINFO_H
#define FIESTA_DATAREADER_SHN_ITEMSERVEREQUIPTYPEINFO_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace fiesta {

struct ItemServerEquipTypeInfoRow {
    uint32           uiISET_Index;
    uint8            uiEqu_Neckles;
    uint8            uiEqu_Head;
    uint8            uiEqu_Ear;
    uint8            uiEqu_RightHand;
    uint8            uiEqu_Body;
    uint8            uiEqu_LeftHand;
    uint8            uiEqu_RingA;
    uint8            uiEqu_Pant;
    uint8            uiEqu_RingB;
    uint8            uiEqu_Boot;
    uint8            uiEqu_AccBoot;
    uint8            uiEqu_AccPant;
    uint8            uiEqu_AccBody;
    uint8            uiEqu_AccMouth;
    uint8            uiEqu_AccEye;
    uint8            uiEqu_AccHead;
    uint8            uiEqu_AccLeftHand;
    uint8            uiEqu_AccRightHand;
    uint8            uiEqu_AccBack;
    uint8            uiEqu_CosEff;
    uint8            uiEqu_AccHip;
    uint8            uiEqu_MiniMon;
    uint8            uiEqu_MiniMon_R;
    uint8            uiEqu_AccShield;
    uint8            uiEqu_Bracelet;
};

class ItemServerEquipTypeInfoShn {
public:
    static ItemServerEquipTypeInfoShn& Get();
    void Load();
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<ItemServerEquipTypeInfoRow>& Rows() const { return m_kRows; }
private:
    std::vector<ItemServerEquipTypeInfoRow>         m_kRows;
};

} // namespace fiesta
#endif // FIESTA_DATAREADER_SHN_ITEMSERVEREQUIPTYPEINFO_H
