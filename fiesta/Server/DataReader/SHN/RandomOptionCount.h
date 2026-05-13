// Server/DataReader/SHN/RandomOptionCount.h
// Auto-generated: one-file-per-SHN split for RandomOptionCount.shn
#ifndef SHINE_DATAREADER_SHN_RANDOMOPTIONCOUNT_H
#define SHINE_DATAREADER_SHN_RANDOMOPTIONCOUNT_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace shine {

struct RandomOptionCountRow {
    std::string      kInxName;
    uint16           uiLimitCount;
    uint16           uiLimitDropRate;
};

class RandomOptionCountShn {
public:
    static RandomOptionCountShn& Get();
    void Load();
    const RandomOptionCountRow* FindByInx(const std::string& rInx) const;
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<RandomOptionCountRow>& Rows() const { return m_kRows; }
private:
    std::vector<RandomOptionCountRow>         m_kRows;
    std::map<std::string, size_t>  m_kByInx;
};

} // namespace shine
#endif // SHINE_DATAREADER_SHN_RANDOMOPTIONCOUNT_H
