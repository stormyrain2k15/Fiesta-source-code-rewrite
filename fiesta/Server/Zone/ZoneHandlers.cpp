// Server/Zone/ZoneHandlers.cpp
// Damage formula, PvP scaler, AbState, instance entry, resurrect.
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
#include "DeathReviveSystem.h"
#include "AbState.h"
#include "StatDistribute.h"
#include "Link.h"
#include "TownPortalSystem.h"
#include "LuckyCapsuleSystem.h"
#include "GambleHouse/GambleSystem.h"
#include "GroupTables.h"   // ItemTables::Get().Find

namespace shine {

// ---------------------------------------------------------------------------
// Movement
// ---------------------------------------------------------------------------
static void H_Move(IOCPSession* s, const GPacket& pkt) {
    ClientSession* cs = (ClientSession*)s; if (!cs || !cs->GetPlayer()) return;
    PacketBuffer b = pkt.Body(); float x=0,y=0,z=0;
    b.ReadF32(x); b.ReadF32(y); b.ReadF32(z);
    MoveKind k = (pkt.GetOpcode()==NC_ACT_MOVERUN_CMD) ? MOVE_RUN : MOVE_WALK;
    if (!MoveManager::Move(cs->GetPlayer(), Vec3(x,y,z), k)) cs->Close();
}
static void H_Stop(IOCPSession* s, const GPacket&) {
    ClientSession* cs = (ClientSession*)s;
    if (cs) MoveManager::Stop(cs->GetPlayer());
}

// ---------------------------------------------------------------------------
// Combat — use FindObject so players can attack mobs, not just other players
// ---------------------------------------------------------------------------
static void H_NormalAttack(IOCPSession* s, const GPacket& pkt) {
    ClientSession* cs = (ClientSession*)s; if (!cs || !cs->GetPlayer()) return;
    PacketBuffer b = pkt.Body(); Handle h = 0; b.ReadU32(h);
    ShineObject* pkTgt = ZoneServer::Get().FindObject(h);
    if (!pkTgt) return;
    DamageInfo d = RuleOfEngagement::NormalAttack(cs->GetPlayer(), pkTgt);
    Battle::Apply(cs->GetPlayer(), pkTgt, d);
    // NC_BAT_DAMAGE_CMD: [targetHandle(4), damage(4), isCrit(1), isMiss(1)]
    PacketBuffer ack;
    ack.WriteU32(h);
    ack.WriteI32(d.bMiss ? 0 : d.iPhys);
    ack.WriteU8(d.bCrit ? 1 : 0);
    ack.WriteU8(d.bMiss ? 1 : 0);
    SendPacket(cs, NC_BAT_DAMAGE_CMD, ack.Data(), ack.Size());
    if (pkTgt->IsDead()) {
        PacketBuffer dead; dead.WriteU32(h);
        SendPacket(cs, NC_BAT_DEAD_CMD, dead.Data(), dead.Size());
    }
}

static void H_SkillUse(IOCPSession* s, const GPacket& pkt) {
    ClientSession* cs = (ClientSession*)s; if (!cs || !cs->GetPlayer()) return;
    PacketBuffer b = pkt.Body(); SkillID sid = 0; Handle h = 0;
    b.ReadU32(sid); b.ReadU32(h);
    ShineObject* tgt = (h != 0) ? ZoneServer::Get().FindObject(h) : NULL;
    bool ok = Skill::TryUse(cs->GetPlayer(), sid, tgt);
    PacketBuffer ack; ack.WriteU8(ok ? 1 : 0);
    SendPacket(cs, NC_BAT_SKILL_USE_ACK, ack.Data(), ack.Size());
    if (ok && tgt && tgt->IsDead()) {
        PacketBuffer dead; dead.WriteU32(h);
        SendPacket(cs, NC_BAT_DEAD_CMD, dead.Data(), dead.Size());
    }
}

// ---------------------------------------------------------------------------
// Resurrect
// ---------------------------------------------------------------------------
static void H_Resurrect(IOCPSession* s, const GPacket& pkt) {
    ClientSession* cs = (ClientSession*)s; if (!cs || !cs->GetPlayer()) return;
    ShinePlayer* pk = cs->GetPlayer();
    PacketBuffer b = pkt.Body();
    uint8 choice = 0; b.ReadU8(choice);  // 0=Shine soul, 1=Town, 2=Resurrect-skill
    ShineSoulPouch pouch; // TODO: load from player persistent storage
    eReviveResult r = DeathReviveSystem::Choose(pk, pouch, (eReviveChoice)choice);
    if (r == REV_OK) {
        // NC_BAT_RESURRECT_ACK: [result(1), curHP(4), curSP(4)]
        PacketBuffer ack;
        ack.WriteU8(1);
        ack.WriteI32(pk->GetHP());
        ack.WriteI32(pk->GetSP());
        SendPacket(cs, NC_BAT_RESURRECT_ACK, ack.Data(), ack.Size());
    } else {
        PacketBuffer ack; ack.WriteU8(0);
        SendPacket(cs, NC_BAT_RESURRECT_ACK, ack.Data(), ack.Size());
    }
}

// ---------------------------------------------------------------------------
// Instance dungeon entry
// ---------------------------------------------------------------------------
static void H_InstanceEnter(IOCPSession* s, const GPacket& pkt) {
    ClientSession* cs = (ClientSession*)s; if (!cs || !cs->GetPlayer()) return;
    PacketBuffer b = pkt.Body();
    uint32 typeId = 0; b.ReadU32(typeId);
    ShinePlayer* pk = cs->GetPlayer();
    std::vector<CharID> members;
    members.push_back(pk->GetCharID());
    uint32 instId = MIDServer::Get().SpinUp(typeId, members);
    InstanceDungeon::OnPlayerEnter(pk, typeId);
    // NC_INSTANCE_ACCEPT_ACK: [instanceId(4), typeId(4)]
    // PROVISIONAL shape — verify against capture before treating as final.
    PacketBuffer ack;
    ack.WriteU32(instId);
    ack.WriteU32(typeId);
    SendPacket(cs, NC_INSTANCE_ACCEPT_ACK, ack.Data(), ack.Size());
}

// ---------------------------------------------------------------------------
// Chat
// ---------------------------------------------------------------------------
static void H_Chat(IOCPSession* s, const GPacket& pkt) {
    ClientSession* cs = (ClientSession*)s; if (!cs || !cs->GetPlayer()) return;
    PacketBuffer b = pkt.Body(); std::string txt; b.ReadString(txt);
    Chat::Send(cs->GetPlayer(), CC_NORMAL, txt);
}
static void H_Logout(IOCPSession* s, const GPacket&) { if (s) s->Close(); }

// ---------------------------------------------------------------------------
// NPC menu / shop
// ---------------------------------------------------------------------------
static void H_NpcMenuPick(IOCPSession* s, const GPacket& pkt) {
    ClientSession* cs = (ClientSession*)s; if (!cs || !cs->GetPlayer()) return;
    PacketBuffer b = pkt.Body(); uint32 npcId=0, viewId=0;
    b.ReadU32(npcId); b.ReadU32(viewId);
    ServerMenuActor::HandlePick(cs->GetPlayer(), npcId, viewId);
}
static void H_NpcShopBuy(IOCPSession* s, const GPacket& pkt) {
    ClientSession* cs = (ClientSession*)s; if (!cs || !cs->GetPlayer()) return;
    PacketBuffer b = pkt.Body(); uint32 npcId=0, inx=0; uint16 qty=0;
    b.ReadU32(npcId); b.ReadU32(inx); b.ReadU16(qty);
    ServerMenuActor::HandleBuy(cs->GetPlayer(), npcId, inx, qty);
}
static void H_NpcShopSell(IOCPSession* s, const GPacket& pkt) {
    ClientSession* cs = (ClientSession*)s; if (!cs || !cs->GetPlayer()) return;
    PacketBuffer b = pkt.Body(); uint32 npcId=0; uint16 slot=0, qty=0;
    b.ReadU32(npcId); b.ReadU16(slot); b.ReadU16(qty);
    ServerMenuActor::HandleSell(cs->GetPlayer(), npcId, slot, qty);
}

// ---------------------------------------------------------------------------
// Item upgrade
// ---------------------------------------------------------------------------
static void H_ItemUpgrade(IOCPSession* s, const GPacket& pkt) {
    ClientSession* cs = (ClientSession*)s; if (!cs || !cs->GetPlayer()) return;
    PacketBuffer b = pkt.Body(); uint32 uiItemId=0; uint8 uiLuck=0;
    b.ReadU32(uiItemId); b.ReadU8(uiLuck);
    ItemUpgrade::ResolveForPlayer(cs->GetPlayer(), uiItemId, uiLuck != 0);
}

// ---------------------------------------------------------------------------
// Free-stat allocation
// ---------------------------------------------------------------------------
static void SendFreeStatAck(ClientSession* cs, ShinePlayer* p, NCOpcode op, bool ok) {
    if (!cs || !p) return;
    PacketBuffer ack;
    ack.WriteU8(ok ? 1 : 0);
    ack.WriteU16(p->GetSTR()); ack.WriteU16(p->GetEND());
    ack.WriteU16(p->GetDEX()); ack.WriteU16(p->GetINT());
    ack.WriteU16(p->GetMEN()); ack.WriteU16(p->GetFreeStat());
    SendPacket(cs, op, ack.Data(), ack.Size());
}
static void H_FreeStatDistribute(IOCPSession* s, const GPacket& pkt) {
    ClientSession* cs = (ClientSession*)s; if (!cs || !cs->GetPlayer()) return;
    ShinePlayer* p = cs->GetPlayer();
    PacketBuffer b = pkt.Body(); uint8 which=0; int32 delta=0;
    b.ReadU8(which); b.ReadI32(delta);
    FreeStatLedger kL; p->BuildFreeStatLedger(kL);
    bool ok = FreeStatSystem::Allocate(kL, (uint8)p->GetClass(),
                                        p->GetLevel(), (char)which, delta);
    if (ok) p->ApplyFreeStatLedger(kL);
    SendFreeStatAck(cs, p, NC_BAT_FREESTAT_DISTRIBUTE_ACK, ok);
}
static void H_FreeStatReset(IOCPSession* s, const GPacket&) {
    ClientSession* cs = (ClientSession*)s; if (!cs || !cs->GetPlayer()) return;
    ShinePlayer* p = cs->GetPlayer();
    FreeStatLedger kL; p->BuildFreeStatLedger(kL);
    FreeStatSystem::RefundAll(kL); p->ApplyFreeStatLedger(kL);
    SendFreeStatAck(cs, p, NC_BAT_FREESTAT_RESET_ACK, true);
}

// ---------------------------------------------------------------------------
// Item-use dispatch (capsules, town-portal scrolls, potions, ...)
// ---------------------------------------------------------------------------
// FEATURE: lucky-capsule + portals
static void H_ItemUse(IOCPSession* s, const GPacket& pkt) {
    ClientSession* cs = (ClientSession*)s; if (!cs || !cs->GetPlayer()) return;
    ShinePlayer* p = cs->GetPlayer();
    PacketBuffer b = pkt.Body();
    uint32 uiItemId = 0; uint16 uiSlot = 0;
    b.ReadU32(uiItemId); b.ReadU16(uiSlot);
    // PROVISIONAL_BODY: NA2016 may add a target-handle field for
    // group-use items (party-wide buffs); confirm against client capture.

    // Lookup item info to find the inx-name (used by both routes).
    const ItemInfoRow* pkInfo = ItemTables::Get().FindItem(uiItemId);
    std::string inx = pkInfo ? pkInfo->kInxName : std::string();

    // 1) Lucky capsule? Roll, grant, consume.
    if (LuckyCapsuleSystem::Get().IsCapsule(uiItemId)) {
        if (LuckyCapsuleSystem::Get().TryOpen(p, uiItemId)) {
            p->Inv().Remove(uiItemId);
        }
        return;
    }

    // 2) Town-portal scroll? Send the destination list.
    int32 tpGroup = TownPortalSystem::Get().ResolveItemToGroup(uiItemId, inx);
    if (tpGroup >= 0) {
        std::vector<LegacyTownPortalRow> list;
        TownPortalSystem::Get().BuildList((uint8)tpGroup, (uint8)p->GetLevel(), list);
        // Send the picker list. The scroll is consumed only after the
        // player actually picks a destination (H_TownPortalReq).
        // PROVISIONAL_BODY: NC_MAP_TOWNPORTAL_LIST_ACK is the picker
        // payload; using NC_MAP_TOWNPORTAL_ACK as a placeholder until
        // the dedicated opcode is added to NETCOMMAND.h.
        PacketBuffer out;
        out.WriteU32(uiItemId);                   // echo for picker context
        out.WriteU8((uint8)tpGroup);
        out.WriteU8((uint8)list.size());
        for (size_t i = 0; i < list.size(); ++i) {
            out.WriteU8(list[i].uiIndex);
            out.WriteU8(list[i].uiMinLevel);
            out.WriteU32(list[i].uiX);
            out.WriteU32(list[i].uiY);
            // Map name as a length-prefixed string.
            out.WriteU8((uint8)list[i].kMapName.size());
            for (size_t j = 0; j < list[i].kMapName.size(); ++j)
                out.WriteU8((uint8)list[i].kMapName[j]);
        }
        SendPacket(cs, NC_MAP_TOWNPORTAL_ACK, out.Data(), out.Size());
        return;
    }

    // 3) Other item-use paths (potions, scrolls, etc.) -- existing
    //    item-use function table dispatch lives in ItemSystems. Need
    //    a real ShineItem record for the canonical entry point;
    //    look it up in the inventory by item id.
    const std::vector<ShineItem>& kInv = p->Inv().All();
    for (size_t i = 0; i < kInv.size(); ++i) {
        if (kInv[i].uiItemId != uiItemId) continue;
        ShineItem mut = kInv[i];
        ShineItemUse::TryUse(p, mut);
        break;
    }
}

// ---------------------------------------------------------------------------
// Town-portal pick (player chose a destination from the picker UI)
// ---------------------------------------------------------------------------
// FEATURE: portals
static void H_TownPortalReq(IOCPSession* s, const GPacket& pkt) {
    ClientSession* cs = (ClientSession*)s; if (!cs || !cs->GetPlayer()) return;
    ShinePlayer* p = cs->GetPlayer();
    PacketBuffer b = pkt.Body();
    uint32 uiScrollItemId = 0; uint8 uiGroup = 0; uint8 uiIndex = 0;
    b.ReadU32(uiScrollItemId); b.ReadU8(uiGroup); b.ReadU8(uiIndex);
    if (TownPortalSystem::Get().Teleport(p, uiGroup, uiIndex)) {
        // Consume the scroll on success only.
        p->Inv().Remove(uiScrollItemId);
    }
}

// ---------------------------------------------------------------------------
// Casino: dice & slot
// ---------------------------------------------------------------------------
// FEATURE: casino-dice + casino-slot
static void H_GambleDice(IOCPSession* s, const GPacket& pkt) {
    ClientSession* cs = (ClientSession*)s; if (!cs || !cs->GetPlayer()) return;
    ShinePlayer* p = cs->GetPlayer();
    PacketBuffer b = pkt.Body();
    uint16 uiBowl = 0; uint32 uiBet = 0; uint8 uiTier = 0;
    b.ReadU16(uiBowl); b.ReadU32(uiBet); b.ReadU8(uiTier);
    int32 pay = GambleSystem::Get().ResolveDice(p, uiBowl, uiBet, uiTier);
    PacketBuffer out;
    out.WriteI32(pay);
    SendPacket(cs, NC_GAMBLE_DICE_REQ + 1, out.Data(), out.Size());
}
static void H_GambleSlot(IOCPSession* s, const GPacket& pkt) {
    ClientSession* cs = (ClientSession*)s; if (!cs || !cs->GetPlayer()) return;
    ShinePlayer* p = cs->GetPlayer();
    PacketBuffer b = pkt.Body();
    uint8 inxLen = 0; b.ReadU8(inxLen);
    std::string inx; inx.reserve(inxLen);
    for (uint8 i = 0; i < inxLen; ++i) {
        uint8 c = 0; b.ReadU8(c); inx.push_back((char)c);
    }
    uint32 uiPool = 0; b.ReadU32(uiPool);
    int32 pay = GambleSystem::Get().ResolveSlot(p, inx, uiPool);
    PacketBuffer out;
    out.WriteI32(pay);
    SendPacket(cs, NC_GAMBLE_SLOT_REQ + 1, out.Data(), out.Size());
}

// ---------------------------------------------------------------------------
// Handler registration
// ---------------------------------------------------------------------------
void RegisterZoneHandlers() {
    ProtocolParser& p = GetZoneParser();
    // Core login / session
    p.Register(NC_CHAR_LOGIN_REQ,               &CharLogin);
    p.Register(NC_CHAR_LOGOUT_CMD,              &CharLogout);
    p.Register(NC_USER_LOGOUT_REQ,              &H_Logout);
    // Movement
    p.Register(NC_ACT_MOVERUN_CMD,              &H_Move);
    p.Register(NC_ACT_MOVEWALK_CMD,             &H_Move);
    p.Register(NC_ACT_STOP_CMD,                 &H_Stop);
    // Combat
    p.Register(NC_BAT_NORMALATTACK_CMD,         &H_NormalAttack);
    p.Register(NC_BAT_SKILL_USE_REQ,            &H_SkillUse);
    p.Register(NC_BAT_RESURRECT_REQ,            &H_Resurrect);
    // NPC / shop
    p.Register(NC_NPC_MENU_PICK_REQ,            &H_NpcMenuPick);
    p.Register(NC_NPC_SHOP_BUY_REQ,             &H_NpcShopBuy);
    p.Register(NC_NPC_SHOP_SELL_REQ,            &H_NpcShopSell);
    // Items
    p.Register(NC_ITEM_UPGRADE_REQ,             &H_ItemUpgrade);
    p.Register(NC_ITEM_USE_REQ,                 &H_ItemUse);          // FEATURE: lucky-capsule + portals
    // Stats
    p.Register(NC_BAT_FREESTAT_DISTRIBUTE_REQ,  &H_FreeStatDistribute);
    p.Register(NC_BAT_FREESTAT_RESET_REQ,       &H_FreeStatReset);
    // Instance
    p.Register(NC_INSTANCE_ACCEPT_REQ,          &H_InstanceEnter);
    // Chat
    p.Register(NC_ACT_CHAT_REQ,                 &H_Chat);
    // Travel: town-portal pick
    p.Register(NC_MAP_TOWNPORTAL_REQ,           &H_TownPortalReq);     // FEATURE: portals
    // Casino
    p.Register(NC_GAMBLE_DICE_REQ,              &H_GambleDice);        // FEATURE: casino-dice
    p.Register(NC_GAMBLE_SLOT_REQ,              &H_GambleSlot);        // FEATURE: casino-slot
}

} // namespace shine
