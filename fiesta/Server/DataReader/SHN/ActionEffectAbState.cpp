// Server/DataReader/SHN/ActionEffectAbState.cpp
// Auto-generated: one-file-per-SHN split for ActionEffectAbState.shn
#include "ActionEffectAbState.h"
#include "../../Shared/ShineLogSystem.h"

namespace shine {

ActionEffectAbStateShn& ActionEffectAbStateShn::Get() { static ActionEffectAbStateShn s; return s; }

void ActionEffectAbStateShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("ActionEffectAbState");
    if (!t) { SHINELOG_WARN("ActionEffectAbState.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        ActionEffectAbStateRow rec;
        rec.kInxName = ShnGetStr(*t, _r, "InxName");
        rec.uiStrength = ShnGetU32(*t, _r, "Strength");
        rec.uiItemActionID = (uint16)ShnGetU32(*t, _r, "ItemActionID");
        m_kByInx[rec.kInxName] = m_kRows.size();
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("ActionEffectAbState.shn: %u rows", (uint32)m_kRows.size());
}

const ActionEffectAbStateRow* ActionEffectAbStateShn::FindByInx(const std::string& rInx) const {
    std::map<std::string, size_t>::const_iterator it = m_kByInx.find(rInx);
    return (it == m_kByInx.end()) ? NULL : &m_kRows[it->second];
}

} // namespace shine
