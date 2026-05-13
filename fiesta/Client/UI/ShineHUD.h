// Client/UI/ShineHUD.h
// Minimal heads-up display: HP bar, SP bar, level/class text.
// Renders using Ni2DBuffer / NiScreenElements so it sits on top of 3D.
// Replace with your full UI framework when ready.
#ifndef SHINE_CLIENT_UI_SHINEHUD_H
#define SHINE_CLIENT_UI_SHINEHUD_H

#include <NiMain.h>
#include "../Network/ZoneSession.h"  // PlayerState
#include "../../Server/Shared/ShineTypes.h"

namespace shine {

class ShineHUD {
public:
    ShineHUD();
    ~ShineHUD();

    bool Init(NiRenderer* pkRenderer);
    void Update(const PlayerState& rPlayer);
    void Render(NiRenderer* pkRenderer);
    void Shutdown();

    void SetVisible(bool bVisible) { m_bVisible = bVisible; }
    bool IsVisible() const { return m_bVisible; }

private:
    bool                    m_bVisible;
    NiRenderer*             m_pkRenderer;

    // Screen-space bar geometry
    NiScreenElementsPtr     m_spHPBar;
    NiScreenElementsPtr     m_spSPBar;
    NiScreenElementsPtr     m_spHPBarFill;
    NiScreenElementsPtr     m_spSPBarFill;

    PlayerState             m_kLast;

    void BuildBars();
    void UpdateBar(NiScreenElements* pkFill, int32 iCur, int32 iMax, const NiColorA& rColor);
};

} // namespace shine
#endif // SHINE_CLIENT_UI_SHINEHUD_H
