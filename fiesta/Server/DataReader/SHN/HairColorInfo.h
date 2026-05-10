// Server/DataReader/SHN/HairColorInfo.h
// Auto-generated: one-file-per-SHN split for HairColorInfo.shn
#ifndef FIESTA_DATAREADER_SHN_HAIRCOLORINFO_H
#define FIESTA_DATAREADER_SHN_HAIRCOLORINFO_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace fiesta {

struct HairColorInfoRow {
    uint8            uiID;
    std::string      kIndexName;
    std::string      kName;
    std::string      kColorTextureName;
    uint8            uiGrade;
};

class HairColorInfoShn {
public:
    static HairColorInfoShn& Get();
    void Load();
    const HairColorInfoRow* FindById(uint32 uiID) const;
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<HairColorInfoRow>& Rows() const { return m_kRows; }
private:
    std::vector<HairColorInfoRow>         m_kRows;
    std::map<uint32,      size_t>  m_kById;
};

} // namespace fiesta
#endif // FIESTA_DATAREADER_SHN_HAIRCOLORINFO_H
