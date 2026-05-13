// Server/Zone/Tables/MountTables.cpp
// FEATURE: world-creation -- moved from GroupTables.cpp under
// the one-cpp-per-shn convention (docs/PER_SHN_CONVENTION.md).
#include "BindMacros.h"
#include "../GroupTables.h"

namespace shine {

MountTables& MountTables::Get() { static MountTables s; return s; }
void MountTables::Bind() {
    if (const ShnFile* t = ShnRegistry::Get().GetTable("MoverMain")) {
        ITER_ROWS(t) {
            LegacyMoverMainRow rec;
            rec.uiMoverID = ShnGetU32(*t, _r, "MoverID");
            rec.kMoverIDX = ShnGetStr(*t, _r, "MoverIDX");
            rec.uiCastMs  = ShnGetU32(*t, _r, "CastingTime");
            rec.uiCoolMs  = ShnGetU32(*t, _r, "CoolTime");
            rec.uiRun     = ShnGetU32(*t, _r, "RunSpeed");
            rec.uiWalk    = ShnGetU32(*t, _r, "WalkSpeed");
            rec.uiHours   = ShnGetU32(*t, _r, "DurationHour");
            rec.uiMaxSlot = ShnGetU32(*t, _r, "MaxCharSlot");
            m_kMainById[rec.uiMoverID] = m_kMain.size();
            m_kMain.push_back(rec);
        }
    }
    if (const ShnFile* t = ShnRegistry::Get().GetTable("MoverItem")) {
        ITER_ROWS(t) {
            LegacyMoverItemRow rec;
            rec.uiID     = ShnGetU32(*t, _r, "ID");
            rec.kItemIDX = ShnGetStr(*t, _r, "ItemIDX");
            m_kItemById[rec.uiID] = m_kItem.size();
            m_kItem.push_back(rec);
        }
    }
}
const LegacyMoverMainRow* MountTables::FindMain(uint32 uiID) const {
    std::map<uint32, size_t>::const_iterator it = m_kMainById.find(uiID);
    return (it == m_kMainById.end()) ? NULL : &m_kMain[it->second]; }
const LegacyMoverItemRow* MountTables::FindItem(uint32 uiID) const {
    std::map<uint32, size_t>::const_iterator it = m_kItemById.find(uiID);
    return (it == m_kItemById.end()) ? NULL : &m_kItem[it->second]; }


} // namespace shine
