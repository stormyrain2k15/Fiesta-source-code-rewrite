// Server/Zone/ShineNPCTable.cpp
#include "ShineNPCTable.h"
#include "../DataReader/TableScriptFile.h"
#include "../Shared/ShineLogSystem.h"

namespace fiesta {

ShineNPCTable& ShineNPCTable::Get() { static ShineNPCTable s; return s; }

bool ShineNPCTable::Load(const std::string& rRoot) {
    m_kNPCs.clear(); m_kLinks.clear(); m_kIndex.clear();
    std::string path = rRoot + "\\World\\NPC.txt";
    TableScriptFile f;
    if (!f.Load(path)) return false;

    if (const TsTable* t = f.Find("ShineNPC")) {
        for (size_t r = 0; r < t->kRecords.size(); ++r) {
            ShineNPCRow n;
            n.kMobName  = t->GetStr(r, "MobName");
            n.kMap      = t->GetStr(r, "Map");
            n.iCoordX   = (int32)t->GetInt(r, "Coord-X");
            n.iCoordY   = (int32)t->GetInt(r, "Coord-Y");
            n.iDirect   = (int16)t->GetInt(r, "Direct");
            n.uiNPCMenu = (uint8)t->GetInt(r, "NPCMenu");
            n.kRole     = t->GetStr(r, "Role");
            n.kRoleArg0 = t->GetStr(r, "RoleArg0");
            if (!n.kMobName.empty()) m_kIndex[n.kMobName] = m_kNPCs.size();
            m_kNPCs.push_back(n);
        }
    }
    if (const TsTable* t = f.Find("LinkTable")) {
        for (size_t r = 0; r < t->kRecords.size(); ++r) {
            LinkRow lk;
            lk.kArgument  = t->GetStr(r, "argument");
            lk.kMapServer = t->GetStr(r, "MapServer");
            lk.kMapClient = t->GetStr(r, "MapClient");
            lk.iCoordX    = (int32)t->GetInt(r, "Coord-X");
            lk.iCoordY    = (int32)t->GetInt(r, "Coord-Y");
            lk.iDirect    = (int16)t->GetInt(r, "Direct");
            lk.uiParty    = (uint8)t->GetInt(r, "Party");
            m_kLinks.push_back(lk);
        }
    }
    SHINELOG_INFO("ShineNPCTable: %u NPCs, %u links", (uint32)m_kNPCs.size(), (uint32)m_kLinks.size());
    return true;
}

const ShineNPCRow* ShineNPCTable::FindNPC(const std::string& rMobName) const {
    std::map<std::string, size_t>::const_iterator it = m_kIndex.find(rMobName);
    return (it == m_kIndex.end()) ? NULL : &m_kNPCs[it->second];
}

} // namespace fiesta
