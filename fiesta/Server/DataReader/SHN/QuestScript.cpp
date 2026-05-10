// Server/DataReader/SHN/QuestScript.cpp
// Auto-generated: one-file-per-SHN split for QuestScript.shn
#include "QuestScript.h"
#include "../../Shared/ShineLogSystem.h"

namespace fiesta {

QuestScriptShn& QuestScriptShn::Get() { static QuestScriptShn s; return s; }

void QuestScriptShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("QuestScript");
    if (!t) { SHINELOG_WARN("QuestScript.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        QuestScriptRow rec;
        rec.uiID = (uint16)ShnGetU32(*t, _r, "ID");
        rec.kScript = ShnGetStr(*t, _r, "Script");
        m_kById[rec.uiID] = m_kRows.size();
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("QuestScript.shn: %u rows", (uint32)m_kRows.size());
}

const QuestScriptRow* QuestScriptShn::FindById(uint32 uiID) const {
    std::map<uint32, size_t>::const_iterator it = m_kById.find(uiID);
    return (it == m_kById.end()) ? NULL : &m_kRows[it->second];
}

} // namespace fiesta
