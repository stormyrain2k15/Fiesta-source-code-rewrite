// Server/Zone/Tables/CraftTables.cpp
// FEATURE: world-creation -- TownPortal.shn (presence) +
// Gather.shn + Produce.shn. Three coupled SHNs answering the simple
// "is this id a known crafting/portal entry?" question. Note the
// fuller TownPortal binder (with destination data) lives in
// TownPortalSystem.cpp -- this one only tracks ID presence for
// dispatch checks.
#include "BindMacros.h"
#include "../MiscTables.h"

namespace fiesta {

CraftTables& CraftTables::Get() { static CraftTables s; return s; }

void CraftTables::Bind() {
    // FEATURE: world-creation -- column read: ID
    if (const ShnFile* t = ShnRegistry::Get().GetTable("TownPortal"))
        for (size_t i = 0; i < t->Rows().size(); ++i)
            m_kTP[ShnGetU32(*t, i, "ID")] = true;
    if (const ShnFile* t = ShnRegistry::Get().GetTable("Gather"))
        for (size_t i = 0; i < t->Rows().size(); ++i)
            m_kGather[ShnGetU32(*t, i, "ID")] = true;
    if (const ShnFile* t = ShnRegistry::Get().GetTable("Produce"))
        for (size_t i = 0; i < t->Rows().size(); ++i)
            m_kProduce[ShnGetU32(*t, i, "ID")] = true;
}

bool CraftTables::HasTownPortal(uint32 u) const { return m_kTP.find(u) != m_kTP.end(); }
bool CraftTables::HasGather    (uint32 u) const { return m_kGather.find(u) != m_kGather.end(); }
bool CraftTables::HasProduce   (uint32 u) const { return m_kProduce.find(u) != m_kProduce.end(); }

} // namespace fiesta
