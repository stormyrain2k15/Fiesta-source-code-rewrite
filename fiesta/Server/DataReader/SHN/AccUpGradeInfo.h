// Server/DataReader/SHN/AccUpGradeInfo.h
// Auto-generated: one-file-per-SHN split for AccUpGradeInfo.shn
#ifndef SHINE_DATAREADER_SHN_ACCUPGRADEINFO_H
#define SHINE_DATAREADER_SHN_ACCUPGRADEINFO_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace shine {

struct AccUpGradeInfoRow {
    uint16           uiID;
    std::string      kInxName;
    uint32           uiUpFactor;
    uint16           uiSuccess;
    uint16           uiMin1;
    uint16           uiMax1;
    uint16           uiLimit1;
    uint16           uiMin2;
    uint16           uiMax2;
    uint16           uiLimit2;
    uint16           uiMin3;
    uint16           uiMax3;
    uint16           uiLimit3;
    uint16           uiMin4;
    uint16           uiMax4;
    uint16           uiLimit4;
    uint16           uiMin5;
    uint16           uiMax5;
    uint16           uiLimit5;
    uint16           uiMin6;
    uint16           uiMax6;
    uint16           uiLimit6;
    uint16           uiMin7;
    uint16           uiMax7;
    uint16           uiLimit7;
    uint16           uiMin8;
    uint16           uiMax8;
    uint16           uiLimit8;
    uint16           uiMin9;
    uint16           uiMax9;
    uint16           uiLimit9;
    uint16           uiMin10;
    uint16           uiMax10;
    uint16           uiLimit10;
    uint16           uiMin11;
    uint16           uiMax11;
    uint16           uiLimit11;
    uint16           uiMin12;
    uint16           uiMax12;
    uint16           uiLimit12;
};

class AccUpGradeInfoShn {
public:
    static AccUpGradeInfoShn& Get();
    void Load();
    const AccUpGradeInfoRow* FindById(uint32 uiID) const;
    const AccUpGradeInfoRow* FindByInx(const std::string& rInx) const;
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<AccUpGradeInfoRow>& Rows() const { return m_kRows; }
private:
    std::vector<AccUpGradeInfoRow>         m_kRows;
    std::map<uint32,      size_t>  m_kById;
    std::map<std::string, size_t>  m_kByInx;
};

} // namespace shine
#endif // SHINE_DATAREADER_SHN_ACCUPGRADEINFO_H
