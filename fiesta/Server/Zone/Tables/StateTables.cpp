// Server/Zone/Tables/StateTables.cpp
// FEATURE: world-creation -- StateField.shn + StateItem.shn + StateMob.shn
// Three coupled SHNs: per-map field-state effect, per-item item-state
// effect, per-mob mob-state effect. All share the same shape (ID,
// Effect) and one StateTables surface.
#include "BindMacros.h"
#include "../MiscTables.h"

namespace shine {

StateTables& StateTables::Get() { static StateTables s; return s; }

void StateTables::Bind() {
    // FEATURE: world-creation -- column read: ID, Effect
    if (const ShnFile* t = ShnRegistry::Get().GetTable("StateField"))
        for (size_t i = 0; i < t->Rows().size(); ++i)
            m_kField[ShnGetU32(*t, i, "ID")] = ShnGetI32(*t, i, "Effect");
    if (const ShnFile* t = ShnRegistry::Get().GetTable("StateItem"))
        for (size_t i = 0; i < t->Rows().size(); ++i)
            m_kItem [ShnGetU32(*t, i, "ID")] = ShnGetI32(*t, i, "Effect");
    if (const ShnFile* t = ShnRegistry::Get().GetTable("StateMob"))
        for (size_t i = 0; i < t->Rows().size(); ++i)
            m_kMob  [ShnGetU32(*t, i, "ID")] = ShnGetI32(*t, i, "Effect");
}

int32 StateTables::FieldEffect(uint32 uiMapID) const {
    std::map<uint32, int32>::const_iterator it = m_kField.find(uiMapID);
    return (it == m_kField.end()) ? 0 : it->second;
}
int32 StateTables::ItemEffect (uint32 uiItem)  const {
    std::map<uint32, int32>::const_iterator it = m_kItem.find(uiItem);
    return (it == m_kItem.end()) ? 0 : it->second;
}
int32 StateTables::MobEffect  (uint32 uiMob)   const {
    std::map<uint32, int32>::const_iterator it = m_kMob.find(uiMob);
    return (it == m_kMob.end()) ? 0 : it->second;
}

} // namespace shine
