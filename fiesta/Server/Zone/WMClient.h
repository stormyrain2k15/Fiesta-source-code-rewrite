// Server/Zone/WMClient.h
// 06 -- Zone-side outbound link to the WorldManager exe.
//
// On Zone boot:
//   - Connect to WM:WM.ZonePort
//   - Send NC_INTER_ZONE_REGISTER_REQ { zoneId, zoneIp, clientPort }
//   - Receive NC_INTER_HELLO_ACK
//   - Periodic NC_INTER_ZONE_HEARTBEAT_CMD via Zone's tick loop
//
// Inbound from WM:
//   - NC_INTER_OPTOOL_*_PUSH (ban/kick/jail/unjail/sysmsg/giveitem/takeitem)
//     -- routed through WMClient to the in-process Zone helpers.
#ifndef FIESTA_ZONE_WMCLIENT_H
#define FIESTA_ZONE_WMCLIENT_H
#include "../Shared/Socket_Connector.h"
#include "../Shared/IOCPManager.h"

namespace fiesta {

class WMClient {
public:
    static WMClient& Get();

    bool Connect(IOCPManager* pkIOCP, const std::string& rIp, uint16 uiPort,
                 uint16 uiZoneId, const std::string& rZoneIp, uint16 uiZoneClientPort);
    void Disconnect();
    void Heartbeat();

    bool IsConnected() const { return m_kConn.IsConnected(); }
private:
    WMClient() {}
    Socket_Connector m_kConn;
};

} // namespace fiesta
#endif
