// Server/DataReader/SHN/ClassName.h
// Auto-generated: one-file-per-SHN split for ClassName.shn
#ifndef SHINE_DATAREADER_SHN_CLASSNAME_H
#define SHINE_DATAREADER_SHN_CLASSNAME_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace shine {

struct ClassNameRow {
    uint8            uiClassID;
    std::string      kAcPrefix;
    std::string      kAcEngName;
    std::string      kAcLocalName;
};

class ClassNameShn {
public:
    static ClassNameShn& Get();
    void Load();
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<ClassNameRow>& Rows() const { return m_kRows; }
    // Looks up by ClassID (uiClassID column).
    const ClassNameRow* FindById(uint32 uiID) const;
private:
    std::vector<ClassNameRow>         m_kRows;
    std::map<uint32, size_t>          m_kById;
};

} // namespace shine
#endif // SHINE_DATAREADER_SHN_CLASSNAME_H
