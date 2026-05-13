// Client/UI/ShineUI.h
// UI system: loads existing resmenu/resctrl assets, positions driven by
// UILayout (saved to ShineUI.cfg, user-adjustable via Alt+drag).
#ifndef SHINE_CLIENT_UI_SHINEUI_H
#define SHINE_CLIENT_UI_SHINEUI_H

#include <NiMain.h>
#include "UILayout.h"
#include "../Network/ZoneSession.h"
#include "../../Server/Shared/ShineTypes.h"
#include <string>
#include <map>

namespace shine {

struct UIPanel {
    NiScreenElementsPtr spQuad;
    NiTexturePtr        spTexture;
    const char*         szLayoutKey; // key into UILayout
    float               fFill;
    bool                bIsFill;     // true = width scales by fFill ratio

    UIPanel() : szLayoutKey(NULL), fFill(1.0f), bIsFill(false) {}
};

class ShineUI {
public:
    ShineUI();
    ~ShineUI();

    bool Init(NiRenderer* pkRenderer, const std::string& rCfgPath);
    void Shutdown();

    void Update(const PlayerState& rPlayer, int iMouseX, int iMouseY,
                bool bAltDown, bool bLButtonDown);
    void Render(NiRenderer* pkRenderer);

    void SetVisible(bool b) { m_bVisible = b; }
    bool IsVisible()  const { return m_bVisible; }

    // Reset all panel positions to defaults and save
    void ResetLayout() { UILayout::Get().ResetAll(); RebuildQuads(); }

private:
    bool        m_bVisible;
    NiRenderer* m_pkRenderer;
    int         m_iScreenW, m_iScreenH;

    UIPanel     m_kMainBar;
    UIPanel     m_kHPFrame;
    UIPanel     m_kSPFrame;
    UIPanel     m_kHPFill;
    UIPanel     m_kSPFill;
    UIPanel     m_kMiniMap;

    std::map<std::string, NiTexturePtr> m_kTexCache;

    // Register a panel with UILayout and build its quad
    void SetupPanel(UIPanel& rPanel, const char* szKey,
                    float fDefX, float fDefY, float fDefW, float fDefH,
                    const std::string& rTexPath, const NiColorA& rFallback,
                    bool bIsFill = false);

    // Rebuild all quads from current UILayout positions (called after drag)
    void RebuildQuads();
    void RebuildQuad(UIPanel& rPanel);

    void UpdateFill(UIPanel& rPanel, float fRatio);
    void RenderPanel(const UIPanel& rPanel, NiRenderer* pkRenderer);

    NiTexture* LoadTexture(const std::string& rPath);
};

} // namespace shine
#endif // SHINE_CLIENT_UI_SHINEUI_H
