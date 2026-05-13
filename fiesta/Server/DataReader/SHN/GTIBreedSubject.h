// Server/DataReader/SHN/GTIBreedSubject.h
// Auto-generated: one-file-per-SHN split for GTIBreedSubject.shn
#ifndef SHINE_DATAREADER_SHN_GTIBREEDSUBJECT_H
#define SHINE_DATAREADER_SHN_GTIBREEDSUBJECT_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace shine {

struct GTIBreedSubjectRow {
    std::string      kInxName;
    std::string      kMobIndex;
};

class GTIBreedSubjectShn {
public:
    static GTIBreedSubjectShn& Get();
    void Load();
    const GTIBreedSubjectRow* FindByInx(const std::string& rInx) const;
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<GTIBreedSubjectRow>& Rows() const { return m_kRows; }
private:
    std::vector<GTIBreedSubjectRow>         m_kRows;
    std::map<std::string, size_t>  m_kByInx;
};

} // namespace shine
#endif // SHINE_DATAREADER_SHN_GTIBREEDSUBJECT_H
