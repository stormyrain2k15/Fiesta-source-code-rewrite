// Server/DataReader/SHN/QuestData.h
// Auto-generated: one-file-per-SHN split for QuestData.shn
#ifndef SHINE_DATAREADER_SHN_QUESTDATA_H
#define SHINE_DATAREADER_SHN_QUESTDATA_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace shine {

struct QuestDataRow {
    uint32           uiID;
    std::string      kInxName;
};

class QuestDataShn {
public:
    static QuestDataShn& Get();
    void Load();
    const QuestDataRow* FindById(uint32 uiID) const;
    const QuestDataRow* FindByInx(const std::string& rInx) const;
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<QuestDataRow>& Rows() const { return m_kRows; }
private:
    std::vector<QuestDataRow>         m_kRows;
    std::map<uint32,      size_t>  m_kById;
    std::map<std::string, size_t>  m_kByInx;
};

} // namespace shine
#endif // SHINE_DATAREADER_SHN_QUESTDATA_H
