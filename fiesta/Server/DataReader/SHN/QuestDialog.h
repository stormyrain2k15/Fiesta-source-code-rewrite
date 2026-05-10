// Server/DataReader/SHN/QuestDialog.h
// Auto-generated: one-file-per-SHN split for QuestDialog.shn
#ifndef FIESTA_DATAREADER_SHN_QUESTDIALOG_H
#define FIESTA_DATAREADER_SHN_QUESTDIALOG_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace fiesta {

struct QuestDialogRow {
    uint32           uiID;
    std::string      kDialog;
};

class QuestDialogShn {
public:
    static QuestDialogShn& Get();
    void Load();
    const QuestDialogRow* FindById(uint32 uiID) const;
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<QuestDialogRow>& Rows() const { return m_kRows; }
private:
    std::vector<QuestDialogRow>         m_kRows;
    std::map<uint32,      size_t>  m_kById;
};

} // namespace fiesta
#endif // FIESTA_DATAREADER_SHN_QUESTDIALOG_H
