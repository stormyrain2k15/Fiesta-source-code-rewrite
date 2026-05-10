// Server/DataReader/SHN/HairInfo.h
// Auto-generated: one-file-per-SHN split for HairInfo.shn
#ifndef FIESTA_DATAREADER_SHN_HAIRINFO_H
#define FIESTA_DATAREADER_SHN_HAIRINFO_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace fiesta {

struct HairInfoRow {
    uint8            uiID;
    std::string      kIndexName;
    std::string      kHairName;
    uint8            uiGrade;
    uint32           uiFighter;
    uint32           uiArcher;
    uint32           uiCleric;
    uint32           uiMage;
    uint32           uiJoker;
    uint32           uiSentinel;
    uint8            uiUcIsLink_Front;
    std::string      kAcModelName_Front;
    std::string      kFrontTex;
    uint8            uiUcIsLink_Bottom;
    std::string      kAcModelName_Bottom;
    std::string      kBottomTex;
    uint8            uiUcIsLink_Top;
    std::string      kAcModelName_Top;
    std::string      kTopTex;
    uint32           uiException1;
    uint32           uiException2;
    uint8            uiUcIsLink_Acc;
    std::string      kAcModelName_Acc;
    std::string      kAcc1Tex;
    uint8            uiUcIsLink_Acc2;
    std::string      kAcModelName_Acc2;
    std::string      kAcc2Tex;
    uint8            uiUcIsLink_Acc3;
    std::string      kAcModelName_Acc3;
    std::string      kAcc3Tex;
};

class HairInfoShn {
public:
    static HairInfoShn& Get();
    void Load();
    const HairInfoRow* FindById(uint32 uiID) const;
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<HairInfoRow>& Rows() const { return m_kRows; }
private:
    std::vector<HairInfoRow>         m_kRows;
    std::map<uint32,      size_t>  m_kById;
};

} // namespace fiesta
#endif // FIESTA_DATAREADER_SHN_HAIRINFO_H
