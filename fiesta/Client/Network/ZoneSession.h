// Client/Network/ZoneSession.h
// Zone server state machine.
// Sends NC_CHAR_LOGIN_REQ, receives NC_CHAR_LOGIN_ACK + NC_CHAR_INFO_CMD,
// then handles the ongoing game packet stream.
//
// Flow (after WM hands off zone IP + port + token):
//   Connect → OnConnected()
//     → NC_CHAR_LOGIN_REQ [accountId(4)][charId(4)][token(16)]
//   ← NC_CHAR_LOGIN_ACK  [ok(1)][handle(4)]
//   ← NC_CHAR_INFO_CMD   [handle(4)][level(2)][class(2)][hp(4)][maxhp(4)]
//                         [sp(4)][maxsp(4)][posX(4f)][posY(4f)][posZ(4f)][mapId(2)]
//   ← NC_MAP_WORLDTICK_CMD  (periodic world tick)
//   ← ... game packet stream ...
#ifndef SHINE_CLIENT_NETWORK_ZONESESSION_H
#define SHINE_CLIENT_NETWORK_ZONESESSION_H

#include "ShineNetClient.h"
#include "../../Server/Shared/ShineTypes.h"
#include <string>

namespace shine {

struct ZoneHandoff {
    std::string kIp;
    uint16      uiPort;
    uint8       aToken[16];
    uint32      uiAccountId;
    uint32      uiCharId;
};

struct PlayerState {
    uint32  uiHandle;
    uint16  uiLevel;
    uint16  uiClass;
    int32   iHP;
    int32   iMaxHP;
    int32   iSP;
    int32   iMaxSP;
    float   fPosX;
    float   fPosY;
    float   fPosZ;
    uint16  uiMapId;
};

// VS2010-compatible callback typedefs (no <functional>/std::function).
typedef void (*ZoneReadyCallback) (void* pkCtx, const PlayerState& rPlayer);
typedef void (*ZonePacketCallback)(void* pkCtx, const GPacket& rPkt);
typedef void (*ZoneFailCallback)  (void* pkCtx, const char* szReason);

class ZoneSession : public INetPacketSink {
public:
    ZoneSession();

    void SetCallbacks(ZoneReadyCallback onReady,
                      ZonePacketCallback onPacket,
                      ZoneFailCallback   onFail,
                      void* pkCtx);

    bool Connect(const ZoneHandoff& rHandoff);
    void Disconnect();

    // Send movement update [posX(4f)][posY(4f)][posZ(4f)][dir(2)]
    void SendMove(float fX, float fY, float fZ, uint16 uiDir);

    // Send keepalive tick response
    void SendTickAck(uint32 uiTick);

    bool IsReady() const { return m_eState == ZS_IN_WORLD; }
    const PlayerState& GetPlayer() const { return m_kPlayer; }
    ShineNetClient& Net() { return m_kNet; }

    // INetPacketSink
    virtual void OnConnected()                  override;
    virtual void OnDisconnected()               override;
    virtual void OnPacket(const GPacket& rPkt)  override;

private:
    enum State { ZS_IDLE, ZS_WAIT_LOGIN_ACK, ZS_WAIT_INFO, ZS_IN_WORLD };

    ShineNetClient  m_kNet;
    State           m_eState;
    ZoneHandoff     m_kHandoff;
    PlayerState     m_kPlayer;

    ZoneReadyCallback   m_cbReady;
    ZonePacketCallback  m_cbPacket;
    ZoneFailCallback    m_cbFail;
    void*               m_pkCbCtx;

    void SendLoginReq();
    void HandleLoginAck     (const GPacket& rPkt);
    void HandleCharInfo     (const GPacket& rPkt);
    void HandleCharBase     (const GPacket& rPkt);
    void HandleCharShape    (const GPacket& rPkt);
    void HandleCharNewAck   (const GPacket& rPkt);
    void HandleCharDelAck   (const GPacket& rPkt);
    void HandleWorldTick    (const GPacket& rPkt);
};

} // namespace shine
#endif // SHINE_CLIENT_NETWORK_ZONESESSION_H
