// Client/UI/UILayout.cpp
#include "UILayout.h"
#include "../../Server/Shared/ShineLogSystem.h"
#include <cstdio>
#include <cstring>
#include <cmath>

namespace shine {

UILayout& UILayout::Get() {
    static UILayout s; return s;
}

void UILayout::Init(const std::string& rCfgPath) {
    m_kCfgPath   = rCfgPath;
    m_bDirty     = false;
    m_bDragActive = false;
    m_bWasLButton = false;
    Load(); // overlay saved positions on top of whatever defaults were registered
    SHINELOG_INFO("UILayout: init from '%s' (%u panels)",
                  rCfgPath.c_str(), (uint32)m_kPanels.size());
}

PanelLayout& UILayout::Register(const char* szName,
                                 float fDefX, float fDefY,
                                 float fDefW, float fDefH) {
    PanelLayout& p = m_kPanels[szName];
    p.kName = szName;
    // Only set defaults if this is a new registration (don't clobber a
    // loaded position for a panel that was already in the cfg file)
    if (p.fDefW == 0.0f) {
        p.fDefX = p.fX = fDefX;
        p.fDefY = p.fY = fDefY;
        p.fDefW = p.fW = fDefW;
        p.fDefH = p.fH = fDefH;
        p.bVisible = true;
    } else {
        // Panel was loaded from file -- update defaults but keep loaded pos
        p.fDefX = fDefX; p.fDefY = fDefY;
        p.fDefW = fDefW; p.fDefH = fDefH;
    }
    return p;
}

PanelLayout& UILayout::GetPanel(const char* szName) {
    std::map<std::string,PanelLayout>::iterator it = m_kPanels.find(szName);
    if (it == m_kPanels.end()) {
        // Auto-register with zeros as a safety net
        SHINELOG_WARN("UILayout::GetPanel: '%s' not registered", szName);
        return m_kPanels[szName];
    }
    return it->second;
}

// ── Save / Load ───────────────────────────────────────────────────────────────

void UILayout::Save() {
    FILE* f = NULL;
    fopen_s(&f, m_kCfgPath.c_str(), "w");
    if (!f) {
        SHINELOG_WARN("UILayout::Save: cannot write '%s'", m_kCfgPath.c_str());
        return;
    }

    fprintf(f, "; ShineUI.cfg -- UI panel layout\n");
    fprintf(f, "; Alt+drag to move panels. Delete this file to reset all.\n\n");

    for (std::map<std::string,PanelLayout>::const_iterator it = m_kPanels.begin();
         it != m_kPanels.end(); ++it) {
        const PanelLayout& p = it->second;
        fprintf(f, "[%s]\n", p.kName.c_str());
        fprintf(f, "X=%.6f\n", p.fX);
        fprintf(f, "Y=%.6f\n", p.fY);
        fprintf(f, "W=%.6f\n", p.fW);
        fprintf(f, "H=%.6f\n", p.fH);
        fprintf(f, "Visible=%d\n\n", p.bVisible ? 1 : 0);
    }

    fclose(f);
    m_bDirty = false;
    SHINELOG_INFO("UILayout: saved %u panels to '%s'",
                  (uint32)m_kPanels.size(), m_kCfgPath.c_str());
}

void UILayout::Load() {
    // Check if file exists
    FILE* f = NULL;
    fopen_s(&f, m_kCfgPath.c_str(), "r");
    if (!f) {
        SHINELOG_INFO("UILayout: no save file at '%s' -- using defaults",
                      m_kCfgPath.c_str());
        return;
    }
    fclose(f);

    // For each registered panel, try to load its saved position
    for (std::map<std::string,PanelLayout>::iterator it = m_kPanels.begin();
         it != m_kPanels.end(); ++it) {
        PanelLayout& p = it->second;
        const char* szSec = p.kName.c_str();
        const char* szCfg = m_kCfgPath.c_str();

        float fX = ReadIniFloat(szSec, "X", p.fX);
        float fY = ReadIniFloat(szSec, "Y", p.fY);
        float fW = ReadIniFloat(szSec, "W", p.fW);
        float fH = ReadIniFloat(szSec, "H", p.fH);
        int   iV = GetPrivateProfileIntA(szSec, "Visible", 1, szCfg);

        // Basic sanity: keep panel on screen
        if (fX >= 0.0f && fX <= 1.0f && fY >= 0.0f && fY <= 1.0f &&
            fW > 0.0f   && fW <= 1.0f && fH > 0.0f  && fH <= 1.0f) {
            p.fX = fX; p.fY = fY; p.fW = fW; p.fH = fH;
        }
        p.bVisible = (iV != 0);
    }

    SHINELOG_INFO("UILayout: loaded positions from '%s'", m_kCfgPath.c_str());
}

float UILayout::ReadIniFloat(const char* szSection, const char* szKey,
                              float fDefault) const {
    char buf[64];
    GetPrivateProfileStringA(szSection, szKey, "", buf, sizeof(buf),
                             m_kCfgPath.c_str());
    if (buf[0] == '\0') return fDefault;
    return (float)atof(buf);
}

void UILayout::ResetAll() {
    for (std::map<std::string,PanelLayout>::iterator it = m_kPanels.begin();
         it != m_kPanels.end(); ++it) {
        it->second.ResetToDefault();
    }
    m_bDirty = true;
    Save();
    SHINELOG_INFO("UILayout: all panels reset to defaults");
}

// ── Drag system ───────────────────────────────────────────────────────────────

bool UILayout::TickDrag(int iMouseX, int iMouseY, bool bAltDown, bool bLButtonDown,
                         int iScreenW, int iScreenH) {
    if (iScreenW == 0 || iScreenH == 0) return false;

    bool bMoved = false;

    float fNX = (float)iMouseX / (float)iScreenW;
    float fNY = (float)iMouseY / (float)iScreenH;

    // ── Double-click reset: Alt + double-click on a panel ────────────────────
    // (handled separately via WM_LBUTTONDBLCLK -- not in this tick path)

    // ── Start drag ───────────────────────────────────────────────────────────
    if (bAltDown && bLButtonDown && !m_bWasLButton) {
        // New click -- hit test to find which panel was clicked
        std::string kHit = HitTest(fNX, fNY);
        if (!kHit.empty()) {
            m_kDragging   = kHit;
            m_bDragActive = true;
            PanelLayout& p = m_kPanels[kHit];
            // Offset from panel origin to click point (in pixels)
            m_iDragOffX = iMouseX - (int)(p.fX * iScreenW);
            m_iDragOffY = iMouseY - (int)(p.fY * iScreenH);
            SHINELOG_DEBUG("UILayout: dragging '%s'", kHit.c_str());
        }
    }

    // ── Continue drag ─────────────────────────────────────────────────────────
    if (m_bDragActive && bLButtonDown && bAltDown) {
        PanelLayout& p = m_kPanels[m_kDragging];
        float fNewX = (float)(iMouseX - m_iDragOffX) / (float)iScreenW;
        float fNewY = (float)(iMouseY - m_iDragOffY) / (float)iScreenH;

        // Clamp to screen bounds
        if (fNewX < 0.0f) fNewX = 0.0f;
        if (fNewY < 0.0f) fNewY = 0.0f;
        if (fNewX + p.fW > 1.0f) fNewX = 1.0f - p.fW;
        if (fNewY + p.fH > 1.0f) fNewY = 1.0f - p.fH;

        if (fabsf(fNewX - p.fX) > 0.0001f || fabsf(fNewY - p.fY) > 0.0001f) {
            p.fX = fNewX;
            p.fY = fNewY;
            m_bDirty = true;
            bMoved   = true;
        }
    }

    // ── End drag ──────────────────────────────────────────────────────────────
    if (m_bDragActive && (!bLButtonDown || !bAltDown)) {
        m_bDragActive = false;
        if (m_bDirty) {
            Save(); // save immediately on drag end
        }
        SHINELOG_DEBUG("UILayout: drag end '%s'", m_kDragging.c_str());
        m_kDragging.clear();
    }

    m_bWasLButton = bLButtonDown;
    return bMoved;
}

std::string UILayout::HitTest(float fNX, float fNY) const {
    // Test in reverse order (last registered = on top)
    for (std::map<std::string,PanelLayout>::const_reverse_iterator it =
         m_kPanels.rbegin(); it != m_kPanels.rend(); ++it) {
        const PanelLayout& p = it->second;
        if (!p.bVisible) continue;
        if (fNX >= p.fX && fNX <= p.fX + p.fW &&
            fNY >= p.fY && fNY <= p.fY + p.fH) {
            return p.kName;
        }
    }
    return std::string();
}

} // namespace shine
