// Server/Zone/NPCSystem.h
// NPC / menus / shops.
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
    void RegisterKey(uint32 uiNpcId, const std::string& rMobName);
    void RegisterMenu(uint32 uiNpcId, uint8 uiMenu, const std::string& rRole,
                      const std::string& rRoleArg);
    ShineNPC* Find(uint32 uiNpcId);
    const std::string& KeyOf(uint32 uiNpcId) const;
    uint8              MenuOf(uint32 uiNpcId) const;
    const std::string& RoleOf(uint32 uiNpcId) const;
    const std::string& RoleArgOf(uint32 uiNpcId) const;
private:
    std::map<uint32, ShineNPC*>    m_kAll;
    std::map<uint32, std::string>  m_kKey;
    std::map<uint32, uint8>        m_kMenu;
    std::map<uint32, std::string>  m_kRole;
    std::map<uint32, std::string>  m_kRoleArg;
    std::string                    m_kEmpty;
};

class NPCAct                : public NPCAction { public: virtual void OnClick(ShinePlayer*, ShineNPC*); };
class NPCItemList           {
public:
    // Resolve the NPC's mob-name via NPCManager and walk every "TabXX" in
    // the matching `NPCItemList/<MobName>.txt` to enumerate offered items.
    // Each shop entry is mapped through ItemTables / ItemExtraTables for
    // pricing (BuyPrice from ItemInfo; Microbill price from ItemShop).
    static void GetForShop(uint32 uiNpcId, std::vector<NPCMenuItem>& rOut);
};
class CurrentMenu           { public: uint32 uiOpenNpcId; };

// ServerMenuActor -- the *server-authoritative* drawer for the NPC click box.
//   OpenMenu        : top-level click. Renders the page-N NPCAction rows for
//                     this NPC as buttons. Sends NC_NPC_MENU_OPEN_CMD.
//   SendDialog      : a chained dialog page (DialogID -> ButtonsFor in
//                     NpcDialogData / NPCViewInfo). Sends NC_NPC_MENU_OPEN_CMD
//                     with the chained text + view-info rows as buttons.
//   OpenShop        : projects NPCItemList(<npc>) into NC_NPC_SHOP_OPEN_CMD.
//   HandlePick      : the inbound dispatcher for NC_NPC_MENU_PICK_REQ.
//                     Resolves the ViewRow, reads its ActionTag, and routes
//                     into Talk / Trade / Quest / Promote / Save / Mover.
//   HandleBuy       : NC_NPC_SHOP_BUY_REQ  -> validates + inventory insert.
//   HandleSell      : NC_NPC_SHOP_SELL_REQ -> validates + inventory remove.
class ServerMenuActor {
public:
    static void OpenMenu  (ShinePlayer* pk, uint32 uiNpcId);
    static void SendDialog(ShinePlayer* pk, uint32 uiNpcId, uint32 uiDialogId);
    static void OpenShop  (ShinePlayer* pk, uint32 uiNpcId);
    static void HandlePick(ShinePlayer* pk, uint32 uiNpcId, uint32 uiViewInfoId);
    static void HandleBuy (ShinePlayer* pk, uint32 uiNpcId, uint32 uiInxName, uint16 uiQty);
    static void HandleSell(ShinePlayer* pk, uint32 uiNpcId, uint16 uiInvSlot, uint16 uiQty);
private:
    static void SendPickAck(ShinePlayer* pk, uint32 uiNpcId, uint32 uiViewInfoId,
                            uint8 uiResult, const std::string& rMsg);
};

class SellItemManager       { public: static bool BuyFromNpc(ShinePlayer* pk, uint32 uiNpcId, uint32 uiInx, uint16 uiQty); };
class NpcScheduleServer     { public: static void Tick(); };

} // namespace fiesta
#endif
