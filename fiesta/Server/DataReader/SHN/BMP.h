// Server/DataReader/SHN/BMP.h
// Auto-generated: one-file-per-SHN split for BMP.shn
#ifndef FIESTA_DATAREADER_SHN_BMP_H
#define FIESTA_DATAREADER_SHN_BMP_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace fiesta {

struct BMPRow {
    std::string      kBMPIndex;
    std::string      kFileName;
};

class BMPShn {
public:
    static BMPShn& Get();
    void Load();
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<BMPRow>& Rows() const { return m_kRows; }
private:
    std::vector<BMPRow>         m_kRows;
};

} // namespace fiesta
#endif // FIESTA_DATAREADER_SHN_BMP_H
