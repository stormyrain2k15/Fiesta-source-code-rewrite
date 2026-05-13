// Server/Zone/NPCItemListTable.h
// typed loader for NPCItemList/<NpcKey>.txt. Each file declares many
// "TabXX" tables (one per UI tab) of merchant items. Each tab row holds up
// to 6 item-index columns (Column00..Column05) where '-' means empty slot.
#ifndef SHINE_ZONE_NPCITEMLISTTABLE_H
#define SHINE_ZONE_NPCITEMLISTTABLE_H
#include "../Shared/ShineTypes.h"
#include <map>
#include <string>
#include <vector>

namespace shine {

struct NPCShopRow {
    uint8                    uiRec;
    std::vector<std::string> kColumns;     // up to 6
};
struct NPCShopTab {
    std::string             kTabName;       // "Tab00".."TabNN"
    std::vector<NPCShopRow> kRows;
};
class NPCItemListFile {
public:
    bool Load(const std::string& rPath);
    const std::vector<NPCShopTab>& Tabs() const { return m_kTabs; }
    const NPCShopTab* FindTab(const std::string& rName) const;
private:
    std::vector<NPCShopTab>      m_kTabs;
    std::map<std::string,size_t> m_kIndex;
};

class NPCItemListBox {
public:
    static NPCItemListBox& Get();
    const NPCItemListFile* Load(const std::string& rRoot, const std::string& rNpcKey);
    const NPCItemListFile* Find(const std::string& rNpcKey) const;
    void Clear();
private:
    std::map<std::string, NPCItemListFile*> m_kAll;
};

} // namespace shine
#endif
