// Client/Network/WMSession.h
// WorldManager client state machine.
// Sits between LoginSession and ZoneSession in the connection chain.
//
// Flow (after LoginSession gives us WM IP + port + 16-byte token):
//   Connect → OnConnected()
//     → NC_CHAR_LOGIN_REQ  [accountId(4)][token(16)]
//   ← NC_CHAR_LOGIN_ACK   [ok(1)]
//     → NC_WM_CHARSELECT_REQ [charId(4)]
//   ← NC_WM_ZONE_ASSIGN_CMD [zoneIp(str)][zonePort(2)][zoneToken(16)]
//     → [disconnect WM, fire ZoneHandoff callback]
#ifndef SHINE_CLIENT_NETWORK_WMSESSION_H
#define SHINE_CLIENT_NETWORK_WMSESSION_H

#include "ShineNetClient.h"
#include "ZoneSession.h"

namespace shine {

// VS2010-compatible callback typedefs (no <functional>/std::function).
typedef void (*WMSuccessCallback)(void* pkCtx, const ZoneHandoff& rHandoff);
typedef void (*WMFailCallback)   (void* pkCtx, const char* szReason);

class WMSession : public INetPacketSink {
public:
    WMSession();

    void SetCallbacks(WMSuccessCallback onSuccess, WMFailCallback onFail, void* pkCtx);

    struct WMHandoff {
        std::string kIp;
        uint16      uiPort;
        uint8       aToken[16];
        uint32      uiAccountId;
        uint32      uiCharId;
    };

    bool Connect(const WMHandoff& rHandoff);
    void Disconnect();

    ShineNetClient& Net() { return m_kNet; }

    // INetPacketSink
    virtual void OnConnected()                  override;
    virtual void OnDisconnected()               override;
    virtual void OnPacket(const GPacket& rPkt)  override;

private:
    enum State { WMS_IDLE, WMS_WAIT_AUTH_ACK, WMS_WAIT_ZONE_ASSIGN, WMS_DONE };

    ShineNetClient      m_kNet;
    State               m_eState;
    WMHandoff           m_kHandoff;

    WMSuccessCallback   m_cbSuccess;
    WMFailCallback      m_cbFail;
    void*               m_pkCbCtx;

    void SendAuthReq();
    void SendCharSelectReq();

    void HandleAuthAck      (const GPacket& rPkt);
    void HandleZoneAssign   (const GPacket& rPkt);
};

} // namespace shine
#endif // SHINE_CLIENT_NETWORK_WMSESSION_H
