// Server/Zone/WMClient.cpp
#include "WMClient.h"
#include "ZoneServer.h"
#include "ShineObject.h"
#include "Inventory.h"
#include "GMEventManager.h"
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
        uint32 lo = 0, hi = 0; b.ReadU32(lo); b.ReadU32(hi);
        uint64 itemKey = ((uint64)hi << 32) | (uint64)lo;
        SHINELOG_WARN("Zone: TAKEITEM key=%llu", (unsigned long long)itemKey);
        // Inventory removal by item-key requires an index; pass 2 hooks this
        // up against the per-player Inventory. The SQL row deletion is done
        // by the panel via NC_OPTOOL_QUERY_REQ in the meantime.
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

} // namespace fiesta

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

} // namespace fiesta
