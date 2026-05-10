// Server/Zone/Tables/QuestTables.cpp
// FEATURE: world-creation -- QuestDialog + QuestScript + QuestSpecies.
#include "BindMacros.h"
#include "../MoreTables.h"

namespace fiesta {
QuestTables& QuestTables::Get() { static QuestTables s; return s; }
void QuestTables::Bind() {
    // FEATURE: world-creation -- column read: ID, Dialog, Script, MobGroupName
    if (const ShnFile* t = ShnRegistry::Get().GetTable("QuestDialog"))
        for (size_t r=0;r<t->Rows().size();++r) m_kDialog [ShnGetU32(*t,r,"ID")] = ShnGetStr(*t,r,"Dialog");
    if (const ShnFile* t = ShnRegistry::Get().GetTable("QuestScript"))
        for (size_t r=0;r<t->Rows().size();++r) m_kScript [ShnGetU32(*t,r,"ID")] = ShnGetStr(*t,r,"Script");
    if (const ShnFile* t = ShnRegistry::Get().GetTable("QuestSpecies"))
        for (size_t r=0;r<t->Rows().size();++r) m_kSpecies[ShnGetU32(*t,r,"ID")] = ShnGetStr(*t,r,"MobGroupName");
}
const std::string& QuestTables::Dialog (uint32 u) const { std::map<uint32,std::string>::const_iterator i=m_kDialog.find(u);  return i==m_kDialog.end()?m_kEmpty:i->second; }
const std::string& QuestTables::Script (uint32 u) const { std::map<uint32,std::string>::const_iterator i=m_kScript.find(u);  return i==m_kScript.end()?m_kEmpty:i->second; }
const std::string& QuestTables::Species(uint32 u) const { std::map<uint32,std::string>::const_iterator i=m_kSpecies.find(u); return i==m_kSpecies.end()?m_kEmpty:i->second; }
} // namespace fiesta
