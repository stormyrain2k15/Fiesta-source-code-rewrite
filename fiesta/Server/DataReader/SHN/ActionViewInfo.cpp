// Server/DataReader/SHN/ActionViewInfo.cpp
// Auto-generated: one-file-per-SHN split for ActionViewInfo.shn
#include "ActionViewInfo.h"
#include "../../Shared/ShineLogSystem.h"

namespace shine {

ActionViewInfoShn& ActionViewInfoShn::Get() { static ActionViewInfoShn s; return s; }

void ActionViewInfoShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("ActionViewInfo");
    if (!t) { SHINELOG_WARN("ActionViewInfo.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        ActionViewInfoRow rec;
        rec.uiNIndex = (uint8)ShnGetU32(*t, _r, "nIndex");
        rec.kInxName = ShnGetStr(*t, _r, "InxName");
        rec.kActionName = ShnGetStr(*t, _r, "ActionName");
        rec.uiLinkActionIndex = (uint16)ShnGetU32(*t, _r, "LinkActionIndex");
        rec.kIconFileName = ShnGetStr(*t, _r, "IconFileName");
        rec.uiNIconNum = (uint16)ShnGetU32(*t, _r, "nIconNum");
        rec.uiEActionType = ShnGetU32(*t, _r, "eActionType");
        rec.uiNEventCode = ShnGetU32(*t, _r, "nEventCode");
        rec.uiNAfterCode = ShnGetU32(*t, _r, "nAfterCode");
        rec.uiIsDance = (uint8)ShnGetU32(*t, _r, "IsDance");
        m_kByInx[rec.kInxName] = m_kRows.size();
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("ActionViewInfo.shn: %u rows", (uint32)m_kRows.size());
}

const ActionViewInfoRow* ActionViewInfoShn::FindByInx(const std::string& rInx) const {
    std::map<std::string, size_t>::const_iterator it = m_kByInx.find(rInx);
    return (it == m_kByInx.end()) ? NULL : &m_kRows[it->second];
}

} // namespace shine
