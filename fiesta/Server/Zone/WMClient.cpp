// Server/Zone/WMClient.cpp
#include "WMClient.h"
#include "ZoneServer.h"
#include "ShineObject.h"
#include "Inventory.h"
#include "GMEventManager.h"
#include "InterBroadcastSinks.h"
#include "../Shared/PacketBuffer.h"
#include "../Shared/GPacket.h"
#include "../Shared/ShineLogSystem.h"
#include "../Common/NETCOMMAND.h"
#include "../Common/SendPacket.h"

namespace fiesta {

// ---------------------------------------------------------------------------
//  In-process helpers used by the inbound OPTool push handlers.
// ---------------------------------------------------------------------------
namespace {
    ShinePlayer* FindPlayerByCharId(CharID c) {
        const std::map<Handle, ShinePlayer*>& kPlayers = ZoneServer::Get().Players();
        for (std::map<Handle, ShinePlayer*>::const_iterator it = kPlayers.begin();
             it != kPlayers.end(); ++it) {
            if (it->second && it->second->GetCharID() == c) return it->second;
        }
        return NULL;
    }
}

// ---------------------------------------------------------------------------
//  WMClientSession -- inbound dispatcher for everything WM pushes back to
//  this Zone.
// ---------------------------------------------------------------------------
class WMClientSession : public IOCPSession {
public:
    virtual void OnConnect()    { SHINELOG_INFO("Zone->WM connected"); }
    virtual void OnDisconnect() { SHINELOG_WARN("Zone->WM disconnected"); }
    virtual void OnPacket(const GPacket& rPkt) {
        switch (rPkt.GetOpcode()) {
            case NC_INTER_HELLO_ACK:
                SHINELOG_INFO("Zone: WM hello ACK");
                break;
            case NC_INTER_OPTOOL_BAN_PUSH:        OnBan      (rPkt); break;
            case NC_INTER_OPTOOL_KICK_PUSH:       OnKick     (rPkt); break;
            case NC_INTER_OPTOOL_JAIL_PUSH:       OnJail     (rPkt); break;
            case NC_INTER_OPTOOL_UNJAIL_PUSH:     OnUnjail   (rPkt); break;
            case NC_INTER_OPTOOL_SYSMSG_PUSH:     OnSysMsg   (rPkt); break;
            case NC_INTER_OPTOOL_GIVEITEM_PUSH:   OnGiveItem (rPkt); break;
            case NC_INTER_OPTOOL_TAKEITEM_PUSH:   OnTakeItem (rPkt); break;
            case NC_INTER_BROADCAST_CMD:          OnInterBroadcast(rPkt); break;
            default:
                SHINELOG_DEBUG("Zone<-WM NC=0x%04X (no handler)", rPkt.GetOpcode());
                break;
        }
    }
private:
    static void OnBan(const GPacket& rPkt) {
        PacketBuffer b = rPkt.Body();
        uint32 cid = 0, mins = 0; std::string reason;
        b.ReadU32(cid); b.ReadU32(mins); b.ReadString(reason);
        ShinePlayer* pk = FindPlayerByCharId(cid);
        if (!pk) return;  // not on this zone
        SHINELOG_WARN("Zone: BAN cid=%u mins=%u reason='%s'", cid, mins, reason.c_str());
        if (pk->GetSession()) pk->GetSession()->Close();
    }
    static void OnKick(const GPacket& rPkt) {
        PacketBuffer b = rPkt.Body();
        uint32 cid = 0; std::string reason;
        b.ReadU32(cid); b.ReadString(reason);
        ShinePlayer* pk = FindPlayerByCharId(cid);
        if (!pk) return;
        SHINELOG_WARN("Zone: KICK cid=%u reason='%s'", cid, reason.c_str());
        if (pk->GetSession()) pk->GetSession()->Close();
    }
    static void OnJail(const GPacket& rPkt) {
        PacketBuffer b = rPkt.Body();
        uint32 cid = 0, mins = 0; std::string reason;
        b.ReadU32(cid); b.ReadU32(mins); b.ReadString(reason);
        ShinePlayer* pk = FindPlayerByCharId(cid);
        if (!pk) return;
        SHINELOG_WARN("Zone: JAIL cid=%u %u min reason='%s'", cid, mins, reason.c_str());
        // tPrison row insert + map relocate -- the SQLP_Prison facade can be
        // extended in pass 2; the in-memory side is to mark the player and
        // teleport them. The DeathReviveSystem already supports relocate.
        // Provisional: stuff them at the documented Prison map (id=37).
        pk->SetMap((MapID)37);
    }
    static void OnUnjail(const GPacket& rPkt) {
        PacketBuffer b = rPkt.Body();
        uint32 cid = 0; b.ReadU32(cid);
        ShinePlayer* pk = FindPlayerByCharId(cid);
        if (!pk) return;
        SHINELOG_INFO("Zone: UNJAIL cid=%u", cid);
        // Send them home -- map id 1 is the documented spawn shard.
        pk->SetMap((MapID)1);
    }
    static void OnSysMsg(const GPacket& rPkt) {
        PacketBuffer b = rPkt.Body();
        uint8 scope = 0; std::string text;
        b.ReadU8(scope); b.ReadString(text);
        SHINELOG_INFO("Zone: SYSMSG scope=%u '%s'", (uint32)scope, text.c_str());
        // Re-broadcast to every connected client on this zone.
        PacketBuffer chat;
        chat.WriteU8(0xFF);          // SystemBroadcast channel sentinel
        chat.WriteString(text);
        const std::map<Handle, ShinePlayer*>& kPlayers = ZoneServer::Get().Players();
        for (std::map<Handle, ShinePlayer*>::const_iterator it = kPlayers.begin();
             it != kPlayers.end(); ++it) {
            if (it->second && it->second->GetSession()) {
                SendPacket(it->second->GetSession(), NC_ACT_CHAT_CMD,
                           chat.Data(), chat.Size());
            }
        }
    }
    static void OnGiveItem(const GPacket& rPkt) {
        PacketBuffer b = rPkt.Body();
        uint32 cid = 0, item = 0; uint16 cnt = 1;
        b.ReadU32(cid); b.ReadU32(item); b.ReadU16(cnt);
        ShinePlayer* pk = FindPlayerByCharId(cid);
        if (!pk) return;
        SHINELOG_WARN("Zone: GIVEITEM cid=%u item=%u count=%u", cid, item, (uint32)cnt);
        // Inventory grant: pass-2 flow uses SQLP_Item::Create against the
        // CharDB. The in-memory side appends a ShineItem so the client sees
        // it immediately. Slot resolution is left to the inventory class.
        ShineItem kIt; ZeroMemory(&kIt, sizeof(kIt));
        kIt.uiItemId  = item;
        kIt.uiInxName = (ItemID)item;
        kIt.uiQty     = cnt;
        // Per-player inventory lives off the player object in pass 2; for now
        // we log the grant. The Zone-side persistent inventory hook will
        // attach here once added.
        (void)kIt;
    }
    static void OnTakeItem(const GPacket& rPkt) {
        PacketBuffer b = rPkt.Body();
        // Body shape: { uint32 charNo, uint64 itemKey } -- WM resolves
        // charNo from the OPTool request before forwarding. The legacy
        // pre-pass-2 push omitted charNo and shipped the key alone; we
        // accept both shapes by length-checking.
        uint32 cid = 0;
        uint32 lo  = 0, hi = 0;
        size_t avail = b.Remaining();
        if (avail >= 12) {           // new shape {uint32, uint64}
            b.ReadU32(cid);
            b.ReadU32(lo); b.ReadU32(hi);
        } else {                     // legacy shape {uint64}
            b.ReadU32(lo); b.ReadU32(hi);
        }
        uint64 itemKey = ((uint64)hi << 32) | (uint64)lo;
        SHINELOG_WARN("Zone: TAKEITEM cid=%u key=%llu", cid,
                      (unsigned long long)itemKey);

        // In-memory inventory removal. The OPTool path that hits SQL via
        // p_Item_Delete is a separate flow (NC_OPTOOL_QUERY_REQ on the
        // panel side); this branch keeps the connected client's view
        // consistent so they see the item disappear immediately.
        if (cid == 0) return;
        ShinePlayer* pk = FindPlayerByCharId(cid);
        if (!pk) return;

        const std::vector<ShineItem>& vAll = pk->Inv().All();
        for (size_t i = 0; i < vAll.size(); ++i) {
            if (vAll[i].uiDbItemKey == itemKey) {
                pk->Inv().Remove(vAll[i].uiItemId);
                break;
            }
        }
    }
    // ----- inter-zone broadcast (KQ / chat / GM event / daily / NPC) ---
    // Body shape (matches WM senders):
    //   uint8 kind
    //     0 = chat shout       {uint8=0, CharID from, string channel, string text}
    //     1 = world yell       {uint8=1, CharID from, string channel(empty), string text}
    //     2 = GM event         {uint8=2, uint32 eventNo, uint8 action}
    //     3 = daily reset      {uint8=3, uint32 dayKey}
    //     4 = npc schedule     {uint8=4, uint8 spawn, uint32 npcId, uint16 mapId}
    static void OnInterBroadcast(const GPacket& rPkt) {
        PacketBuffer b = rPkt.Body();
        uint8 uiKind = 0; b.ReadU8(uiKind);
        switch (uiKind) {
            case 0:
            case 1: {
                uint32 cid = 0; std::string ch, txt;
                b.ReadU32(cid); b.ReadString(ch); b.ReadString(txt);
                // Re-broadcast to local clients on the matching channel.
                NCOpcode op = (uiKind == 0) ? NC_CHAT_SHOUT_CMD
                                            : NC_CHAT_NORMAL_CMD;
                PacketBuffer chat;
                chat.WriteU32(cid);
                chat.WriteString(txt);
                const std::map<Handle, ShinePlayer*>& kPl = ZoneServer::Get().Players();
                for (std::map<Handle, ShinePlayer*>::const_iterator it = kPl.begin();
                     it != kPl.end(); ++it) {
                    if (it->second && it->second->GetSession())
                        SendPacket(it->second->GetSession(), op,
                                   chat.Data(), chat.Size());
                }
                break;
            }
            case 2: {
                uint32 uiEventNo = 0; uint8 uiAction = 0;
                b.ReadU32(uiEventNo); b.ReadU8(uiAction);
                GMEventManager_Zone::OnEventBroadcast(uiEventNo, uiAction != 0);
                break;
            }
            case 3: {
                uint32 uiDay = 0; b.ReadU32(uiDay);
                SHINELOG_INFO("Zone: DailyReset day=%u", uiDay);
                // Per-system local resets. Each subsystem owns its
                // counter and clears whatever cooldowns / quest-progress
                // state are reset on the daily boundary. Adding a new
                // system: append a call below; the broadcast itself stays
                // a single ack point.
                DailyResetSink::Get().OnDailyReset(uiDay);
                break;
            }
            case 4: {
                uint8 spawn = 0; uint32 npc = 0; uint16 map = 0;
                b.ReadU8(spawn); b.ReadU32(npc); b.ReadU16(map);
                SHINELOG_INFO("Zone: NPC schedule npc=%u map=%u %s",
                              npc, (uint32)map, spawn ? "SPAWN" : "DESPAWN");
                NpcScheduleSink::Get().OnTransition(npc, map, spawn != 0);
                break;
            }
            default:
                SHINELOG_DEBUG("Zone<-WM InterBroadcast kind=%u (not handled)",
                               (uint32)uiKind);
                break;
        }
    }
};

// ---------------------------------------------------------------------------
//  WMClient
// ---------------------------------------------------------------------------
WMClient& WMClient::Get() { static WMClient s; return s; }

bool WMClient::Connect(IOCPManager* pkIOCP, const std::string& rIp, uint16 uiPort,
                       uint16 uiZoneId, const std::string& rZoneIp, uint16 uiZoneClientPort)
{
    if (!m_kConn.Connect(pkIOCP, rIp, uiPort, new WMClientSession())) return false;
    PacketBuffer body;
    body.WriteU16(uiZoneId);
    body.WriteString(rZoneIp);
    body.WriteU16(uiZoneClientPort);
    GPacket kPkt; kPkt.SetOpcode(NC_INTER_ZONE_REGISTER_REQ);
    kPkt.Body().WriteBytes(body.Data(), body.Size());
    return m_kConn.SendPacket(kPkt);
}

void WMClient::Disconnect() { m_kConn.Close(); }

void WMClient::Heartbeat() {
    if (!IsConnected()) return;
    GPacket kPkt; kPkt.SetOpcode(NC_INTER_ZONE_HEARTBEAT_CMD);
    m_kConn.SendPacket(kPkt);
}

bool WMClient::SendGMEventTrigger(uint32 uiEventNo, uint32 uiDurationSec, bool bStart) {
    if (!IsConnected()) return false;
    // PROVISIONAL_BODY: NC_INTER_GMEVENT_TRIGGER_REQ body shape
    // (eventNo, durationSec, action) is internal to this rewrite --
    // there's no original NA2016 inter-server packet for an ad-hoc GM
    // event push. If the build chain ever links this to an existing
    // OPTool admin opcode, the fields here will need to match that
    // opcode's body.
    PacketBuffer body;
    body.WriteU32(uiEventNo);
    body.WriteU32(uiDurationSec);
    body.WriteU8 (bStart ? 1 : 0);
    GPacket kPkt; kPkt.SetOpcode(NC_INTER_GMEVENT_TRIGGER_REQ);
    kPkt.Body().WriteBytes(body.Data(), body.Size());
    return m_kConn.SendPacket(kPkt);
}

} // namespace fiesta
