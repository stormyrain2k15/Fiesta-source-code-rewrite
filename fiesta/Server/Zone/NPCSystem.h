// Server/Zone/NPCSystem.h
// 19 -- NPC / menus / shops.
// EVIDENCE: PDB_CONFIRMED  symbol: NPCManager, NPCAct, NPCAction, NPCItemList,
//                                  CurrentMenu, ServerMenuActor, SellItemManager, NpcScheduleServer
#ifndef FIESTA_ZONE_NPCSYSTEM_H
#define FIESTA_ZONE_NPCSYSTEM_H
#include "ShineObject.h"
#include <map>
#include <vector>

namespace fiesta {

struct NPCMenuItem { uint32 uiInxName; int64 iPrice; };

class NPCAction {
public:
    virtual ~NPCAction() {}
    virtual void OnClick(ShinePlayer* pk, ShineNPC* pkNpc) = 0;
};

class NPCManager {
public:
    static NPCManager& Get();
    void Register(ShineNPC* pkNpc);
    ShineNPC* Find(uint32 uiNpcId);
private:
    std::map<uint32, ShineNPC*> m_kAll;
};

class NPCAct                : public NPCAction { public: virtual void OnClick(ShinePlayer*, ShineNPC*); };
class NPCItemList           { public: static void GetForShop(uint32 uiNpcId, std::vector<NPCMenuItem>& rOut); };
class CurrentMenu           { public: uint32 uiOpenNpcId; };
class ServerMenuActor       { public: static void OpenMenu(ShinePlayer* pk, uint32 uiNpcId); };
class SellItemManager       { public: static bool BuyFromNpc(ShinePlayer* pk, uint32 uiNpcId, uint32 uiInx, uint16 uiQty); };
class NpcScheduleServer     { public: static void Tick(); };

} // namespace fiesta
#endif
