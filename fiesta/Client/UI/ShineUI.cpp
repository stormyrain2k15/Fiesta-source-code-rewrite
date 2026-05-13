// Client/UI/ShineUI.cpp
// Loads existing resmenu/resctrl textures, positions driven by UILayout.
// Alt+drag to move any panel. Positions persist to ShineUI.cfg.
#include "ShineUI.h"
#include "../Engine/ShineConfig.h"
#include "../Engine/MachineOpt.h"
#include "../../Server/Shared/ShineLogSystem.h"
#include <NiSourceTexture.h>
#include <NiTexturingProperty.h>
#include <NiAlphaProperty.h>

namespace shine {

ShineUI::ShineUI()
    : m_bVisible(false), m_pkRenderer(NULL),
      m_iScreenW(1280), m_iScreenH(720)
{}

ShineUI::~ShineUI() { Shutdown(); }

bool ShineUI::Init(NiRenderer* pkRenderer, const std::string& rCfgPath) {
    m_pkRenderer = pkRenderer;
    if (pkRenderer) {
        m_iScreenW = (int)pkRenderer->GetWidth();
        m_iScreenH = (int)pkRenderer->GetHeight();
    }

    // Init UILayout -- loads ShineUI.cfg if present, uses defaults otherwise
    UILayout::Get().Init(rCfgPath);

    const ShineConfig& cfg = ShineConfig::Get();

    // ── Register panels (defaults match original Shine layout) ───────────
    // MainBar: full-width strip at very bottom
    SetupPanel(m_kMainBar, "MainBar",
        0.0f,  0.0f,   1.0f,  0.08f,
        cfg.MenuPath("MainBar.dds"),
        NiColorA(0.1f, 0.1f, 0.1f, 0.85f));

    // HP bar frame
    SetupPanel(m_kHPFrame, "HPFrame",
        0.02f, 0.085f, 0.22f, 0.028f,
        cfg.MenuPath("HPBar.dds"),
        NiColorA(0.25f, 0.0f, 0.0f, 0.9f));

    // SP bar frame
    SetupPanel(m_kSPFrame, "SPFrame",
        0.02f, 0.117f, 0.22f, 0.025f,
        cfg.MenuPath("SPBar.dds"),
        NiColorA(0.0f, 0.0f, 0.25f, 0.9f));

    // HP fill -- scales width by ratio
    SetupPanel(m_kHPFill, "HPFill",
        0.02f, 0.085f, 0.22f, 0.028f,
        cfg.CtrlPath("HPFill.dds"),
        NiColorA(0.85f, 0.1f, 0.1f, 1.0f),
        /*bIsFill=*/true);

    // SP fill -- scales width by ratio
    SetupPanel(m_kSPFill, "SPFill",
        0.02f, 0.117f, 0.22f, 0.025f,
        cfg.CtrlPath("SPFill.dds"),
        NiColorA(0.1f, 0.35f, 0.9f, 1.0f),
        /*bIsFill=*/true);

    // MiniMap: top-right corner
    SetupPanel(m_kMiniMap, "MiniMap",
        0.82f, 0.78f,  0.16f, 0.20f,
        cfg.MenuPath("MiniMap.dds"),
        NiColorA(0.05f, 0.05f, 0.05f, 0.75f));

    SHINELOG_INFO("ShineUI: initialized %dx%d", m_iScreenW, m_iScreenH);
    return true;
}

void ShineUI::Shutdown() {
    // Save layout on shutdown so positions survive
    if (UILayout::Get().IsDirty())
        UILayout::Get().Save();
    m_kTexCache.clear();
    m_pkRenderer = NULL;
    SHINELOG_INFO("ShineUI: shutdown");
}

// ── Update ────────────────────────────────────────────────────────────────────

void ShineUI::Update(const PlayerState& rPlayer,
                      int iMouseX, int iMouseY,
                      bool bAltDown, bool bLButtonDown) {
    if (!m_bVisible) return;

    // HP fill
    if (rPlayer.iMaxHP > 0) {
        float fR = (float)rPlayer.iHP / (float)rPlayer.iMaxHP;
        if (fR < 0.0f) fR = 0.0f;
        if (fR > 1.0f) fR = 1.0f;
        UpdateFill(m_kHPFill, fR);
    }

    // SP fill
    if (rPlayer.iMaxSP > 0) {
        float fR = (float)rPlayer.iSP / (float)rPlayer.iMaxSP;
        if (fR < 0.0f) fR = 0.0f;
        if (fR > 1.0f) fR = 1.0f;
        UpdateFill(m_kSPFill, fR);
    }

    // Alt+drag panel positioning
    bool bMoved = UILayout::Get().TickDrag(
        iMouseX, iMouseY, bAltDown, bLButtonDown,
        m_iScreenW, m_iScreenH);

    if (bMoved) RebuildQuads();
}

// ── Render ────────────────────────────────────────────────────────────────────

void ShineUI::Render(NiRenderer* pkRenderer) {
    if (!m_bVisible || !pkRenderer) return;

    RenderPanel(m_kMainBar,  pkRenderer);
    RenderPanel(m_kHPFrame,  pkRenderer);
    RenderPanel(m_kSPFrame,  pkRenderer);
    RenderPanel(m_kHPFill,   pkRenderer);
    RenderPanel(m_kSPFill,   pkRenderer);
    RenderPanel(m_kMiniMap,  pkRenderer);
}

// ── Private -- Panel setup ────────────────────────────────────────────────────

void ShineUI::SetupPanel(UIPanel& rPanel, const char* szKey,
                          float fDefX, float fDefY, float fDefW, float fDefH,
                          const std::string& rTexPath, const NiColorA& rFallback,
                          bool bIsFill) {
    rPanel.szLayoutKey = szKey;
    rPanel.fFill       = 1.0f;
    rPanel.bIsFill     = bIsFill;

    // Register with UILayout -- loads saved position if available
    UILayout::Get().Register(szKey, fDefX, fDefY, fDefW, fDefH);

    // Load texture from resmenu/resctrl
    rPanel.spTexture = LoadTexture(rTexPath);

    // Build initial quad at current (possibly loaded) position
    RebuildQuad(rPanel);

    // Apply texture or fallback flat color
    if (rPanel.spTexture && rPanel.spQuad) {
        NiTexturingProperty* pkProp = NiNew NiTexturingProperty();
        pkProp->SetBaseTexture(rPanel.spTexture);
        pkProp->SetApplyMode(NiTexturingProperty::APPLY_MODULATE);

        // Enable alpha so DDS alpha channel renders correctly
        NiAlphaProperty* pkAlpha = NiNew NiAlphaProperty();
        pkAlpha->SetAlphaBlending(true);
        pkAlpha->SetSrcBlendMode(NiAlphaProperty::ALPHA_SRCALPHA);
        pkAlpha->SetDestBlendMode(NiAlphaProperty::ALPHA_INVSRCALPHA);

        rPanel.spQuad->AttachProperty(pkProp);
        rPanel.spQuad->AttachProperty(pkAlpha);
        rPanel.spQuad->SetColors(0, NiColorA(1.0f, 1.0f, 1.0f, 1.0f));
    } else if (rPanel.spQuad) {
        // No texture -- flat color fallback
        rPanel.spQuad->SetColors(0, rFallback);
    }
}

// ── Private -- Quad rebuild ───────────────────────────────────────────────────

void ShineUI::RebuildQuads() {
    RebuildQuad(m_kMainBar);
    RebuildQuad(m_kHPFrame);
    RebuildQuad(m_kSPFrame);
    RebuildQuad(m_kHPFill);
    RebuildQuad(m_kSPFill);
    RebuildQuad(m_kMiniMap);
}

void ShineUI::RebuildQuad(UIPanel& rPanel) {
    if (!rPanel.szLayoutKey) return;
    const PanelLayout& layout = UILayout::Get().GetPanel(rPanel.szLayoutKey);

    if (!rPanel.spQuad)
        rPanel.spQuad = NiNew NiScreenElements(NiPrimitiveType::PRIMITIVE_QUADS);

    float fW = rPanel.bIsFill ? layout.fW * rPanel.fFill : layout.fW;
    rPanel.spQuad->SetRectangle(0, layout.fX, layout.fY, fW, layout.fH);
    rPanel.spQuad->UpdateBound();
    rPanel.spQuad->UpdateProperties();
}

// ── Private -- Fill update ────────────────────────────────────────────────────

void ShineUI::UpdateFill(UIPanel& rPanel, float fRatio) {
    if (!rPanel.spQuad || !rPanel.bIsFill) return;
    rPanel.fFill = fRatio;
    const PanelLayout& layout = UILayout::Get().GetPanel(rPanel.szLayoutKey);
    rPanel.spQuad->SetRectangle(0, layout.fX, layout.fY,
                                layout.fW * fRatio, layout.fH);
    rPanel.spQuad->UpdateBound();
}

// ── Private -- Render one panel ───────────────────────────────────────────────

void ShineUI::RenderPanel(const UIPanel& rPanel, NiRenderer* pkRenderer) {
    if (!rPanel.spQuad) return;
    if (rPanel.szLayoutKey) {
        const PanelLayout& l = UILayout::Get().GetPanel(rPanel.szLayoutKey);
        if (!l.bVisible) return;
    }
    pkRenderer->RenderScreenElements(rPanel.spQuad);
}

// ── Private -- Texture loader ─────────────────────────────────────────────────

NiTexture* ShineUI::LoadTexture(const std::string& rPath) {
    // Cache check
    std::map<std::string, NiTexturePtr>::iterator it = m_kTexCache.find(rPath);
    if (it != m_kTexCache.end()) return it->second;

    // NiSourceTexture::Create handles DDS, BMP, TGA transparently
    NiSourceTexture* pkTex = NiSourceTexture::Create(rPath.c_str());
    if (!pkTex) {
        SHINELOG_WARN("ShineUI: texture not found '%s' -- fallback color active",
                      rPath.c_str());
        m_kTexCache[rPath] = NULL;
        return NULL;
    }

    m_kTexCache[rPath] = pkTex;
    SHINELOG_DEBUG("ShineUI: loaded '%s'", rPath.c_str());
    return pkTex;
}

} // namespace shine
