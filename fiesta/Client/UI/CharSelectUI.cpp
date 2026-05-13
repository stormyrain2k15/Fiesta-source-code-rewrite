// Client/UI/CharSelectUI.cpp
#include "CharSelectUI.h"
#include "../Framework/CharSelectFrameWork.h"
#include "../Engine/ShineConfig.h"
#include "../../Server/Shared/ShineLogSystem.h"
#include <NiSourceTexture.h>
#include <NiTexturingProperty.h>
#include <NiAlphaProperty.h>
#include <cstdio>

namespace shine {

CharSelectUI::CharSelectUI()
    : m_bInitialized(false), m_eMode(MODE_SELECT),
      m_pkRenderer(NULL), m_uiCreateClass(0),
      m_uiCreateHair(0), m_uiCreateHairColor(0), m_uiCreateFace(0),
      m_fNoticeTimer(0.0f), m_iSelectedSlot(-1),
      m_iMouseX(0), m_iMouseY(0), m_iScreenW(1280), m_iScreenH(720)
{
    for (int i = 0; i < MAX_SLOTS; ++i) {
        m_aSlots[i].bHover    = false;
        m_aSlots[i].bSelected = false;
        m_aSlots[i].uiCharId  = 0;
    }
}

CharSelectUI::~CharSelectUI() { Shutdown(); }

bool CharSelectUI::Init(NiRenderer* pkRenderer, const std::vector<CharSlot>& rSlots) {
    m_pkRenderer = pkRenderer;
    if (pkRenderer) {
        m_iScreenW = (int)pkRenderer->GetWidth();
        m_iScreenH = (int)pkRenderer->GetHeight();
    }

    // Build slot panels
    for (int i = 0; i < MAX_SLOTS; ++i) {
        const CharSlot* pkSlot = (i < (int)rSlots.size()) ? &rSlots[i] : NULL;
        BuildSlotPanel(m_aSlots[i], i, pkSlot);
    }

    BuildButtons();
    m_bInitialized = true;
    SHINELOG_INFO("CharSelectUI: initialized");
    return true;
}

void CharSelectUI::Shutdown() {
    for (int i = 0; i < MAX_SLOTS; ++i) {
        m_aSlots[i].spBg        = NULL;
        m_aSlots[i].spHighlight = NULL;
    }
    m_spBtnEnter  = NULL;
    m_spBtnCreate = NULL;
    m_spBtnDelete = NULL;
    m_spNoticePanel = NULL;
    m_kTexCache.clear();
    m_bInitialized = false;
}

void CharSelectUI::Update(float fDt) {
    if (m_fNoticeTimer > 0.0f) {
        m_fNoticeTimer -= fDt;
        if (m_fNoticeTimer < 0.0f) m_fNoticeTimer = 0.0f;
    }
}

void CharSelectUI::Render(NiRenderer* pkRenderer) {
    if (!m_bInitialized || !pkRenderer) return;

    // Render slot panels
    for (int i = 0; i < MAX_SLOTS; ++i) {
        RenderSlot(m_aSlots[i], pkRenderer);
    }

    // Render buttons based on mode and selection
    if (m_eMode == MODE_SELECT) {
        if (m_iSelectedSlot >= 0 && m_aSlots[m_iSelectedSlot].uiCharId != 0) {
            RenderButton(m_spBtnEnter,  pkRenderer);
            RenderButton(m_spBtnDelete, pkRenderer);
        }
        RenderButton(m_spBtnCreate, pkRenderer);
    }

    // Notice overlay
    if (m_fNoticeTimer > 0.0f && m_spNoticePanel)
        pkRenderer->RenderScreenElements(m_spNoticePanel);
}

void CharSelectUI::Refresh(const std::vector<CharSlot>& rSlots) {
    for (int i = 0; i < MAX_SLOTS; ++i) {
        const CharSlot* pkSlot = (i < (int)rSlots.size()) ? &rSlots[i] : NULL;
        BuildSlotPanel(m_aSlots[i], i, pkSlot);
    }
}

void CharSelectUI::ShowNotice(const char* szMsg) {
    m_kNoticeMsg   = szMsg;
    m_fNoticeTimer = 3.0f; // show for 3 seconds
    SHINELOG_INFO("CharSelectUI notice: %s", szMsg);
}

void CharSelectUI::OnMouseMove(int iX, int iY) {
    m_iMouseX = iX; m_iMouseY = iY;
    // Update hover states for slot panels
    float fNX = (float)iX / (float)m_iScreenW;
    float fNY = (float)iY / (float)m_iScreenH;
    for (int i = 0; i < MAX_SLOTS; ++i) {
        // Slot panel layout: 3 panels evenly spaced horizontally, centered
        float fSlotX = 0.15f + i * 0.28f;
        float fSlotY = 0.25f;
        float fSlotW = 0.22f;
        float fSlotH = 0.45f;
        m_aSlots[i].bHover = HitTestRect(fSlotX, fSlotY, fSlotW, fSlotH, iX, iY);
    }
}

void CharSelectUI::OnLButtonDown(int iX, int iY) {
    float fNX = (float)iX / (float)m_iScreenW;
    float fNY = (float)iY / (float)m_iScreenH;

    if (m_eMode == MODE_SELECT) {
        // Check slot clicks
        for (int i = 0; i < MAX_SLOTS; ++i) {
            float fSlotX = 0.15f + i * 0.28f;
            if (HitTestRect(fSlotX, 0.25f, 0.22f, 0.45f, iX, iY)) {
                m_iSelectedSlot = i;
                for (int j = 0; j < MAX_SLOTS; ++j)
                    m_aSlots[j].bSelected = (j == i);
                return;
            }
        }
        // Enter button
        if (m_iSelectedSlot >= 0 &&
            HitTestRect(0.42f, 0.78f, 0.16f, 0.06f, iX, iY) &&
            m_cbSelect) {
            m_cbSelect(m_pkCbCtx, m_aSlots[m_iSelectedSlot].uiCharId);
        }
        // Create button
        if (HitTestRect(0.62f, 0.78f, 0.16f, 0.06f, iX, iY)) {
            m_eMode = MODE_CREATE;
            m_kCreateName.clear();
            m_uiCreateClass = 0;
        }
        // Delete button
        if (m_iSelectedSlot >= 0 &&
            HitTestRect(0.22f, 0.78f, 0.16f, 0.06f, iX, iY) &&
            m_aSlots[m_iSelectedSlot].uiCharId != 0) {
            m_eMode = MODE_DELETE_CONFIRM;
        }
    } else if (m_eMode == MODE_CREATE) {
        // Confirm create: Enter button
        if (HitTestRect(0.42f, 0.78f, 0.16f, 0.06f, iX, iY) &&
            !m_kCreateName.empty() && m_cbCreate) {
            m_cbCreate(m_pkCbCtx, m_kCreateName, m_uiCreateClass,
                       m_uiCreateHair, m_uiCreateHairColor, m_uiCreateFace);
            m_eMode = MODE_SELECT;
        }
        // Cancel
        if (HitTestRect(0.62f, 0.78f, 0.16f, 0.06f, iX, iY))
            m_eMode = MODE_SELECT;
    } else if (m_eMode == MODE_DELETE_CONFIRM) {
        // Confirm delete
        if (HitTestRect(0.42f, 0.78f, 0.16f, 0.06f, iX, iY) &&
            m_iSelectedSlot >= 0 && m_cbDelete) {
            m_cbDelete(m_pkCbCtx, m_aSlots[m_iSelectedSlot].uiCharId);
            m_eMode = MODE_SELECT;
        }
        // Cancel
        if (HitTestRect(0.62f, 0.78f, 0.16f, 0.06f, iX, iY))
            m_eMode = MODE_SELECT;
    }
}

void CharSelectUI::OnKeyDown(int iVK) {
    if (m_eMode == MODE_CREATE) {
        // Basic name input (A-Z, 0-9, backspace)
        if (iVK == VK_BACK && !m_kCreateName.empty()) {
            m_kCreateName.erase(m_kCreateName.size() - 1, 1);
        } else if (m_kCreateName.size() < 16) {
            if (iVK >= 'A' && iVK <= 'Z') m_kCreateName += (char)iVK;
            if (iVK >= '0' && iVK <= '9') m_kCreateName += (char)iVK;
        }
        // Tab through classes
        if (iVK == VK_TAB) m_uiCreateClass = (m_uiCreateClass + 1) % 5;
    }
    // Escape cancels any modal
    if (iVK == VK_ESCAPE) m_eMode = MODE_SELECT;
}

void CharSelectUI::SetCallbacks(SelectCallback onSelect,
                                 CreateCallback onCreate,
                                 DeleteCallback onDelete,
                                 void* pkCtx) {
    m_cbSelect = onSelect;
    m_cbCreate = onCreate;
    m_cbDelete = onDelete;
    m_pkCbCtx  = pkCtx;
}

// ── Private ───────────────────────────────────────────────────────────────────

void CharSelectUI::BuildSlotPanel(SlotPanel& rPanel, int iIdx,
                                   const CharSlot* pkSlot) {
    float fX = 0.15f + iIdx * 0.28f;
    float fY = 0.25f;
    float fW = 0.22f;
    float fH = 0.45f;

    // Background
    if (!rPanel.spBg)
        rPanel.spBg = NiNew NiScreenElements(NiPrimitiveType::PRIMITIVE_QUADS);
    if (rPanel.spBg) {
        rPanel.spBg->SetRectangle(0, fX, fY, fW, fH);
        // Try to load CharSlot.dds, fallback to dark grey panel
        NiTexture* pkTex = LoadTex("CharSlot.dds");
        if (pkTex) {
            NiTexturingProperty* pkProp = NiNew NiTexturingProperty();
            pkProp->SetBaseTexture(pkTex);
            pkProp->SetApplyMode(NiTexturingProperty::APPLY_MODULATE);
            rPanel.spBg->AttachProperty(pkProp);
            rPanel.spBg->SetColors(0, NiColorA(1,1,1,1));
        } else {
            rPanel.spBg->SetColors(0, NiColorA(0.15f, 0.15f, 0.2f, 0.9f));
        }
        rPanel.spBg->UpdateBound();
    }

    // Highlight overlay (shown when selected/hovered)
    if (!rPanel.spHighlight)
        rPanel.spHighlight = NiNew NiScreenElements(NiPrimitiveType::PRIMITIVE_QUADS);
    if (rPanel.spHighlight) {
        rPanel.spHighlight->SetRectangle(0, fX, fY, fW, fH);
        rPanel.spHighlight->SetColors(0, NiColorA(0.4f, 0.6f, 1.0f, 0.25f));
        NiAlphaProperty* pkAlpha = NiNew NiAlphaProperty();
        pkAlpha->SetAlphaBlending(true);
        pkAlpha->SetSrcBlendMode(NiAlphaProperty::ALPHA_SRCALPHA);
        pkAlpha->SetDestBlendMode(NiAlphaProperty::ALPHA_INVSRCALPHA);
        rPanel.spHighlight->AttachProperty(pkAlpha);
        rPanel.spHighlight->UpdateBound();
    }

    // Label
    if (pkSlot && pkSlot->bOccupied) {
        char buf[64];
        static const char* CLASS_NAMES[] = {
            "Fighter","Cleric","Archer","Mage","Trickster",
            "HolyKnight","Paladin","Ranger","WizMage","Rogue"
        };
        const char* szClass = (pkSlot->uiClass < 10)
            ? CLASS_NAMES[pkSlot->uiClass] : "Unknown";
        _snprintf_s(buf, sizeof(buf), "%s  Lv.%u  %s",
                    pkSlot->kName.c_str(), pkSlot->uiLevel, szClass);
        rPanel.kLabel   = buf;
        rPanel.uiCharId = pkSlot->uiCharId;
    } else {
        rPanel.kLabel   = "[ Empty ]";
        rPanel.uiCharId = 0;
    }
    rPanel.bHover    = false;
    rPanel.bSelected = false;
}

void CharSelectUI::BuildButtons() {
    // Enter button -- bottom center
    m_spBtnEnter = NiNew NiScreenElements(NiPrimitiveType::PRIMITIVE_QUADS);
    if (m_spBtnEnter) {
        NiTexture* pkTex = LoadTex("BtnEnter.dds");
        m_spBtnEnter->SetRectangle(0, 0.42f, 0.78f, 0.16f, 0.06f);
        m_spBtnEnter->SetColors(0, pkTex ? NiColorA(1,1,1,1) : NiColorA(0.2f,0.5f,0.2f,0.9f));
        if (pkTex) {
            NiTexturingProperty* p = NiNew NiTexturingProperty();
            p->SetBaseTexture(pkTex);
            m_spBtnEnter->AttachProperty(p);
        }
        m_spBtnEnter->UpdateBound();
    }

    // Create button -- right of enter
    m_spBtnCreate = NiNew NiScreenElements(NiPrimitiveType::PRIMITIVE_QUADS);
    if (m_spBtnCreate) {
        NiTexture* pkTex = LoadTex("BtnCreate.dds");
        m_spBtnCreate->SetRectangle(0, 0.62f, 0.78f, 0.16f, 0.06f);
        m_spBtnCreate->SetColors(0, pkTex ? NiColorA(1,1,1,1) : NiColorA(0.2f,0.3f,0.5f,0.9f));
        if (pkTex) {
            NiTexturingProperty* p = NiNew NiTexturingProperty();
            p->SetBaseTexture(pkTex);
            m_spBtnCreate->AttachProperty(p);
        }
        m_spBtnCreate->UpdateBound();
    }

    // Delete button -- left of enter
    m_spBtnDelete = NiNew NiScreenElements(NiPrimitiveType::PRIMITIVE_QUADS);
    if (m_spBtnDelete) {
        NiTexture* pkTex = LoadTex("BtnDelete.dds");
        m_spBtnDelete->SetRectangle(0, 0.22f, 0.78f, 0.16f, 0.06f);
        m_spBtnDelete->SetColors(0, pkTex ? NiColorA(1,1,1,1) : NiColorA(0.5f,0.1f,0.1f,0.9f));
        if (pkTex) {
            NiTexturingProperty* p = NiNew NiTexturingProperty();
            p->SetBaseTexture(pkTex);
            m_spBtnDelete->AttachProperty(p);
        }
        m_spBtnDelete->UpdateBound();
    }

    // Notice panel -- bottom center, taller
    m_spNoticePanel = NiNew NiScreenElements(NiPrimitiveType::PRIMITIVE_QUADS);
    if (m_spNoticePanel) {
        m_spNoticePanel->SetRectangle(0, 0.3f, 0.86f, 0.4f, 0.05f);
        m_spNoticePanel->SetColors(0, NiColorA(0.8f, 0.7f, 0.1f, 0.85f));
        m_spNoticePanel->UpdateBound();
    }
}

void CharSelectUI::RenderSlot(const SlotPanel& rPanel, NiRenderer* pkR) {
    if (rPanel.spBg) pkR->RenderScreenElements(rPanel.spBg);
    if ((rPanel.bSelected || rPanel.bHover) && rPanel.spHighlight)
        pkR->RenderScreenElements(rPanel.spHighlight);
}

void CharSelectUI::RenderButton(NiScreenElements* pkBtn, NiRenderer* pkR) {
    if (pkBtn) pkR->RenderScreenElements(pkBtn);
}

bool CharSelectUI::HitTestRect(float fX, float fY, float fW, float fH,
                                int iMouseX, int iMouseY) const {
    float fNX = (float)iMouseX / (float)m_iScreenW;
    float fNY = (float)iMouseY / (float)m_iScreenH;
    return fNX >= fX && fNX <= fX+fW && fNY >= fY && fNY <= fY+fH;
}

NiTexture* CharSelectUI::LoadTex(const char* szName) {
    const ShineConfig& cfg = ShineConfig::Get();
    std::string kPath = cfg.MenuPath(szName);
    std::map<std::string,NiTexturePtr>::iterator it = m_kTexCache.find(kPath);
    if (it != m_kTexCache.end()) return it->second;
    NiSourceTexture* pkTex = NiSourceTexture::Create(kPath.c_str());
    m_kTexCache[kPath] = pkTex;
    if (!pkTex) SHINELOG_DEBUG("CharSelectUI: texture missing '%s'", kPath.c_str());
    return pkTex;
}

} // namespace shine
