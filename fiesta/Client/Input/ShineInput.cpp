// Client/Input/ShineInput.cpp
#include "ShineInput.h"
#include "../../Server/Shared/ShineLogSystem.h"
#include "../../Server/Common/NETCOMMAND.h"
#include "../../Server/Shared/PacketBuffer.h"
#include "../../Server/Shared/GPacket.h"
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979f
#endif

namespace shine {

const float ShineInput::SEND_INTERVAL = 0.1f;   // 10 position updates/sec
const float ShineInput::RUN_SPEED     = 350.0f;  // units/sec -- tune to match server MoveManager
const float ShineInput::WALK_SPEED    = 150.0f;

ShineInput::ShineInput()
    : m_fFacing(0.0f), m_fCamYaw(0.0f),
      m_eMove(MOVE_STOPPED), m_eLastSentMove(MOVE_STOPPED),
      m_fSendTimer(0.0f), m_fMoveSpeed(RUN_SPEED), m_fWalkSpeed(WALK_SPEED)
{}

ShineInput& ShineInput::Get() {
    static ShineInput s; return s;
}

void ShineInput::SetPosition(float fX, float fY, float fZ) {
    m_kPos = Vec3f(fX, fY, fZ);
    m_kLastSentPos = m_kPos;
}

void ShineInput::OnServerPositionCorrect(float fX, float fY, float fZ) {
    // Server is authoritative — snap to corrected position
    m_kPos = Vec3f(fX, fY, fZ);
}

bool ShineInput::IsKeyDown(int vk) const {
    return (GetAsyncKeyState(vk) & 0x8000) != 0;
}

void ShineInput::Tick(float fDt, ZoneSession* pkZone) {
    // ── Determine move direction from keys ────────────────────────────────────
    float fDX = 0.0f, fDZ = 0.0f;

    if (IsKeyDown('W') || IsKeyDown(VK_UP))    fDZ -= 1.0f; // forward
    if (IsKeyDown('S') || IsKeyDown(VK_DOWN))  fDZ += 1.0f; // backward
    if (IsKeyDown('A') || IsKeyDown(VK_LEFT))  fDX -= 1.0f; // strafe left
    if (IsKeyDown('D') || IsKeyDown(VK_RIGHT)) fDX += 1.0f; // strafe right

    bool bMoving = (fDX != 0.0f || fDZ != 0.0f);
    bool bWalk   = IsKeyDown(VK_SHIFT);

    // ── Rotate movement by camera yaw so WASD is camera-relative ─────────────
    if (bMoving) {
        float fYawRad = m_fCamYaw * (M_PI / 180.0f);
        float fCosY   = cosf(fYawRad);
        float fSinY   = sinf(fYawRad);
        float fRX     = fDX * fCosY - fDZ * fSinY;
        float fRZ     = fDX * fSinY + fDZ * fCosY;
        fDX = fRX; fDZ = fRZ;

        // Normalize diagonal movement
        float fLen = sqrtf(fDX*fDX + fDZ*fDZ);
        if (fLen > 0.001f) { fDX /= fLen; fDZ /= fLen; }

        // Facing angle (degrees from north)
        m_fFacing = atan2f(fDX, -fDZ) * (180.0f / M_PI);
        if (m_fFacing < 0.0f) m_fFacing += 360.0f;
    }

    // ── Update move state ─────────────────────────────────────────────────────
    MoveState ePrev = m_eMove;
    if (!bMoving) {
        m_eMove = MOVE_STOPPED;
    } else {
        m_eMove = bWalk ? MOVE_WALK : MOVE_RUN;
    }

    // ── Dead-reckon position ──────────────────────────────────────────────────
    if (m_eMove != MOVE_STOPPED) {
        float fSpeed = (m_eMove == MOVE_WALK) ? m_fWalkSpeed : m_fMoveSpeed;
        m_kPos.x += fDX * fSpeed * fDt;
        m_kPos.z += fDZ * fSpeed * fDt;
    }

    // ── Send stop packet when we transition to stopped ────────────────────────
    if (ePrev != MOVE_STOPPED && m_eMove == MOVE_STOPPED) {
        if (pkZone) SendStopPacket(pkZone);
        m_eLastSentMove = MOVE_STOPPED;
        return;
    }

    // ── Send position packets at SEND_INTERVAL ────────────────────────────────
    if (m_eMove != MOVE_STOPPED) {
        m_fSendTimer += fDt;
        if (m_fSendTimer >= SEND_INTERVAL) {
            m_fSendTimer = 0.0f;
            if (pkZone) SendMovePacket(pkZone, m_eMove);
            m_kLastSentPos  = m_kPos;
            m_eLastSentMove = m_eMove;
        }
    }
}

void ShineInput::SendMovePacket(ZoneSession* pkZone, MoveState eKind) {
    // NC_ACT_MOVERUN_CMD / NC_ACT_MOVEWALK_CMD: [posX(f)][posY(f)][posZ(f)]
    uint16 uiOp = (eKind == MOVE_WALK)
        ? (uint16)NC_ACT_MOVEWALK_CMD
        : (uint16)NC_ACT_MOVERUN_CMD;

    PacketBuffer body;
    body.WriteF32(m_kPos.x);
    body.WriteF32(m_kPos.y);
    body.WriteF32(m_kPos.z);
    pkZone->Net().SendPacket(uiOp, body.Data(), body.Size());
}

void ShineInput::SendStopPacket(ZoneSession* pkZone) {
    // NC_ACT_STOP_REQ: no body
    pkZone->Net().SendPacket((uint16)NC_ACT_STOP_REQ);
}

} // namespace shine
