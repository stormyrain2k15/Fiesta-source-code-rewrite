// Server/Zone/Tables/NPCActionTable.cpp
// FEATURE: world-creation -- World/NPCAction.txt binder.
// Per-NPC dialog page: condition rows + action rows. The runtime
// joins (NPCMenu, ConditionID) to decide which dialog options to
// surface for a given (player, npc-context).
#include "../WorldTables.h"
#include "../../DataReader/TableScriptFile.h"
#include "../../Shared/ShineLogSystem.h"

namespace shine {

NPCActionTable& NPCActionTable::Get() { static NPCActionTable s; return s; }

bool NPCActionTable::Load(const std::string& rRoot) {
    m_kConds.clear(); m_kActions.clear();
    m_kByMenu.clear(); m_kCondById.clear();
    TableScriptFile f;
    if (!f.Load(rRoot + "\\World\\NPCAction.txt")) return false;
    // FEATURE: world-creation -- column read: ConditionID, ConditionA,
    // TypeA, AX, AY, ConditionB, TypeB, BX, BY
    if (const TsTable* t = f.Find("NPCCondition")) {
        for (size_t r = 0; r < t->kRecords.size(); ++r) {
            CondRow x;
            x.uiConditionID = (uint8)t->GetInt(r, "ConditionID");
            x.kConditionA   = t->GetStr(r, "ConditionA");
            x.kTypeA        = t->GetStr(r, "TypeA");
            x.uiAX          = (uint32)t->GetInt(r, "AX");
            x.uiAY          = (uint32)t->GetInt(r, "AY");
            x.kConditionB   = t->GetStr(r, "ConditionB");
            x.kTypeB        = t->GetStr(r, "TypeB");
            x.uiBX          = (uint32)t->GetInt(r, "BX");
            x.uiBY          = (uint32)t->GetInt(r, "BY");
            m_kCondById[x.uiConditionID] = m_kConds.size();
            m_kConds.push_back(x);
        }
    }
    // FEATURE: world-creation -- column read: NPCMenu, ConditionID,
    // Action, Arg0, Arg1, ViewInfo
    if (const TsTable* t = f.Find("NPCAction")) {
        for (size_t r = 0; r < t->kRecords.size(); ++r) {
            ActionRow a;
            a.uiNPCMenu     = (uint8)t->GetInt(r, "NPCMenu");
            a.uiConditionID = (uint8)t->GetInt(r, "ConditionID");
            a.kAction       = t->GetStr(r, "Action");
            a.kArg0         = t->GetStr(r, "Arg0");
            a.kArg1         = t->GetStr(r, "Arg1");
            a.uiViewInfoId  = (uint32)t->GetInt(r, "ViewInfo");
            m_kByMenu[a.uiNPCMenu].push_back(m_kActions.size());
            m_kActions.push_back(a);
        }
    }
    SHINELOG_INFO("NPCAction.txt: %u conditions, %u actions",
                  (uint32)m_kConds.size(), (uint32)m_kActions.size());
    return true;
}

void NPCActionTable::ActionsForMenu(uint8 uiNPCMenu,
                                    std::vector<const ActionRow*>& rOut) const {
    rOut.clear();
    std::map<uint8, std::vector<size_t> >::const_iterator it = m_kByMenu.find(uiNPCMenu);
    if (it == m_kByMenu.end()) return;
    rOut.reserve(it->second.size());
    for (size_t i = 0; i < it->second.size(); ++i)
        rOut.push_back(&m_kActions[it->second[i]]);
}

const NPCActionTable::CondRow* NPCActionTable::FindCond(uint8 uiConditionID) const {
    std::map<uint8, size_t>::const_iterator it = m_kCondById.find(uiConditionID);
    return (it == m_kCondById.end()) ? NULL : &m_kConds[it->second];
}

} // namespace shine
