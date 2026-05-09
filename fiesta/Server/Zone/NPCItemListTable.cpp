// Server/Zone/NPCItemListTable.cpp
#include "NPCItemListTable.h"
#include "../DataReader/TableScriptFile.h"
#include "../Shared/ShineLogSystem.h"

namespace fiesta {

bool NPCItemListFile::Load(const std::string& rPath) {
    m_kTabs.clear(); m_kIndex.clear();
    TableScriptFile f;
    if (!f.Load(rPath)) return false;
    for (size_t i = 0; i < f.Count(); ++i) {
        const TsTable& t = f.At(i);
        NPCShopTab tab; tab.kTabName = t.kName;
        for (size_t r = 0; r < t.kRecords.size(); ++r) {
            NPCShopRow row;
            row.uiRec = (uint8)t.GetInt(r, "Rec");
            for (int c = 0; c < 6; ++c) {
                char b[12]; sprintf_s(b, sizeof(b), "Column%02d", c);
                std::string v;
                if (t.GetCell(r, b, v)) row.kColumns.push_back(v);
            }
            tab.kRows.push_back(row);
        }
        m_kIndex[tab.kTabName] = m_kTabs.size();
        m_kTabs.push_back(tab);
    }
    return !m_kTabs.empty();
}

const NPCShopTab* NPCItemListFile::FindTab(const std::string& rName) const {
    std::map<std::string,size_t>::const_iterator it = m_kIndex.find(rName);
    return (it == m_kIndex.end()) ? NULL : &m_kTabs[it->second];
}

NPCItemListBox& NPCItemListBox::Get() { static NPCItemListBox s; return s; }

const NPCItemListFile* NPCItemListBox::Load(const std::string& rRoot, const std::string& rNpcKey) {
    std::string path = rRoot + "\\NPCItemList\\" + rNpcKey + ".txt";
    NPCItemListFile* pk = new NPCItemListFile();
    if (!pk->Load(path)) { delete pk; return NULL; }
    m_kAll[rNpcKey] = pk;
    SHINELOG_DEBUG("NPCItemList '%s' tabs=%u", rNpcKey.c_str(), (uint32)pk->Tabs().size());
    return pk;
}
const NPCItemListFile* NPCItemListBox::Find(const std::string& rNpcKey) const {
    std::map<std::string, NPCItemListFile*>::const_iterator it = m_kAll.find(rNpcKey);
    return (it == m_kAll.end()) ? NULL : it->second;
}
void NPCItemListBox::Clear() {
    for (std::map<std::string, NPCItemListFile*>::iterator it = m_kAll.begin(); it != m_kAll.end(); ++it)
        delete it->second;
    m_kAll.clear();
}

} // namespace fiesta
