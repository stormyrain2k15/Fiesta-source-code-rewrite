// Server/DataReader/SHN/BadNameFilter.h
// Auto-generated: one-file-per-SHN split for BadNameFilter.shn
#ifndef SHINE_DATAREADER_SHN_BADNAMEFILTER_H
#define SHINE_DATAREADER_SHN_BADNAMEFILTER_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace shine {

struct BadNameFilterRow {
    std::string      kBadName;
    uint32           uiType;
};

class BadNameFilterShn {
public:
    static BadNameFilterShn& Get();
    void Load();
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<BadNameFilterRow>& Rows() const { return m_kRows; }
private:
    std::vector<BadNameFilterRow>         m_kRows;
};

} // namespace shine
#endif // SHINE_DATAREADER_SHN_BADNAMEFILTER_H
