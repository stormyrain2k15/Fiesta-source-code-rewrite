// Server/DataReader/SHN/FaceInfo.h
// Auto-generated: one-file-per-SHN split for FaceInfo.shn
#ifndef FIESTA_DATAREADER_SHN_FACEINFO_H
#define FIESTA_DATAREADER_SHN_FACEINFO_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace fiesta {

struct FaceInfoRow {
    uint8            uiID;
    std::string      kFaceName;
    uint8            uiGrade;
    uint8            uiFM_F_Male;
    uint8            uiFT_F_Male;
    uint8            uiFM_F_Female;
    uint8            uiFT_F_Female;
    uint8            uiFM_C_Male;
    uint8            uiFT_C_Male;
    uint8            uiFM_C_Female;
    uint8            uiFT_C_Female;
    uint8            uiFM_A_Male;
    uint8            uiFT_A_Male;
    uint8            uiFM_A_Female;
    uint8            uiFT_A_Female;
    uint8            uiFM_M_Male;
    uint8            uiFT_M_Male;
    uint8            uiFM_M_Female;
    uint8            uiFT_M_Female;
    uint8            uiFM_J_Male;
    uint8            uiFT_J_Male;
    uint8            uiFM_J_Female;
    uint8            uiFT_J_Female;
    uint8            uiFM_S_Male;
    uint8            uiFT_S_Male;
    uint8            uiFM_S_Female;
    uint8            uiFT_S_Female;
};

class FaceInfoShn {
public:
    static FaceInfoShn& Get();
    void Load();
    const FaceInfoRow* FindById(uint32 uiID) const;
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<FaceInfoRow>& Rows() const { return m_kRows; }
private:
    std::vector<FaceInfoRow>         m_kRows;
    std::map<uint32,      size_t>  m_kById;
};

} // namespace fiesta
#endif // FIESTA_DATAREADER_SHN_FACEINFO_H
