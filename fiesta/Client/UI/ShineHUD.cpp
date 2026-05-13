// Client/UI/ShineHUD.cpp
#include "ShineHUD.h"
#include "../../Server/Shared/ShineLogSystem.h"
#include <NiScreenElements.h>

namespace shine {

ShineHUD::ShineHUD() : m_bVisible(false), m_pkRenderer(NULL) {
    ZeroMemory(&m_kLast, sizeof(m_kLast));
}

ShineHUD::~ShineHUD() { Shutdown(); }

bool ShineHUD::Init(NiRenderer* pkRenderer) {
    m_pkRenderer = pkRenderer;
    BuildBars();
    SHINELOG_INFO("ShineHUD: initialized");
    return true;
}

void ShineHUD::Shutdown() {
    m_spHPBar     = NULL;
    m_spSPBar     = NULL;
    m_spHPBarFill = NULL;
    m_spSPBarFill = NULL;
    m_pkRenderer  = NULL;
}

void ShineHUD::BuildBars() {
    // HP bar background (dark red) at screen bottom-left
    // NiScreenElements coords are normalized [0,1] x [0,1], origin bottom-left
    m_spHPBar = NiNew NiScreenElements(NiPrimitiveType::PRIMITIVE_QUADS);
    if (m_spHPBar) {
        m_spHPBar->SetRectangle(0, 0.02f, 0.02f, 0.20f, 0.025f);
        m_spHPBar->SetColors(0, NiColorA(0.3f, 0.0f, 0.0f, 0.8f));
        m_spHPBar->UpdateBound();
    }

    // HP fill (bright red)
    m_spHPBarFill = NiNew NiScreenElements(NiPrimitiveType::PRIMITIVE_QUADS);
    if (m_spHPBarFill) {
        m_spHPBarFill->SetRectangle(0, 0.02f, 0.02f, 0.20f, 0.025f);
        m_spHPBarFill->SetColors(0, NiColorA(0.85f, 0.1f, 0.1f, 1.0f));
        m_spHPBarFill->UpdateBound();
    }

    // SP bar background (dark blue) just above HP bar
    m_spSPBar = NiNew NiScreenElements(NiPrimitiveType::PRIMITIVE_QUADS);
    if (m_spSPBar) {
        m_spSPBar->SetRectangle(0, 0.02f, 0.048f, 0.20f, 0.022f);
        m_spSPBar->SetColors(0, NiColorA(0.0f, 0.0f, 0.3f, 0.8f));
        m_spSPBar->UpdateBound();
    }

    // SP fill (bright blue)
    m_spSPBarFill = NiNew NiScreenElements(NiPrimitiveType::PRIMITIVE_QUADS);
    if (m_spSPBarFill) {
        m_spSPBarFill->SetRectangle(0, 0.02f, 0.048f, 0.20f, 0.022f);
        m_spSPBarFill->SetColors(0, NiColorA(0.1f, 0.4f, 0.9f, 1.0f));
        m_spSPBarFill->UpdateBound();
    }
}

void ShineHUD::Update(const PlayerState& rPlayer) {
    m_kLast = rPlayer;

    if (m_spHPBarFill && rPlayer.iMaxHP > 0)
        UpdateBar(m_spHPBarFill, rPlayer.iHP, rPlayer.iMaxHP,
                  NiColorA(0.85f, 0.1f, 0.1f, 1.0f));

    if (m_spSPBarFill && rPlayer.iMaxSP > 0)
        UpdateBar(m_spSPBarFill, rPlayer.iSP, rPlayer.iMaxSP,
                  NiColorA(0.1f, 0.4f, 0.9f, 1.0f));
}

void ShineHUD::UpdateBar(NiScreenElements* pkFill, int32 iCur, int32 iMax,
                          const NiColorA& rColor) {
    float fRatio = (iMax > 0) ? ((float)iCur / (float)iMax) : 0.0f;
    if (fRatio < 0.0f) fRatio = 0.0f;
    if (fRatio > 1.0f) fRatio = 1.0f;

    // Scale the fill width by ratio (full width = 0.20 normalized)
    NiRect<float> kRect;
    pkFill->GetRectangle(0, kRect.m_left, kRect.m_bottom,
                            kRect.m_right, kRect.m_top);
    float fX    = kRect.m_left;
    float fY    = kRect.m_bottom;
    float fFull = 0.20f;
    float fH    = kRect.m_top - kRect.m_bottom;

    pkFill->SetRectangle(0, fX, fY, fFull * fRatio, fH);
    pkFill->SetColors(0, rColor);
    pkFill->UpdateBound();
}

void ShineHUD::Render(NiRenderer* pkRenderer) {
    if (!m_bVisible || !pkRenderer) return;

    // Draw background bars then fills
    if (m_spHPBar)     pkRenderer->RenderScreenElements(m_spHPBar);
    if (m_spHPBarFill) pkRenderer->RenderScreenElements(m_spHPBarFill);
    if (m_spSPBar)     pkRenderer->RenderScreenElements(m_spSPBar);
    if (m_spSPBarFill) pkRenderer->RenderScreenElements(m_spSPBarFill);
}

} // namespace shine
