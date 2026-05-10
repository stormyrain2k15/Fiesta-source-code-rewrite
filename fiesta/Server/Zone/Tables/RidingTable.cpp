// Server/Zone/Tables/RidingTable.cpp
// FEATURE: world-creation -- Riding.shn binder.
// Mount/ride speed lookup by mover id.
#include "BindMacros.h"
#include "../MiscTables.h"

namespace fiesta {

RidingTable& RidingTable::Get() { static RidingTable s; return s; }

void RidingTable::Bind() {
    // FEATURE: world-creation -- column read: ID, Speed
    if (const ShnFile* t = ShnRegistry::Get().GetTable("Riding"))
        for (size_t i = 0; i < t->Rows().size(); ++i)
            m_kSpeed[ShnGetU32(*t, i, "ID")] = ShnGetI32(*t, i, "Speed");
}

int32 RidingTable::SpeedFor(uint32 uiID) const {
    std::map<uint32, int32>::const_iterator it = m_kSpeed.find(uiID);
    return (it == m_kSpeed.end()) ? 0 : it->second;
}

} // namespace fiesta
