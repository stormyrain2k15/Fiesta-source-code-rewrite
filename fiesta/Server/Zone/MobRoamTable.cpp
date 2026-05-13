// Server/Zone/MobRoamTable.cpp
#include "MobRoamTable.h"
#include "../DataReader/TableScriptFile.h"
#include "../Shared/ShineLogSystem.h"

namespace shine {

bool MobRoamMap::Load(const std::string& rPath) {
    m_kPoints.clear();
    TableScriptFile f;
    if (!f.Load(rPath)) return false;
    const TsTable* t = f.Find("Roaming");
    if (!t) return false;
    for (size_t r = 0; r < t->kRecords.size(); ++r) {
        MobRoamPoint p;
        p.uiID         = (uint16)t->GetInt(r, "ID");
        p.uiX          = (uint32)t->GetInt(r, "X");
        p.uiY          = (uint32)t->GetInt(r, "Y");
        p.kEventIndex  = t->GetStr(r, "EventIndex");
        m_kPoints.push_back(p);
    }
    return !m_kPoints.empty();
}

const MobRoamPoint* MobRoamMap::Next(uint16 uiCurID) const {
    if (m_kPoints.empty()) return NULL;
    // Linear scan -- patrol paths are small (typically <= 64 waypoints).
    for (size_t i = 0; i < m_kPoints.size(); ++i) {
        if (m_kPoints[i].uiID == uiCurID) {
            size_t n = (i + 1) % m_kPoints.size();
            return &m_kPoints[n];
        }
    }
    return &m_kPoints[0];
}

MobRoamBox& MobRoamBox::Get() { static MobRoamBox s; return s; }

const MobRoamMap* MobRoamBox::Load(const std::string& rRoot, const std::string& rMapID) {
    std::string p = rRoot + "\\MobRoam\\" + rMapID + ".txt";
    MobRoamMap* pk = new MobRoamMap();
    if (!pk->Load(p)) { delete pk; return NULL; }
    m_kAll[rMapID] = pk;
    SHINELOG_DEBUG("MobRoam '%s' points=%u", rMapID.c_str(), (uint32)pk->Points().size());
    return pk;
}
const MobRoamMap* MobRoamBox::Find(const std::string& rMapID) const {
    std::map<std::string, MobRoamMap*>::const_iterator it = m_kAll.find(rMapID);
    return (it == m_kAll.end()) ? NULL : it->second;
}
void MobRoamBox::Clear() {
    for (std::map<std::string, MobRoamMap*>::iterator it = m_kAll.begin(); it != m_kAll.end(); ++it)
        delete it->second;
    m_kAll.clear();
}

} // namespace shine
