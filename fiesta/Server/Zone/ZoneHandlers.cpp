// Server/Zone/ZoneHandlers.cpp
// wire NETCOMMAND opcodes to their handler functions for the Zone parser.
#include "ZoneServer.h"
#include "../Common/ProtocolParser.h"
#include "../Common/NETCOMMAND.h"
#include "../Common/SendPacket.h"
#include "CharLogin.h"
#include "MoveManager.h"
#include "Battle.h"
#include "SkillSystem.h"
#include "Inventory.h"
#include "ItemSystems.h"
#include "ItemUpgrade.h"
#include "FreeStatSystem.h"
#include "NPCSystem.h"
#include "QuestSystem.h"
#include "Party.h"
#include "GuildSystem.h"
#include "MarketSystems.h"
#include "SocialSystems.h"
#include "CraftAndPet.h"
#include "AdminSecLog.h"
#include "InstanceDungeon.h"
#include "KingdomQuest.h"

namespace fiesta {

static void H_Move(IOCPSession* s, const GPacket& pkt) {
    ClientSession* cs = (ClientSession*)s; if (!cs || !cs->GetPlayer()) return;
    PacketBuffer b = pkt.Body(); float x=0,y=0,z=0; b.ReadF32(x); b.ReadF32(y); b.ReadF32(z);
    MoveKind k = (pkt.GetOpcode() == NC_ACT_MOVERUN_CMD) ? MOVE_RUN : MOVE_WALK;
    if (!MoveManager::Move(cs->GetPlayer(), Vec3(x,y,z), k)) cs->Close();
}
static void H_Stop(IOCPSession* s, const GPacket&) {
    ClientSession* cs = (ClientSession*)s; if (cs) MoveManager::Stop(cs->GetPlayer());
}
static void H_NormalAttack(IOCPSession* s, const GPacket& pkt) {
    ClientSession* cs = (ClientSession*)s; if (!cs || !cs->GetPlayer()) return;
    PacketBuffer b = pkt.Body(); Handle h=0; b.ReadU32(h);
    std::map<Handle, ShinePlayer*>::const_iterator it = ZoneServer::Get().Players().find(h);
    if (it == ZoneServer::Get().Players().end()) return;
    DamageInfo d = RuleOfEngagement::NormalAttack(cs->GetPlayer(), it->second);
    Battle::Apply(cs->GetPlayer(), it->second, d);
    PacketBuffer ack; ack.WriteU32(h); ack.WriteI32(d.iPhys); ack.WriteU8(d.bCrit?1:0);
    SendPacket(cs, NC_BAT_DAMAGE_CMD, ack.Data(), ack.Size());
}
static void H_SkillUse(IOCPSession* s, const GPacket& pkt) {
    ClientSession* cs = (ClientSession*)s; if (!cs || !cs->GetPlayer()) return;
    PacketBuffer b = pkt.Body(); SkillID sid=0; Handle h=0; b.ReadU32(sid); b.ReadU32(h);
    std::map<Handle, ShinePlayer*>::const_iterator it = ZoneServer::Get().Players().find(h);
    ShineObject* tgt = (it == ZoneServer::Get().Players().end()) ? NULL : it->second;
    bool ok = Skill::TryUse(cs->GetPlayer(), sid, tgt);
    PacketBuffer ack; ack.WriteU8(ok?1:0); SendPacket(cs, NC_BAT_SKILL_USE_ACK, ack.Data(), ack.Size());
}
static void H_Chat(IOCPSession* s, const GPacket& pkt) {
    ClientSession* cs = (ClientSession*)s; if (!cs || !cs->GetPlayer()) return;
    PacketBuffer b = pkt.Body(); std::string txt; b.ReadString(txt);
    Chat::Send(cs->GetPlayer(), CC_NORMAL, txt);
}
static void H_Logout(IOCPSession* s, const GPacket&) { if (s) s->Close(); }

// ---- NPC menu / shop ------------------------------------------------------
// Inbound flow:
//   NC_NPC_MENU_PICK_REQ  : [ uint32 npcId, uint32 viewInfoId ]
//                           viewInfoId == 0 -> open root menu.
//   NC_NPC_SHOP_BUY_REQ   : [ uint32 npcId, uint32 inxName, uint16 qty ]
//   NC_NPC_SHOP_SELL_REQ  : [ uint32 npcId, uint16 invSlot, uint16 qty ]
static void H_NpcMenuPick(IOCPSession* s, const GPacket& pkt) {
    ClientSession* cs = (ClientSession*)s; if (!cs || !cs->GetPlayer()) return;
    PacketBuffer b = pkt.Body();
    uint32 npcId = 0, viewId = 0;
    b.ReadU32(npcId); b.ReadU32(viewId);
    ServerMenuActor::HandlePick(cs->GetPlayer(), npcId, viewId);
}
static void H_NpcShopBuy(IOCPSession* s, const GPacket& pkt) {
    ClientSession* cs = (ClientSession*)s; if (!cs || !cs->GetPlayer()) return;
    PacketBuffer b = pkt.Body();
    uint32 npcId = 0, inx = 0; uint16 qty = 0;
    b.ReadU32(npcId); b.ReadU32(inx); b.ReadU16(qty);
    ServerMenuActor::HandleBuy(cs->GetPlayer(), npcId, inx, qty);
}
static void H_NpcShopSell(IOCPSession* s, const GPacket& pkt) {
    ClientSession* cs = (ClientSession*)s; if (!cs || !cs->GetPlayer()) return;
    PacketBuffer b = pkt.Body();
    uint32 npcId = 0; uint16 slot = 0, qty = 0;
    b.ReadU32(npcId); b.ReadU16(slot); b.ReadU16(qty);
    ServerMenuActor::HandleSell(cs->GetPlayer(), npcId, slot, qty);
}

// ---- Item upgrade --------------------------------------------------------
// Inbound: NC_ITEM_UPGRADE_REQ  [ uint32 itemId, uint8 useLuckStone ]
// Outbound: NC_ITEM_UPGRADE_ACK [ uint8 result, uint32 itemId, uint16 newEnchant ]
//   result codes: 0=OK 1=FAIL 2=DOWNGRADE 3=DESTROY 4=BLOCKED.
static void H_ItemUpgrade(IOCPSession* s, const GPacket& pkt) {
    ClientSession* cs = (ClientSession*)s; if (!cs || !cs->GetPlayer()) return;
    PacketBuffer b = pkt.Body();
    uint32 uiItemId = 0; uint8 uiLuck = 0;
    b.ReadU32(uiItemId); b.ReadU8(uiLuck);
    ItemUpgrade::ResolveForPlayer(cs->GetPlayer(), uiItemId, uiLuck != 0);
}

// ---- Free-stat allocation -------------------------------------------------
// Inbound: NC_BAT_FREESTAT_DISTRIBUTE_REQ [ uint8 which, int32 delta ]
//   `which` is the ASCII letter S/E/D/I/M selecting STR/END/DEX/INT/MEN.
// Outbound: NC_BAT_FREESTAT_DISTRIBUTE_ACK
//   [ uint8 ok, uint16 STR, uint16 END, uint16 DEX, uint16 INT, uint16 MEN,
//     uint16 unspentPoints ]
static void SendFreeStatAck(ClientSession* cs, ShinePlayer* p, NCOpcode op, bool bOk) {
    if (!cs || !p) return;
    PacketBuffer ack;
    ack.WriteU8(bOk ? 1 : 0);
    ack.WriteU16(p->GetSTR());
    ack.WriteU16(p->GetEND());
    ack.WriteU16(p->GetDEX());
    ack.WriteU16(p->GetINT());
    ack.WriteU16(p->GetMEN());
    ack.WriteU16(p->GetFreeStat());
    SendPacket(cs, op, ack.Data(), ack.Size());
}

static void H_FreeStatDistribute(IOCPSession* s, const GPacket& pkt) {
    ClientSession* cs = (ClientSession*)s; if (!cs || !cs->GetPlayer()) return;
    ShinePlayer* p = cs->GetPlayer();
    PacketBuffer b = pkt.Body();
    uint8 uiWhich = 0; int32 nDelta = 0;
    b.ReadU8(uiWhich); b.ReadI32(nDelta);

    FreeStatLedger kLedger;
    p->BuildFreeStatLedger(kLedger);
    bool bOk = FreeStatSystem::Allocate(kLedger, (uint8)p->GetClass(),
                                         p->GetLevel(), (char)uiWhich, nDelta);
    if (bOk) p->ApplyFreeStatLedger(kLedger);
    SendFreeStatAck(cs, p, NC_BAT_FREESTAT_DISTRIBUTE_ACK, bOk);
}

static void H_FreeStatReset(IOCPSession* s, const GPacket&) {
    ClientSession* cs = (ClientSession*)s; if (!cs || !cs->GetPlayer()) return;
    ShinePlayer* p = cs->GetPlayer();
    FreeStatLedger kLedger;
    p->BuildFreeStatLedger(kLedger);
    FreeStatSystem::RefundAll(kLedger);
    p->ApplyFreeStatLedger(kLedger);
    SendFreeStatAck(cs, p, NC_BAT_FREESTAT_RESET_ACK, true);
}

void RegisterZoneHandlers() {
    ProtocolParser& p = GetZoneParser();
    p.Register(NC_CHAR_LOGIN_REQ,           &CharLogin);
    p.Register(NC_CHAR_LOGOUT_CMD,          &CharLogout);
    p.Register(NC_ACT_MOVERUN_CMD,          &H_Move);
    p.Register(NC_ACT_MOVEWALK_CMD,         &H_Move);
    p.Register(NC_ACT_STOP_CMD,             &H_Stop);
    p.Register(NC_BAT_NORMALATTACK_CMD,     &H_NormalAttack);
    p.Register(NC_BAT_SKILL_USE_REQ,        &H_SkillUse);
    p.Register(NC_ACT_CHAT_REQ,             &H_Chat);
    p.Register(NC_USER_LOGOUT_REQ,          &H_Logout);
    p.Register(NC_NPC_MENU_PICK_REQ,        &H_NpcMenuPick);
    p.Register(NC_NPC_SHOP_BUY_REQ,         &H_NpcShopBuy);
    p.Register(NC_NPC_SHOP_SELL_REQ,        &H_NpcShopSell);
    p.Register(NC_ITEM_UPGRADE_REQ,         &H_ItemUpgrade);
    p.Register(NC_BAT_FREESTAT_DISTRIBUTE_REQ, &H_FreeStatDistribute);
    p.Register(NC_BAT_FREESTAT_RESET_REQ,   &H_FreeStatReset);
}

} // namespace fiesta
