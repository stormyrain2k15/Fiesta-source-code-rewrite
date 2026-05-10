// Server/DataReader/SHN/MobSpecies.h
// Auto-generated: one-file-per-SHN split for MobSpecies.shn
#ifndef FIESTA_DATAREADER_SHN_MOBSPECIES_H
#define FIESTA_DATAREADER_SHN_MOBSPECIES_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace fiesta {

struct MobSpeciesRow {
    uint16           uiID;
    std::string      kMobName;
    std::string      kUnkCol2;
    std::string      kUnkCol3;
    std::string      kUnkCol4;
};

class MobSpeciesShn {
public:
    static MobSpeciesShn& Get();
    void Load();
    const MobSpeciesRow* FindById(uint32 uiID) const;
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<MobSpeciesRow>& Rows() const { return m_kRows; }
private:
    std::vector<MobSpeciesRow>         m_kRows;
    std::map<uint32,      size_t>  m_kById;
};

} // namespace fiesta
#endif // FIESTA_DATAREADER_SHN_MOBSPECIES_H
