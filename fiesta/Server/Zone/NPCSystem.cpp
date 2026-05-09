// Server/Zone/NPCSystem.cpp
#include "NPCSystem.h"
#include "Inventory.h"
#include "NPCItemListTable.h"
#include "GroupTables.h"
#include "ExtendedTables.h"
#include "../Shared/ShineLogSystem.h"
#include <windows.h>

namespace fiesta {

NPCManager& NPCManager::Get() { static NPCManager s; return s; }

void NPCManager::Register(ShineNPC* p) { if (p) m_kAll[p->m_uiNpcId] = p; }
void NPCManager::RegisterKey(uint32 id, const std::string& rN) { m_kKey[id] = rN; }
ShineNPC* NPCManager::Find(uint32 id) {
    std::map<uint32, ShineNPC*>::iterator it = m_kAll.find(id);
    return (it == m_kAll.end()) ? NULL : it->second;
}
const std::string& NPCManager::KeyOf(uint32 id) const {
    std::map<uint32, std::string>::const_iterator it = m_kKey.find(id);
    return (it == m_kKey.end()) ? m_kEmpty : it->second;
}

void NPCAct::OnClick(ShinePlayer* pk, ShineNPC* pkNpc) {
    if (!pk || !pkNpc) return;
    ServerMenuActor::OpenMenu(pk, pkNpc->m_uiNpcId);
}

// Walks the "TabXX" tables in `NPCItemList/<MobName>.txt`, resolves each
// 33-char ItemIndex via ItemTables, and projects (uiInxName, BuyPrice)
// for the merchant UI. NPCItemListBox is keyed by the mob-name registered
// at Field-spawn time.
void NPCItemList::GetForShop(uint32 uiNpcId, std::vector<NPCMenuItem>& rOut) {
    rOut.clear();
    const std::string& rKey = NPCManager::Get().KeyOf(uiNpcId);
    if (rKey.empty()) return;
    const NPCItemListFile* pkF = NPCItemListBox::Get().Find(rKey);
    if (!pkF) return;
    const std::vector<NPCShopTab>& tabs = pkF->Tabs();
    for (size_t t = 0; t < tabs.size(); ++t) {
        const NPCShopTab& tab = tabs[t];
        for (size_t r = 0; r < tab.kRows.size(); ++r) {
            const NPCShopRow& row = tab.kRows[r];
            for (size_t c = 0; c < row.kColumns.size(); ++c) {
                const std::string& s = row.kColumns[c];
                if (s.empty() || s == "-") continue;
                const ItemInfoRow* pkI = ItemTables::Get().FindByInx(s);
                if (!pkI) continue;
                NPCMenuItem mi;
                mi.uiInxName = pkI->uiID;
                mi.iPrice    = (int64)pkI->uiBuyPrice;
                rOut.push_back(mi);
            }
        }
    }
}

void ServerMenuActor::OpenMenu(ShinePlayer* pk, uint32 uiNpcId) {
    if (!pk) return;
    // The actual menu packet is composed by the per-role handler. Here we
    // simply log the open so the server-side trace shows the route.
    SHINELOG_DEBUG("MenuOpen char=%u npc=%u key=%s", (uint32)pk->GetCharID(),
                   uiNpcId, NPCManager::Get().KeyOf(uiNpcId).c_str());
}

bool SellItemManager::BuyFromNpc(ShinePlayer* pk, uint32 uiNpcId, uint32 uiInx, uint16 uiQty) {
    if (!pk || uiQty == 0) return false;
    // Confirm the NPC actually offers this item.
    std::vector<NPCMenuItem> kList;
    NPCItemList::GetForShop(uiNpcId, kList);
    bool ok = false; int64 iPrice = 0;
    for (size_t i = 0; i < kList.size(); ++i) {
        if (kList[i].uiInxName == uiInx) { ok = true; iPrice = kList[i].iPrice; break; }
    }
    if (!ok) return false;
    int64 iTotal = iPrice * (int64)uiQty;
    if ((int64)pk->GetMoney() < iTotal) return false;
    pk->AddMoney(-iTotal);
    // Inventory insert is owned by the inventory subsystem; the NPC menu
    // dispatches the resolved (uiInxName, qty) pair into it via the
    // existing CMSG_BUY_FROM_NPC handler. We've already debited the gold
    // and validated the SKU here.
    return true;
}

void NpcScheduleServer::Tick() {
    // The NpcScheduleTable in ExtendedTables.h is hour-of-week granular.
    // We recompute the visibility set every minute by sampling the local
    // time once and pulling each registered NPC against the table.
    SYSTEMTIME st; GetLocalTime(&st);
    uint32 hourOfWeek = (uint32)st.wDayOfWeek * 24u + (uint32)st.wHour;
    (void)hourOfWeek;     // Visibility update is owned by the per-Field tick;
                          // this stub establishes the cadence hook.
}

} // namespace fiesta
