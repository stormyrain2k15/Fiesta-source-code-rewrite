// Server/DataReader/SHN/RandomOption.h
// Auto-generated: one-file-per-SHN split for RandomOption.shn
#ifndef FIESTA_DATAREADER_SHN_RANDOMOPTION_H
#define FIESTA_DATAREADER_SHN_RANDOMOPTION_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace fiesta {

struct RandomOptionRow {
    std::string      kDropItemIndex;
    uint32           uiRandomOptionType;
    uint32           uiMin;
    uint32           uiMax;
    uint32           uiTypeDropRate;
};

class RandomOptionShn {
public:
    static RandomOptionShn& Get();
    void Load();
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<RandomOptionRow>& Rows() const { return m_kRows; }
private:
    std::vector<RandomOptionRow>         m_kRows;
};

} // namespace fiesta
#endif // FIESTA_DATAREADER_SHN_RANDOMOPTION_H
