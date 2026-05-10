// Server/DataReader/SHN/DiceDividind.h
// Auto-generated: one-file-per-SHN split for DiceDividind.shn
#ifndef FIESTA_DATAREADER_SHN_DICEDIVIDIND_H
#define FIESTA_DATAREADER_SHN_DICEDIVIDIND_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace fiesta {

struct DiceDividindRow {
    int16            iDividendRate;
    int16            iUnkCol1;
    int16            iUnkCol2;
    int16            iUnkCol3;
    int16            iUnkCol4;
    int16            iUnkCol5;
    int16            iUnkCol6;
    int16            iUnkCol7;
    int16            iUnkCol8;
    int16            iUnkCol9;
    int16            iUnkCol10;
    int16            iUnkCol11;
    int16            iUnkCol12;
    int16            iUnkCol13;
    int16            iUnkCol14;
    uint8            uiAnyTriple;
};

class DiceDividindShn {
public:
    static DiceDividindShn& Get();
    void Load();
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<DiceDividindRow>& Rows() const { return m_kRows; }
private:
    std::vector<DiceDividindRow>         m_kRows;
};

} // namespace fiesta
#endif // FIESTA_DATAREADER_SHN_DICEDIVIDIND_H
