// Client/UI/ShineHotbar.h
// Skill/item hotbar -- 3 bars of 10 slots each (30 total, matching tKeyMapInit).
// Bar 1: keys 1-0       (FunctionNo 47-56)
// Bar 2: Shift+1..0     (FunctionNo 59-68)
// Bar 3: Alt+1..0       (FunctionNo 71-80)
// Slot contents loaded from NC_CHAR_OPTION_SHORTCUT_CMD (0x7032).
// Renders below MainBar. Slot textures from resctrl\SlotBg.dds + item icons.
#ifndef SHINE_CLIENT_UI_SHINEHOTBAR_H
#define SHINE_CLIENT_UI_SHINEHOTBAR_H

#include <NiMain.h>
#include "../../Server/Shared/ShineTypes.h"
#include "UILayout.h"
#include <string>
#include <map>

namespace shine {

enum HotbarSlotType { HSLOT_EMPTY=0, HSLOT_SKILL=1, HSLOT_ITEM=2, HSLOT_MACRO=3 };

struct HotbarSlot {
    HotbarSlotType  eType;
    uint32          uiRefId;     // skill InxNo or item InxNo
    std::string     kInxName;    // for icon lookup
    uint8           uiBar;       // 0,1,2
    uint8           uiSlotIdx;   // 0-9 within bar

    HotbarSlot() : eType(HSLOT_EMPTY), uiRefId(0), uiBar(0), uiSlotIdx(0) {}
};

class ShineHotbar {
public:
    ShineHotbar();
    ~ShineHotbar();

    bool Init(NiRenderer* pkRenderer, const std::string& rCfgPath);
    void Shutdown();

    // Called from ZoneSession when NC_CHAR_OPTION_SHORTCUT_CMD arrives
    // pData = raw body bytes, uiLen = size
    void OnShortcutCmd(const uint8* pData, uint32 uiLen);

    void Update(float fDt);
    void Render(NiRenderer* pkRenderer);

    // Key press from ShineInput -- checks if a hotbar function key was hit
    // Returns true if the key was a hotbar activation
    bool OnKeyPress(uint8 uiFunctionNo);

    void SetVisible(bool b) { m_bVisible = b; }
    bool IsVisible()  const { return m_bVisible; }

    // Number of active bars to show (1 or 2, default 1; more can be toggled)
    void SetActiveBars(int iBars);

private:
    static const int BARS      = 3;
    static const int SLOTS_PER = 10;
    static const int TOTAL     = BARS * SLOTS_PER;

    bool            m_bVisible;
    int             m_iActiveBars;
    NiRenderer*     m_pkRenderer;

    HotbarSlot      m_aSlots[TOTAL];

    // Screen elements per slot (slot bg + icon overlay)
    NiScreenElementsPtr m_aSlotBg  [TOTAL];
    NiScreenElementsPtr m_aSlotIcon[TOTAL];
    NiTexturePtr        m_spSlotBgTex;

    // Icon texture cache
    std::map<std::string, NiTexturePtr> m_kIconCache;

    void BuildSlotQuads();
    void RebuildBar(int iBar);
    NiTexture* LoadIcon(const std::string& rInxName);
    NiTexture* LoadTex(const char* szPath);

    // Slot position for bar i, slot j in normalized screen coords
    void GetSlotRect(int iBar, int iSlot,
                     float& rfX, float& rfY, float& rfW, float& rfH) const;
};

} // namespace shine
#endif // SHINE_CLIENT_UI_SHINEHOTBAR_H
