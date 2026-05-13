// Server/DataReader/SHN/QuestScript.h
// Auto-generated: one-file-per-SHN split for QuestScript.shn
#ifndef SHINE_DATAREADER_SHN_QUESTSCRIPT_H
#define SHINE_DATAREADER_SHN_QUESTSCRIPT_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace shine {

struct QuestScriptRow {
    uint16           uiID;
    std::string      kScript;
};

class QuestScriptShn {
public:
    static QuestScriptShn& Get();
    void Load();
    const QuestScriptRow* FindById(uint32 uiID) const;
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<QuestScriptRow>& Rows() const { return m_kRows; }
private:
    std::vector<QuestScriptRow>         m_kRows;
    std::map<uint32,      size_t>  m_kById;
};

} // namespace shine
#endif // SHINE_DATAREADER_SHN_QUESTSCRIPT_H
