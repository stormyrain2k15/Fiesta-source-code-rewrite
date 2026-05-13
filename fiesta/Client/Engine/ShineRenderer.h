// Client/Engine/ShineRenderer.h
// Gamebryo renderer setup driven by MachineOpt.
// Handles DX9 renderer creation, device caps checking, resolution apply,
// fullscreen toggle, and VSync.
//
// Called from ShineApp::Initialize() before the framework manager starts.
// Gamebryo creates the renderer internally via NiApplication but we override
// CreateRenderer() to apply our MachineOpt settings rather than the defaults.
//
// The original client used NiDX9Select.h for the renderer dialog --
// we skip that and read settings directly from MachineOpt/ShineOption.cfg.
#ifndef SHINE_CLIENT_ENGINE_SHINERENDERER_H
#define SHINE_CLIENT_ENGINE_SHINERENDERER_H

#include <NiMain.h>
#include <NiDX9Renderer.h>
#include "MachineOpt.h"
#include "../../Server/Shared/ShineTypes.h"

namespace shine {

class ShineRenderer {
public:
    // Create and return a DX9 renderer using current MachineOpt settings.
    // Called by ShineApp::CreateRenderer() which overrides NiApplication's default.
    static NiRenderer* Create(HWND hWnd);

    // Apply changed MachineOpt settings to a running renderer.
    // Call after the player changes options in the settings screen.
    // Handles resolution change and fullscreen toggle without restart where possible.
    static bool ApplyOptions(NiRenderer* pkRenderer, HWND hWnd);

    // Check if the current adapter supports the requested display mode.
    static bool IsValidDisplayMode(uint32 uiW, uint32 uiH, uint32 uiBPP);

    // Get actual back buffer dimensions (may differ from requested if adapter clamped)
    static void GetBackBufferSize(NiRenderer* pkRenderer,
                                  uint32& ruiW, uint32& ruiH);
};

} // namespace shine
#endif // SHINE_CLIENT_ENGINE_SHINERENDERER_H
