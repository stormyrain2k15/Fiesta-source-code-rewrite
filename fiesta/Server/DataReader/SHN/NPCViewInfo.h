// Server/DataReader/SHN/NPCViewInfo.h
// Auto-generated: one-file-per-SHN split for NPCViewInfo.shn
#ifndef FIESTA_DATAREADER_SHN_NPCVIEWINFO_H
#define FIESTA_DATAREADER_SHN_NPCVIEWINFO_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace fiesta {

struct NPCViewInfoRow {
    uint16           uiTypeIndex;
    uint32           uiClass;
    uint16           uiGender;
    uint8            uiFaceShape;
    uint8            uiHairType;
    uint8            uiHairColor;
    uint32           uiBaseActionCode;
    uint32           uiPeriodActionCode;
    uint32           uiActionDelayTime;
    uint8            uiBUseEventAction;
    std::string      kEqu_RightHand;
    std::string      kEqu_LeftHand;
    std::string      kEqu_Body;
    std::string      kEqu_Leg;
    std::string      kEqu_Shoes;
    std::string      kEqu_AccBody;
    std::string      kEqu_AccLeg;
    std::string      kEqu_AccShoes;
    std::string      kEqu_AccMouth;
    std::string      kEqu_AccHeadA;
    std::string      kEqu_AccEye;
    std::string      kEqu_AccHead;
    std::string      kEqu_AccLeftHand;
    std::string      kEqu_AccRightHand;
    std::string      kEqu_AccBack;
    std::string      kEqu_AccWeast;
    std::string      kEqu_AccHip;
    std::string      kEqu_MiniMon;
    std::string      kEqu_MiniMon_R;
};

class NPCViewInfoShn {
public:
    static NPCViewInfoShn& Get();
    void Load();
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<NPCViewInfoRow>& Rows() const { return m_kRows; }
private:
    std::vector<NPCViewInfoRow>         m_kRows;
};

} // namespace fiesta
#endif // FIESTA_DATAREADER_SHN_NPCVIEWINFO_H
