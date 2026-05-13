// Client/UI/UILayout.h
// Persists UI panel positions across sessions.
// Hardcoded defaults are set once at compile time. User can drag panels
// to any position; changes are written to ShineUI.cfg beside the exe.
//
// Save file format (ShineUI.cfg):
//   [PanelName]
//   X=0.020000
//   Y=0.085000
//   W=0.220000
//   H=0.028000
//   Visible=1
//
// On load: reads ShineUI.cfg, overrides defaults for any panel found.
// On first run: no cfg exists, defaults apply, file is created on first save.
// On drag end: file is saved immediately so a crash doesn't lose the layout.
//
// Drag mechanics:
//   Alt + Left-click drag = move any panel
//   Double Alt + Left-click = reset panel to default position
//   All positions normalized [0,1] screen space
#ifndef SHINE_CLIENT_UI_UILAYOUT_H
#define SHINE_CLIENT_UI_UILAYOUT_H

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <string>
#include <map>
#include "../../Server/Shared/ShineTypes.h"

namespace shine {

struct PanelLayout {
    std::string kName;
    // Current position (may be user-adjusted)
    float fX, fY, fW, fH;
    bool  bVisible;
    // Default position (hardcoded, used for reset)
    float fDefX, fDefY, fDefW, fDefH;

    PanelLayout() : fX(0), fY(0), fW(0), fH(0), bVisible(true),
                    fDefX(0), fDefY(0), fDefW(0), fDefH(0) {}

    PanelLayout(const char* szName,
                float fX, float fY, float fW, float fH)
        : kName(szName), fX(fX), fY(fY), fW(fW), fH(fH), bVisible(true),
          fDefX(fX), fDefY(fY), fDefW(fW), fDefH(fH) {}

    void ResetToDefault() { fX=fDefX; fY=fDefY; fW=fDefW; fH=fDefH; }
    bool IsAtDefault() const {
        return fX==fDefX && fY==fDefY && fW==fDefW && fH==fDefH;
    }
};

class UILayout {
public:
    static UILayout& Get();

    // Call at startup with path to save file (e.g. "<exeDir>ShineUI.cfg")
    void Init(const std::string& rCfgPath);

    // Register a panel with its hardcoded default rect.
    // Returns reference to the layout (may be overridden by saved file).
    PanelLayout& Register(const char* szName,
                          float fDefX, float fDefY,
                          float fDefW, float fDefH);

    // Get a registered panel (asserts if not found -- programmer error)
    PanelLayout& GetPanel(const char* szName);

    // Save all panel positions to cfg file immediately
    void Save();

    // Reset all panels to hardcoded defaults and save
    void ResetAll();

    // ── Drag system ───────────────────────────────────────────────────────────
    // Call every frame from ShineUI::Update before rendering.
    // Returns true if any panel was moved (caller should re-layout).
    bool TickDrag(int iMouseX, int iMouseY, bool bAltDown, bool bLButtonDown,
                  int iScreenW, int iScreenH);

    // Returns name of currently dragged panel, or "" if none
    const std::string& GetDragging() const { return m_kDragging; }

    bool IsDirty() const { return m_bDirty; }

private:
    UILayout() : m_bDirty(false), m_bDragging(false),
                 m_iDragOffX(0), m_iDragOffY(0) {}

    std::map<std::string, PanelLayout> m_kPanels;
    std::string m_kCfgPath;
    bool        m_bDirty;

    // Drag state
    std::string m_kDragging;   // name of panel being dragged
    bool        m_bDragActive;
    int         m_iDragOffX;   // pixel offset from panel origin to click point
    int         m_iDragOffY;
    bool        m_bWasLButton; // last frame LButton state

    // Hit-test: which panel contains normalized point (fNX, fNY)?
    // Returns "" if none.
    std::string HitTest(float fNX, float fNY) const;

    void Load();
    void WriteFloat(FILE* f, const char* szKey, float fVal);
    float ReadIniFloat(const char* szSection, const char* szKey,
                       float fDefault) const;
};

} // namespace shine
#endif // SHINE_CLIENT_UI_UILAYOUT_H
