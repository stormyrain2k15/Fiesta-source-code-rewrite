// Server/DataReader/SHN/ColorInfo.h
// Auto-generated: one-file-per-SHN split for ColorInfo.shn
#ifndef FIESTA_DATAREADER_SHN_COLORINFO_H
#define FIESTA_DATAREADER_SHN_COLORINFO_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace fiesta {

struct ColorInfoRow {
    uint32           uiEColorID;
    uint8            uiColorR;
    uint8            uiColorG;
    uint8            uiColorB;
};

class ColorInfoShn {
public:
    static ColorInfoShn& Get();
    void Load();
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<ColorInfoRow>& Rows() const { return m_kRows; }
private:
    std::vector<ColorInfoRow>         m_kRows;
};

} // namespace fiesta
#endif // FIESTA_DATAREADER_SHN_COLORINFO_H
