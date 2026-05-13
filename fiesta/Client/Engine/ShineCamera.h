// Client/Engine/ShineCamera.h
// Third-person chase camera controller wrapping NiCamera.
// Mouse wheel = zoom. Right-drag = orbit. Target = player position.
#ifndef SHINE_CLIENT_ENGINE_SHINECAMERA_H
#define SHINE_CLIENT_ENGINE_SHINECAMERA_H

#include <NiMain.h>
#include "../../Server/Shared/ShineTypes.h"

namespace shine {

class ShineCamera {
public:
    ShineCamera();
    ~ShineCamera();

    bool Init(NiRenderer* pkRenderer);
    void Shutdown();
    void Tick(float fDt);

    void SetTarget(float fX, float fY, float fZ);

    NiCamera*   GetNiCamera() { return m_spCamera; }
    float       GetYaw()      const { return m_fYaw; }

private:
    NiCameraPtr m_spCamera;

    // Orbit state
    float   m_fYaw;    // degrees around Y
    float   m_fPitch;  // degrees, clamped [-80, -10]
    float   m_fDist;   // units from target
    float   m_fTargX, m_fTargY, m_fTargZ;

    // Input tracking
    POINT   m_kLastMouse;
    bool    m_bRightDown;

    void    UpdateTransform();
    void    HandleInput();
};

} // namespace shine
#endif // SHINE_CLIENT_ENGINE_SHINECAMERA_H
