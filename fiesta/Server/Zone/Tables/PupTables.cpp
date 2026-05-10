// Server/Zone/Tables/PupTables.cpp
// FEATURE: world-creation -- moved from GroupTables.cpp under
// the one-cpp-per-shn convention (docs/PER_SHN_CONVENTION.md).
#include "BindMacros.h"
#include "../GroupTables.h"

namespace fiesta {

PupTables& PupTables::Get() { static PupTables s; return s; }
void PupTables::Bind() {
    if (const ShnFile* t = ShnRegistry::Get().GetTable("PupMain")) {
        ITER_ROWS(t) {
            LegacyPupMainRow rec;
            rec.uiPupID    = ShnGetU32(*t, _r, "PupID");
            rec.kPupIDX    = ShnGetStr(*t, _r, "PupIDX");
            rec.kItemIDX   = ShnGetStr(*t, _r, "ItemIDX");
            rec.uiPupSpeed = ShnGetU32(*t, _r, "PupSpeed");
            m_kMainById[rec.uiPupID] = m_kMain.size();
            m_kMain.push_back(rec);
        }
    }
    if (const ShnFile* t = ShnRegistry::Get().GetTable("PupServer")) {
        ITER_ROWS(t) {
            LegacyPupServerRow rec;
            rec.uiPupID    = ShnGetU32(*t, _r, "PupID");
            rec.uiSpawnHP  = ShnGetU32(*t, _r, "SpawnHP");
            rec.uiSpawnSP  = ShnGetU32(*t, _r, "SpawnSP");
            rec.uiCooldown = ShnGetU32(*t, _r, "Cooldown");
            rec.uiAggro    = ShnGetU32(*t, _r, "Aggro");
            m_kServerById[rec.uiPupID] = m_kServer.size();
            m_kServer.push_back(rec);
        }
    }
    if (const ShnFile* t = ShnRegistry::Get().GetTable("PupCase")) {
        ITER_ROWS(t) {
            LegacyPupCaseRow rec;
            rec.uiCaseID   = ShnGetU32(*t, _r, "CaseID");
            rec.uiPriority = ShnGetU32(*t, _r, "Priority");
            rec.uiAction   = ShnGetU32(*t, _r, "Action");
            m_kCaseById[rec.uiCaseID] = m_kCase.size();
            m_kCase.push_back(rec);
        }
    }
}
const LegacyPupMainRow*   PupTables::FindMain  (uint32 uiID) const {
    std::map<uint32, size_t>::const_iterator it = m_kMainById.find(uiID);
    return (it == m_kMainById.end()) ? NULL : &m_kMain[it->second]; }
const LegacyPupServerRow* PupTables::FindServer(uint32 uiID) const {
    std::map<uint32, size_t>::const_iterator it = m_kServerById.find(uiID);
    return (it == m_kServerById.end()) ? NULL : &m_kServer[it->second]; }
const LegacyPupCaseRow*   PupTables::FindCase  (uint32 uiID) const {
    std::map<uint32, size_t>::const_iterator it = m_kCaseById.find(uiID);
    return (it == m_kCaseById.end()) ? NULL : &m_kCase[it->second]; }


} // namespace fiesta
