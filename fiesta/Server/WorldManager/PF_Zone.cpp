// Server/WorldManager/PF_Zone.cpp
// Protocol family: WM <-> Zone. Implements WMZoneSession::OnConnect /
// OnDisconnect / OnPacket. The Zone server connects out to WM at boot,
// announces itself with NC_INTER_ZONE_REGISTER_REQ (zid + listen ip+port),
// and emits NC_INTER_ZONE_HEARTBEAT_CMD every few seconds. WM uses this
// session to fan OPTool pushes back out to the zones.
#include "WorldManagerServer.h"
#include "../Shared/ShineLogSystem.h"
#include "../Common/NETCOMMAND.h"
#include "../Common/SendPacket.h"

namespace fiesta {

void WMZoneSession::OnConnect()    { m_uiZoneId = 0xFFFF; }
void WMZoneSession::OnDisconnect() {
    SHINELOG_INFO("WM: Zone%02u disconnected", m_uiZoneId);
    WorldManagerServer::Get().UnregisterZoneSession(this);
}

void WMZoneSession::OnPacket(const GPacket& rPkt) {
    switch (rPkt.GetOpcode()) {
        case NC_INTER_ZONE_REGISTER_REQ: {
            PacketBuffer b = rPkt.Body();
            uint16 zid = 0; b.ReadU16(zid);
            std::string ip; b.ReadString(ip);
            uint16 port = 0; b.ReadU16(port);
            m_uiZoneId = zid;
            WorldManagerServer::Get().RegisterZone(zid, ip, port);
            WorldManagerServer::Get().RegisterZoneSession(zid, this);
            PacketBuffer ack; ack.WriteU8(1);
            SendPacket(this, NC_INTER_HELLO_ACK, ack.Data(), ack.Size());
            break;
        }
        case NC_INTER_ZONE_HEARTBEAT_CMD:
            WorldManagerServer::Get().TouchZoneHeartbeat(m_uiZoneId);
            break;
        default:
            SHINELOG_DEBUG("WM<-Zone%02u NC=0x%04X", m_uiZoneId, rPkt.GetOpcode());
            break;
    }
}

} // namespace fiesta
