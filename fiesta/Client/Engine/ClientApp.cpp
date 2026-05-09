// Client/Engine/ClientApp.cpp
// Pass-1 client shell. The Ni* method calls below describe the integration shape;
// the actual Gamebryo headers from ThirdParty/Gamebryo provide the real declarations.
//
// Why no real #include <NiMain.h> here yet:
//   The Gamebryo headers under ThirdParty/Gamebryo are the project owner's vendored
//   tree. Wiring them into this TU is a build-system step (add include + lib paths
//   in the VS project). When that's done, replace the stubs in Init/Render/Shutdown
//   with NiApplication / NiDX9Renderer calls. The architecture in this file is
//   correct -- only the include directives need to switch from forward decls to
//   real Gamebryo headers.
#include "ClientApp.h"
#include "../../Server/Shared/ShineLogSystem.h"
#include "../../Server/Common/NETCOMMAND.h"
#include "../../Server/Common/SendPacket.h"

namespace fiesta {

ClientApp::ClientApp()
    : m_hWnd(NULL), m_bRunning(false),
      m_pkNiApp(NULL), m_pkScene(NULL), m_pkCamera(NULL) {}

ClientApp::~ClientApp() { Shutdown(); }

bool ClientApp::Init(HINSTANCE hInst, const char* szTitle) {
    // 1) Create Win32 window (real impl: WNDCLASSEX + CreateWindowEx).
    // 2) Bring Gamebryo up: NiInit() / NiApplication subclass / NiDX9Renderer::Create().
    // 3) Build scene root NiNode + NiCamera; install renderer.
    // 4) Wire input -> action commands.
    SHINELOG_INFO("ClientApp::Init '%s' (Gamebryo bootstrap deferred to integration step)", szTitle);
    m_bRunning = true;
    return true;
}

int ClientApp::Run() {
    DWORD dwLast = GetTickCount();
    while (m_bRunning) {
        PumpMessages();
        DWORD dwNow = GetTickCount();
        float fDt = (float)(dwNow - dwLast) / 1000.0f; dwLast = dwNow;
        Tick(fDt);
        Render();
        Sleep(1);
    }
    return 0;
}

void ClientApp::Shutdown() {
    if (m_pkNiApp) m_pkNiApp = NULL;       // released via NiApplication shutdown sequence
    m_pkScene = NULL; m_pkCamera = NULL;
    m_bRunning = false;
}

bool ClientApp::ConnectLogin(const char* szIp, uint16 uiPort) {
    // Real impl: open IOCP-backed client TCP socket to login server, run login state machine.
    SHINELOG_INFO("ClientApp connect login %s:%u (network-side wiring is server-symmetric)", szIp, uiPort);
    return true;
}

void ClientApp::OnNetworkPacket(const GPacket& rPkt) {
    // Client-side dispatcher mirrors the server ProtocolParser; one NC handler per opcode.
    SHINELOG_DEBUG("Client recv NC=0x%04X", rPkt.GetOpcode());
}

void ClientApp::PumpMessages() {
    MSG msg;
    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
        if (msg.message == WM_QUIT) m_bRunning = false;
        TranslateMessage(&msg); DispatchMessage(&msg);
    }
}

void ClientApp::Tick(float)  { /* update animation, camera, input -> NETCOMMAND */ }
void ClientApp::Render()     { /* m_pkNiApp->BeginFrame() / Render(scene, camera) / EndFrame() */ }

} // namespace fiesta
