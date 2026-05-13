// Client/UI/ShineHotbar.cpp
#include "ShineHotbar.h"
#include "../Engine/ShineConfig.h"
#include "../Input/KeyMap.h"
#include "../../Server/Shared/ShineLogSystem.h"
#include <NiSourceTexture.h>
#include <NiTexturingProperty.h>
#include <NiAlphaProperty.h>
#include <cstring>

namespace shine {

ShineHotbar::ShineHotbar()
    : m_bVisible(false), m_iActiveBars(1), m_pkRenderer(NULL)
{}
ShineHotbar::~ShineHotbar() { Shutdown(); }

bool ShineHotbar::Init(NiRenderer* pkRenderer, const std::string& rCfgPath) {
    m_pkRenderer = pkRenderer;

    // Pre-load slot background texture
    const ShineConfig& cfg = ShineConfig::Get();
    m_spSlotBgTex = LoadTex(cfg.CtrlPath("SlotBg.dds").c_str());

    BuildSlotQuads();
    SHINELOG_INFO("ShineHotbar: initialized %d bars", BARS);
    return true;
}

void ShineHotbar::Shutdown() {
    for (int i = 0; i < TOTAL; ++i) {
        m_aSlotBg[i]   = NULL;
        m_aSlotIcon[i] = NULL;
    }
    m_kIconCache.clear();
    m_pkRenderer = NULL;
}

void ShineHotbar::OnShortcutCmd(const uint8* pData, uint32 uiLen) {
    // NC_CHAR_OPTION_SHORTCUT_CMD body:
    // Array of shortcut entries. Each entry:
    //   [uint8 type][uint32 refId][string inxName]
    // Entries are in slot order 0..TOTAL-1
    size_t pos = 0;
    for (int i = 0; i < TOTAL && pos + 5 < uiLen; ++i) {
        uint8  eType  = pData[pos++];
        uint32 uiRef  = 0;
        memcpy(&uiRef, pData + pos, 4); pos += 4;

        // Read null-terminated string
        std::string kName;
        while (pos < uiLen && pData[pos] != 0)
            kName += (char)pData[pos++];
        if (pos < uiLen) pos++; // consume null

        m_aSlots[i].eType    = (HotbarSlotType)eType;
        m_aSlots[i].uiRefId  = uiRef;
        m_aSlots[i].kInxName = kName;
        m_aSlots[i].uiBar    = (uint8)(i / SLOTS_PER);
        m_aSlots[i].uiSlotIdx= (uint8)(i % SLOTS_PER);
    }

    // Rebuild icon overlays for all bars
    for (int b = 0; b < BARS; ++b) RebuildBar(b);
    SHINELOG_INFO("ShineHotbar: shortcut data loaded %u bytes", uiLen);
}

void ShineHotbar::Update(float fDt) {}

void ShineHotbar::Render(NiRenderer* pkRenderer) {
    if (!m_bVisible || !pkRenderer) return;

    int iBarsToRender = m_iActiveBars;
    for (int b = 0; b < iBarsToRender; ++b) {
        for (int s = 0; s < SLOTS_PER; ++s) {
            int iIdx = b * SLOTS_PER + s;
            if (m_aSlotBg[iIdx])   pkRenderer->RenderScreenElements(m_aSlotBg[iIdx]);
            if (m_aSlotIcon[iIdx]) pkRenderer->RenderScreenElements(m_aSlotIcon[iIdx]);
        }
    }
}

bool ShineHotbar::OnKeyPress(uint8 uiFunctionNo) {
    // FunctionNo 47-56  = bar 0 slots 0-9   (1-0 keys)
    // FunctionNo 59-68  = bar 1 slots 0-9   (Shift+1..0)
    // FunctionNo 71-80  = bar 2 slots 0-9   (Alt+1..0)
    int iSlotIdx = -1;
    if (uiFunctionNo >= 47 && uiFunctionNo <= 56)
        iSlotIdx = 0 * SLOTS_PER + (uiFunctionNo - 47);
    else if (uiFunctionNo >= 59 && uiFunctionNo <= 68)
        iSlotIdx = 1 * SLOTS_PER + (uiFunctionNo - 59);
    else if (uiFunctionNo >= 71 && uiFunctionNo <= 80)
        iSlotIdx = 2 * SLOTS_PER + (uiFunctionNo - 71);

    if (iSlotIdx < 0 || iSlotIdx >= TOTAL) return false;

    const HotbarSlot& slot = m_aSlots[iSlotIdx];
    if (slot.eType == HSLOT_EMPTY) return true; // consumed but empty

    SHINELOG_DEBUG("ShineHotbar: activate slot %d type=%d ref=%u inx='%s'",
                   iSlotIdx, (int)slot.eType, slot.uiRefId, slot.kInxName.c_str());
    // TODO: route to SkillSystem or ItemSystem
    return true;
}

void ShineHotbar::SetActiveBars(int iBars) {
    m_iActiveBars = (iBars < 1) ? 1 : (iBars > BARS ? BARS : iBars);
}

// ── Private ───────────────────────────────────────────────────────────────────

void ShineHotbar::BuildSlotQuads() {
    for (int b = 0; b < BARS; ++b) {
        for (int s = 0; s < SLOTS_PER; ++s) {
            int   iIdx = b * SLOTS_PER + s;
            float fX, fY, fW, fH;
            GetSlotRect(b, s, fX, fY, fW, fH);

            // Slot background
            m_aSlotBg[iIdx] = NiNew NiScreenElements(NiPrimitiveType::PRIMITIVE_QUADS);
            if (m_aSlotBg[iIdx]) {
                m_aSlotBg[iIdx]->SetRectangle(0, fX, fY, fW, fH);
                if (m_spSlotBgTex) {
                    NiTexturingProperty* pkProp = NiNew NiTexturingProperty();
                    pkProp->SetBaseTexture(m_spSlotBgTex);
                    pkProp->SetApplyMode(NiTexturingProperty::APPLY_MODULATE);
                    m_aSlotBg[iIdx]->AttachProperty(pkProp);
                    m_aSlotBg[iIdx]->SetColors(0, NiColorA(1,1,1,0.85f));
                } else {
                    m_aSlotBg[iIdx]->SetColors(0, NiColorA(0.15f,0.15f,0.15f,0.85f));
                }
                NiAlphaProperty* pkAlpha = NiNew NiAlphaProperty();
                pkAlpha->SetAlphaBlending(true);
                pkAlpha->SetSrcBlendMode(NiAlphaProperty::ALPHA_SRCALPHA);
                pkAlpha->SetDestBlendMode(NiAlphaProperty::ALPHA_INVSRCALPHA);
                m_aSlotBg[iIdx]->AttachProperty(pkAlpha);
                m_aSlotBg[iIdx]->UpdateBound();
            }

            // Icon overlay (initially NULL -- filled by RebuildBar)
            m_aSlotIcon[iIdx] = NULL;
        }
    }
}

void ShineHotbar::RebuildBar(int iBar) {
    for (int s = 0; s < SLOTS_PER; ++s) {
        int   iIdx = iBar * SLOTS_PER + s;
        const HotbarSlot& slot = m_aSlots[iIdx];

        m_aSlotIcon[iIdx] = NULL;
        if (slot.eType == HSLOT_EMPTY || slot.kInxName.empty()) continue;

        // Load icon for this slot's InxName
        NiTexture* pkIcon = LoadIcon(slot.kInxName);
        if (!pkIcon) continue;

        float fX, fY, fW, fH;
        GetSlotRect(iBar, s, fX, fY, fW, fH);

        m_aSlotIcon[iIdx] = NiNew NiScreenElements(NiPrimitiveType::PRIMITIVE_QUADS);
        if (m_aSlotIcon[iIdx]) {
            m_aSlotIcon[iIdx]->SetRectangle(0, fX+0.001f, fY+0.001f,
                                             fW-0.002f, fH-0.002f);
            NiTexturingProperty* pkProp = NiNew NiTexturingProperty();
            pkProp->SetBaseTexture(pkIcon);
            pkProp->SetApplyMode(NiTexturingProperty::APPLY_MODULATE);
            NiAlphaProperty* pkAlpha = NiNew NiAlphaProperty();
            pkAlpha->SetAlphaBlending(true);
            pkAlpha->SetSrcBlendMode(NiAlphaProperty::ALPHA_SRCALPHA);
            pkAlpha->SetDestBlendMode(NiAlphaProperty::ALPHA_INVSRCALPHA);
            m_aSlotIcon[iIdx]->AttachProperty(pkProp);
            m_aSlotIcon[iIdx]->AttachProperty(pkAlpha);
            m_aSlotIcon[iIdx]->SetColors(0, NiColorA(1,1,1,1));
            m_aSlotIcon[iIdx]->UpdateBound();
        }
    }
}

void ShineHotbar::GetSlotRect(int iBar, int iSlot,
                               float& rfX, float& rfY,
                               float& rfW, float& rfH) const {
    // Bar 0: just above main bar at bottom
    // Bars 1,2: stacked above bar 0
    rfW = 0.044f;
    rfH = 0.055f;
    float fBarBaseY = 0.085f + iBar * 0.060f;
    rfX = 0.28f + iSlot * (rfW + 0.002f);
    rfY = fBarBaseY;
}

NiTexture* ShineHotbar::LoadIcon(const std::string& rInxName) {
    std::map<std::string,NiTexturePtr>::iterator it = m_kIconCache.find(rInxName);
    if (it != m_kIconCache.end()) return it->second;

    // Icon convention: resitem\<InxName>\icon.dds or resctrl\Icons\<InxName>.dds
    const ShineConfig& cfg = ShineConfig::Get();
    std::string kPath = cfg.ItemPath(rInxName.c_str(), "icon.dds");
    NiSourceTexture* pkTex = NiSourceTexture::Create(kPath.c_str());
    if (!pkTex) {
        // Try resctrl\Icons\
        kPath = cfg.CtrlPath(("Icons\\" + rInxName + ".dds").c_str());
        pkTex = NiSourceTexture::Create(kPath.c_str());
    }
    m_kIconCache[rInxName] = pkTex;
    return pkTex;
}

NiTexture* ShineHotbar::LoadTex(const char* szPath) {
    std::string k(szPath);
    std::map<std::string,NiTexturePtr>::iterator it = m_kIconCache.find(k);
    if (it != m_kIconCache.end()) return it->second;
    NiSourceTexture* pkTex = NiSourceTexture::Create(szPath);
    m_kIconCache[k] = pkTex;
    return pkTex;
}

} // namespace shine
