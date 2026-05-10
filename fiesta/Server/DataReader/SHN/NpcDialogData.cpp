// Server/DataReader/SHN/NpcDialogData.cpp
// Auto-generated: one-file-per-SHN split for NpcDialogData.shn
#include "NpcDialogData.h"
#include "../../Shared/ShineLogSystem.h"

namespace fiesta {

NpcDialogDataShn& NpcDialogDataShn::Get() { static NpcDialogDataShn s; return s; }

void NpcDialogDataShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("NpcDialogData");
    if (!t) { SHINELOG_WARN("NpcDialogData.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        NpcDialogDataRow rec;
        rec.uiID = ShnGetU32(*t, _r, "ID");
        rec.kInxName = ShnGetStr(*t, _r, "InxName");
        m_kById[rec.uiID] = m_kRows.size();
        m_kByInx[rec.kInxName] = m_kRows.size();
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("NpcDialogData.shn: %u rows", (uint32)m_kRows.size());
}

const NpcDialogDataRow* NpcDialogDataShn::FindById(uint32 uiID) const {
    std::map<uint32, size_t>::const_iterator it = m_kById.find(uiID);
    return (it == m_kById.end()) ? NULL : &m_kRows[it->second];
}

const NpcDialogDataRow* NpcDialogDataShn::FindByInx(const std::string& rInx) const {
    std::map<std::string, size_t>::const_iterator it = m_kByInx.find(rInx);
    return (it == m_kByInx.end()) ? NULL : &m_kRows[it->second];
}

} // namespace fiesta
