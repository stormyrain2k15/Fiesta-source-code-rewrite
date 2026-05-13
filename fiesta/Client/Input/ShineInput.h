// Client/Input/ShineInput.h
// Keyboard and mouse input manager.
// Polls GetAsyncKeyState each frame, fires movement packets to ZoneSession,
// and drives camera controls.
//
// Movement:
//   W/UpArrow    = run forward
//   S/DownArrow  = run backward
//   A/LeftArrow  = strafe left
//   D/RightArrow = strafe right
//   Shift        = walk (held while moving)
//   Space        = stop
//
// Movement packets:
//   Running → NC_ACT_MOVERUN_CMD  [posX(f)][posY(f)][posZ(f)]
//   Walking → NC_ACT_MOVEWALK_CMD [posX(f)][posY(f)][posZ(f)]
//   Stop    → NC_ACT_STOP_REQ
//
// Position is dead-reckoned client-side between server updates.
// Server is authoritative — corrects position via NC_MAP_MOVE_CMD.
#ifndef SHINE_CLIENT_INPUT_SHINEINPUT_H
#define SHINE_CLIENT_INPUT_SHINEINPUT_H

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "../../Server/Shared/ShineTypes.h"
#include "../Network/ZoneSession.h"

namespace shine {

// Simple 3-component vector (matches server Vec3 but standalone for client)
struct Vec3f {
    float x, y, z;
    Vec3f() : x(0), y(0), z(0) {}
    Vec3f(float x, float y, float z) : x(x), y(y), z(z) {}
    Vec3f operator+(const Vec3f& o) const { return Vec3f(x+o.x, y+o.y, z+o.z); }
    Vec3f operator*(float s) const { return Vec3f(x*s, y*s, z*s); }
};

enum MoveState { MOVE_STOPPED, MOVE_WALK, MOVE_RUN };

class ShineInput {
public:
    static ShineInput& Get();

    // Call once per frame. dt = delta time in seconds.
    // pkZone = active zone session to send move packets to (NULL = no network).
    void Tick(float fDt, ZoneSession* pkZone);

    // Called by ZoneSession when NC_MAP_MOVE_CMD arrives (server correction)
    void OnServerPositionCorrect(float fX, float fY, float fZ);

    // Set initial position from NC_CHAR_INFO_CMD
    void SetPosition(float fX, float fY, float fZ);

    // Set camera yaw so movement is relative to where we're looking
    void SetCameraYaw(float fYawDeg) { m_fCamYaw = fYawDeg; }

    const Vec3f&  GetPosition()  const { return m_kPos; }
    MoveState     GetMoveState() const { return m_eMove; }
    float         GetFacing()    const { return m_fFacing; }

private:
    ShineInput();

    Vec3f       m_kPos;
    float       m_fFacing;      // degrees, 0=north
    float       m_fCamYaw;      // camera yaw for directional movement
    MoveState   m_eMove;
    MoveState   m_eLastSentMove;
    Vec3f       m_kLastSentPos;
    float       m_fSendTimer;   // accumulates time between position sends
    float       m_fMoveSpeed;   // units/sec run speed
    float       m_fWalkSpeed;   // units/sec walk speed

    static const float SEND_INTERVAL;  // seconds between position packets
    static const float RUN_SPEED;
    static const float WALK_SPEED;

    bool IsKeyDown(int vk) const;
    void SendMovePacket(ZoneSession* pkZone, MoveState eKind);
    void SendStopPacket(ZoneSession* pkZone);
};

} // namespace shine
#endif // SHINE_CLIENT_INPUT_SHINEINPUT_H
