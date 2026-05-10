// Client/Engine/ShineCamera.cpp
#include "ShineCamera.h"
#include "../../Server/Shared/ShineLogSystem.h"
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979f
#endif

namespace fiesta {

ShineCamera::ShineCamera()
    : m_fYaw(0.0f), m_fPitch(-45.0f), m_fDist(600.0f),
      m_fTargX(0.0f), m_fTargY(0.0f), m_fTargZ(0.0f),
      m_bRightDown(false)
{
    ZeroMemory(&m_kLastMouse, sizeof(m_kLastMouse));
}

ShineCamera::~ShineCamera() { Shutdown(); }

bool ShineCamera::Init(NiRenderer* pkRenderer) {
    m_spCamera = NiNew NiCamera();
    m_spCamera->SetName("ShineCamera");

    // Frustum -- 60 degree FOV, near/far appropriate for a zone-scale world.
    NiFrustum kFrustum;
    float fAspect = pkRenderer
        ? (float)pkRenderer->GetWidth() / (float)pkRenderer->GetHeight()
        : 4.0f / 3.0f;
    kFrustum.SetFOV(NI_PI / 3.0f, fAspect);
    kFrustum.m_fNear = 10.0f;
    kFrustum.m_fFar  = 20000.0f;
    m_spCamera->SetViewFrustum(kFrustum);

    NiRect<float> kPort(0.0f, 1.0f, 1.0f, 0.0f);
    m_spCamera->SetViewPort(kPort);

    UpdateTransform();
    SHINELOG_INFO("ShineCamera: initialized");
    return true;
}

void ShineCamera::Shutdown() {
    m_spCamera = NULL;
}

void ShineCamera::SetTarget(float fX, float fY, float fZ) {
    m_fTargX = fX;
    m_fTargY = fY;
    m_fTargZ = fZ;
    UpdateTransform();
}

void ShineCamera::Tick(float fDt) {
    HandleInput();
    UpdateTransform();
}

void ShineCamera::HandleInput() {
    // Mouse wheel zoom
    // (ShineInput will post WM_MOUSEWHEEL; for now poll via GetAsyncKeyState)
    // RIGHT drag = orbit
    bool bRight = (GetAsyncKeyState(VK_RBUTTON) & 0x8000) != 0;
    POINT kMouse;
    GetCursorPos(&kMouse);

    if (bRight && m_bRightDown) {
        float fDX = (float)(kMouse.x - m_kLastMouse.x);
        float fDY = (float)(kMouse.y - m_kLastMouse.y);
        m_fYaw   += fDX * 0.4f;
        m_fPitch += fDY * 0.3f;
        if (m_fPitch < -80.0f) m_fPitch = -80.0f;
        if (m_fPitch > -10.0f) m_fPitch = -10.0f;
    }
    m_bRightDown  = bRight;
    m_kLastMouse  = kMouse;

    // Keyboard zoom (+ / -)
    if (GetAsyncKeyState(VK_OEM_PLUS)  & 0x8000) m_fDist -= 10.0f;
    if (GetAsyncKeyState(VK_OEM_MINUS) & 0x8000) m_fDist += 10.0f;
    if (m_fDist < 100.0f)  m_fDist = 100.0f;
    if (m_fDist > 3000.0f) m_fDist = 3000.0f;
}

void ShineCamera::UpdateTransform() {
    if (!m_spCamera) return;

    float fYawRad   = m_fYaw   * (M_PI / 180.0f);
    float fPitchRad = m_fPitch * (M_PI / 180.0f);

    // Spherical → Cartesian offset from target
    float fCosP = cosf(fPitchRad);
    float fSinP = sinf(fPitchRad);
    float fCosY = cosf(fYawRad);
    float fSinY = sinf(fYawRad);

    float fOffX = m_fDist * fCosP * fSinY;
    float fOffY = m_fDist * (-fSinP);     // NiMain Y = up
    float fOffZ = m_fDist * fCosP * fCosY;

    NiPoint3 kPos(m_fTargX + fOffX, m_fTargY + fOffY, m_fTargZ + fOffZ);
    NiPoint3 kTarg(m_fTargX, m_fTargY, m_fTargZ);
    NiPoint3 kUp(0.0f, 1.0f, 0.0f);

    m_spCamera->LookAt(kPos, kTarg, kUp);
    m_spCamera->Update(0.0f);
}

} // namespace fiesta
