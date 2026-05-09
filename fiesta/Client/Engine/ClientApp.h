// Client/Engine/ClientApp.h
// client app: brings Gamebryo up, owns the render loop, dispatches network.
// Gamebryo (Ni*) is consumed AS-IS from /app/fiesta/ThirdParty/Gamebryo. No
// modifications to engine source. Include paths (set in your VS project):
//   ThirdParty/Gamebryo/NiMain          -> NiMain.h
//   ThirdParty/Gamebryo/NiSystem        -> NiSystem.h
//   ThirdParty/Gamebryo/NiDX9Renderer   -> NiDX9Renderer.h
//   ThirdParty/Gamebryo/NiAnimation, NiCollision, NiParticle, NiPortal, NiAudio, NiFloodgate
#ifndef FIESTA_CLIENT_CLIENTAPP_H
#define FIESTA_CLIENT_CLIENTAPP_H
#include "../../Server/Shared/ShineTypes.h"
#include "../../Server/Shared/Socket_Acceptor.h"

// Forward-declare Gamebryo types we touch. Real headers provided by ThirdParty/Gamebryo.
class NiApplication;
class NiCamera;
class NiNode;

namespace fiesta {

class ClientApp {
public:
    ClientApp();
    ~ClientApp();

    bool   Init(HINSTANCE hInst, const char* szWindowTitle);
    int    Run();
    void   Shutdown();

    bool   ConnectLogin(const char* szIp, uint16 uiPort);
    void   OnNetworkPacket(const GPacket& rPkt);
private:
    HWND   m_hWnd;
    bool   m_bRunning;
    NiApplication* m_pkNiApp;
    NiNode*        m_pkScene;
    NiCamera*      m_pkCamera;

    void   Tick(float fDt);
    void   Render();
    void   PumpMessages();
};

} // namespace fiesta
#endif
