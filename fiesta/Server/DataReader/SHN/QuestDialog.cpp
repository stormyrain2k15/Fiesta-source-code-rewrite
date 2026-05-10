// Server/DataReader/SHN/QuestDialog.cpp
// Auto-generated: one-file-per-SHN split for QuestDialog.shn
#include "QuestDialog.h"
#include "../../Shared/ShineLogSystem.h"

namespace fiesta {

QuestDialogShn& QuestDialogShn::Get() { static QuestDialogShn s; return s; }

void QuestDialogShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("QuestDialog");
    if (!t) { SHINELOG_WARN("QuestDialog.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        QuestDialogRow rec;
        rec.uiID = ShnGetU32(*t, _r, "ID");
        rec.kDialog = ShnGetStr(*t, _r, "Dialog");
        m_kById[rec.uiID] = m_kRows.size();
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("QuestDialog.shn: %u rows", (uint32)m_kRows.size());
}

const QuestDialogRow* QuestDialogShn::FindById(uint32 uiID) const {
    std::map<uint32, size_t>::const_iterator it = m_kById.find(uiID);
    return (it == m_kById.end()) ? NULL : &m_kRows[it->second];
}

} // namespace fiesta
