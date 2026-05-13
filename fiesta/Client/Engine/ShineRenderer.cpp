// Client/Engine/ShineRenderer.cpp
#include "ShineRenderer.h"
#include "../../Server/Shared/ShineLogSystem.h"
#include <NiDX9Renderer.h>
#include <d3d9.h>

namespace shine {

NiRenderer* ShineRenderer::Create(HWND hWnd) {
    const MachineOpt::GraphicOption& gfx = MachineOpt::Get().GetGraphicOption();

    SHINELOG_INFO("ShineRenderer: creating DX9 renderer %ux%u bpp=%u fs=%d vsync=%d",
                  gfx.uiDisplayWidth, gfx.uiDisplayHeight,
                  gfx.uiBitsPerPixel, gfx.bFullscreen, gfx.bVSync);

    // Validate requested mode -- fall back to safe defaults if not supported
    uint32 uiW = gfx.uiDisplayWidth;
    uint32 uiH = gfx.uiDisplayHeight;
    uint32 uiBPP = gfx.uiBitsPerPixel;

    if (!IsValidDisplayMode(uiW, uiH, uiBPP)) {
        SHINELOG_WARN("ShineRenderer: %ux%u bpp=%u not supported -- using 1280x720x32",
                      uiW, uiH, uiBPP);
        uiW   = 1280;
        uiH   = 720;
        uiBPP = 32;
        // Update MachineOpt to reflect the actual mode
        MachineOpt::Get().Graphic().uiDisplayWidth  = uiW;
        MachineOpt::Get().Graphic().uiDisplayHeight = uiH;
        MachineOpt::Get().Graphic().uiBitsPerPixel  = uiBPP;
        MachineOpt::Get().SetDirty();
    }

    // Build NiDX9Renderer creation parameters
    NiDX9Renderer::CreationParameters kParams;
    kParams.m_uiWidth           = uiW;
    kParams.m_uiHeight          = uiH;
    kParams.m_eDepthStencilFmt  = NiDX9Renderer::DSFMT_D24S8;
    kParams.m_eFrameBufferFmt   = (uiBPP == 16)
                                    ? NiDX9Renderer::FBFMT_R5G6B5
                                    : NiDX9Renderer::FBFMT_X8R8G8B8;
    kParams.m_ePresentationInterval = gfx.bVSync
                                    ? NiDX9Renderer::PRESENT_INTERVAL_ONE
                                    : NiDX9Renderer::PRESENT_INTERVAL_IMMEDIATE;
    kParams.m_eSwapEffect       = NiDX9Renderer::SWAPEFFECT_DISCARD;
    kParams.m_bFullScreen       = gfx.bFullscreen;
    kParams.m_uiAdapter         = D3DADAPTER_DEFAULT;
    kParams.m_hWnd              = hWnd;
    kParams.m_uiBackBufferCount = 1;
    kParams.m_bRefreshRate      = false; // let driver pick refresh rate

    // Anti-aliasing
    switch (gfx.uiAntiAlias) {
    case 1: kParams.m_eMultiSampleType = D3DMULTISAMPLE_2_SAMPLES; break;
    case 2: kParams.m_eMultiSampleType = D3DMULTISAMPLE_4_SAMPLES; break;
    default:kParams.m_eMultiSampleType = D3DMULTISAMPLE_NONE;      break;
    }

    NiDX9Renderer* pkRenderer = NiDX9Renderer::Create(kParams);
    if (!pkRenderer) {
        // Retry without AA if creation failed
        if (gfx.uiAntiAlias > 0) {
            SHINELOG_WARN("ShineRenderer: creation failed with AA -- retrying without");
            kParams.m_eMultiSampleType = D3DMULTISAMPLE_NONE;
            pkRenderer = NiDX9Renderer::Create(kParams);
        }
    }

    if (!pkRenderer) {
        SHINELOG_ERROR("ShineRenderer: NiDX9Renderer::Create failed");
        return NULL;
    }

    // Set renderer background color
    pkRenderer->SetBackgroundColor(NiColorA(0.0f, 0.0f, 0.0f, 1.0f));

    SHINELOG_INFO("ShineRenderer: DX9 renderer created ok");
    return pkRenderer;
}

bool ShineRenderer::ApplyOptions(NiRenderer* pkRenderer, HWND hWnd) {
    if (!pkRenderer) return false;

    NiDX9Renderer* pkDX9 = NiDynamicCast(NiDX9Renderer, pkRenderer);
    if (!pkDX9) {
        SHINELOG_WARN("ShineRenderer::ApplyOptions: not a DX9 renderer");
        return false;
    }

    const MachineOpt::GraphicOption& gfx = MachineOpt::Get().GetGraphicOption();

    // Recreate the swap chain with new settings
    NiDX9Renderer::CreationParameters kParams;
    kParams.m_uiWidth       = gfx.uiDisplayWidth;
    kParams.m_uiHeight      = gfx.uiDisplayHeight;
    kParams.m_bFullScreen   = gfx.bFullscreen;
    kParams.m_ePresentationInterval = gfx.bVSync
                                    ? NiDX9Renderer::PRESENT_INTERVAL_ONE
                                    : NiDX9Renderer::PRESENT_INTERVAL_IMMEDIATE;
    kParams.m_hWnd          = hWnd;

    bool bOk = pkDX9->Recreate(kParams);
    if (bOk) {
        SHINELOG_INFO("ShineRenderer: applied options %ux%u fs=%d vsync=%d",
                      gfx.uiDisplayWidth, gfx.uiDisplayHeight,
                      gfx.bFullscreen, gfx.bVSync);
        MachineOpt::Get().Save();
    } else {
        SHINELOG_ERROR("ShineRenderer::ApplyOptions: Recreate failed");
    }
    return bOk;
}

bool ShineRenderer::IsValidDisplayMode(uint32 uiW, uint32 uiH, uint32 uiBPP) {
    IDirect3D9* pD3D = Direct3DCreate9(D3D_SDK_VERSION);
    if (!pD3D) return false;

    D3DFORMAT fmt = (uiBPP == 16) ? D3DFMT_R5G6B5 : D3DFMT_X8R8G8B8;
    HRESULT hr = pD3D->CheckDeviceType(
        D3DADAPTER_DEFAULT,
        D3DDEVTYPE_HAL,
        fmt, fmt,
        FALSE); // windowed check first

    if (SUCCEEDED(hr)) {
        // Also check fullscreen mode
        UINT uiModeCount = pD3D->GetAdapterModeCount(D3DADAPTER_DEFAULT, fmt);
        bool bFound = false;
        for (UINT i = 0; i < uiModeCount; ++i) {
            D3DDISPLAYMODE mode;
            pD3D->EnumAdapterModes(D3DADAPTER_DEFAULT, fmt, i, &mode);
            if (mode.Width == uiW && mode.Height == uiH) {
                bFound = true;
                break;
            }
        }
        pD3D->Release();
        return bFound;
    }

    pD3D->Release();
    return false;
}

void ShineRenderer::GetBackBufferSize(NiRenderer* pkRenderer,
                                       uint32& ruiW, uint32& ruiH) {
    if (!pkRenderer) { ruiW = 1280; ruiH = 720; return; }
    ruiW = pkRenderer->GetWidth();
    ruiH = pkRenderer->GetHeight();
}

} // namespace shine
