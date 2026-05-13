// Server/Zone/Tables/ActionTables.cpp
// FEATURE: world-creation -- ActionEffectItem.shn + ActionRangeFactor.shn
// Two coupled SHNs: per-item action effect id, and per-action-kind
// range scaler. Sharing one .cpp because they form a single
// ActionTables surface used by combat/skill range checks.
#include "BindMacros.h"
#include "../MiscTables.h"

namespace shine {

ActionTables& ActionTables::Get() { static ActionTables s; return s; }

void ActionTables::Bind() {
    // FEATURE: world-creation -- column read: ID, Effect
    if (const ShnFile* t = ShnRegistry::Get().GetTable("ActionEffectItem")) {
        for (size_t i = 0; i < t->Rows().size(); ++i) {
            m_kEffect[ShnGetU32(*t, i, "ID")] = ShnGetI32(*t, i, "Effect");
        }
    }
    // FEATURE: world-creation -- column read: Factor
    if (const ShnFile* t = ShnRegistry::Get().GetTable("ActionRangeFactor")) {
        m_kRangeFactor.reserve(t->Rows().size());
        for (size_t i = 0; i < t->Rows().size(); ++i) {
            m_kRangeFactor.push_back(ShnGetI32(*t, i, "Factor"));
        }
    }
}

int32 ActionTables::ItemEffect(uint32 uiID) const {
    std::map<uint32, int32>::const_iterator it = m_kEffect.find(uiID);
    return (it == m_kEffect.end()) ? 0 : it->second;
}
int32 ActionTables::RangeFactor(uint32 uiKind) const {
    return (uiKind < m_kRangeFactor.size()) ? m_kRangeFactor[uiKind] : 1000;
}

} // namespace shine
