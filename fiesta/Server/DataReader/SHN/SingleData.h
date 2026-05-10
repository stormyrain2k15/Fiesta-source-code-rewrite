// Server/DataReader/SHN/SingleData.h
// Auto-generated: one-file-per-SHN split for SingleData.shn
#ifndef FIESTA_DATAREADER_SHN_SINGLEDATA_H
#define FIESTA_DATAREADER_SHN_SINGLEDATA_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace fiesta {

struct SingleDataRow {
    std::string      kSingleDataIDX;
    uint16           uiSingleDataValue;
};

class SingleDataShn {
public:
    static SingleDataShn& Get();
    void Load();
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<SingleDataRow>& Rows() const { return m_kRows; }
private:
    std::vector<SingleDataRow>         m_kRows;
};

} // namespace fiesta
#endif // FIESTA_DATAREADER_SHN_SINGLEDATA_H
