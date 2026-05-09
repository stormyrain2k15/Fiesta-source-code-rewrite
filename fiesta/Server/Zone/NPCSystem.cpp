// Server/Zone/NPCSystem.cpp
#include "NPCSystem.h"
#include "Inventory.h"

namespace fiesta {

NPCManager& NPCManager::Get() { static NPCManager s; return s; }
void  NPCManager::Register(ShineNPC* p) { if (p) m_kAll[p->m_uiNpcId] = p; }
ShineNPC* NPCManager::Find(uint32 id) {
    std::map<uint32, ShineNPC*>::iterator it = m_kAll.find(id);
    return (it == m_kAll.end()) ? NULL : it->second;
}

void NPCAct::OnClick(ShinePlayer* pk, ShineNPC* pkNpc) {
    if (!pk || !pkNpc) return;
    ServerMenuActor::OpenMenu(pk, pkNpc->m_uiNpcId);
}

void NPCItemList::GetForShop(uint32, std::vector<NPCMenuItem>& r) { r.clear(); } // EV_VERIFY
void ServerMenuActor::OpenMenu(ShinePlayer*, uint32) {}
bool SellItemManager::BuyFromNpc(ShinePlayer* pk, uint32, uint32, uint16) { return pk != NULL; }
void NpcScheduleServer::Tick() {}

} // namespace fiesta
