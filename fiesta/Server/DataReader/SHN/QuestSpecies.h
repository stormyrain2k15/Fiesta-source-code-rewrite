// Server/DataReader/SHN/QuestSpecies.h
// Auto-generated: one-file-per-SHN split for QuestSpecies.shn
#ifndef SHINE_DATAREADER_SHN_QUESTSPECIES_H
#define SHINE_DATAREADER_SHN_QUESTSPECIES_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace shine {

struct QuestSpeciesRow {
    uint16           uiID;
    std::string      kMobGroupName;
    std::string      kUnkCol2;
    std::string      kUnkCol3;
    std::string      kUnkCol4;
    std::string      kUnkCol5;
};

class QuestSpeciesShn {
public:
    static QuestSpeciesShn& Get();
    void Load();
    const QuestSpeciesRow* FindById(uint32 uiID) const;
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<QuestSpeciesRow>& Rows() const { return m_kRows; }
private:
    std::vector<QuestSpeciesRow>         m_kRows;
    std::map<uint32,      size_t>  m_kById;
};

} // namespace shine
#endif // SHINE_DATAREADER_SHN_QUESTSPECIES_H
