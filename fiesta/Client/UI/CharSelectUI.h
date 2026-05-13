// Client/UI/CharSelectUI.h
// Character select screen.
// Shows slots as panels loaded from resmenu\game\CharSelect*.dds.
// Supports: select, create (inline form), delete with confirm.
// Falls back to flat color quads if textures missing.
#ifndef SHINE_CLIENT_UI_CHARSELECTUI_H
#define SHINE_CLIENT_UI_CHARSELECTUI_H

#include <NiMain.h>
#include "../../Server/Shared/ShineTypes.h"
#include <string>
#include <vector>
#include <map>

namespace shine {
struct CharSlot;

// VS2010-compatible callback typedefs (no <functional>/std::function).
// Each handler gets a void* ctx so a single static thunk can route back
// into the owning object.
typedef void (*SelectCallback)(void* pkCtx, uint32 uiCharId);
typedef void (*CreateCallback)(void* pkCtx, const std::string& rName,
                                uint16 uiClass, uint8 uiHair,
                                uint8 uiHairColor, uint8 uiFace);
typedef void (*DeleteCallback)(void* pkCtx, uint32 uiCharId);

class CharSelectUI {
public:
    CharSelectUI();
    ~CharSelectUI();

    bool Init(NiRenderer* pkRenderer, const std::vector<CharSlot>& rSlots);
    void Shutdown();
    bool IsInitialized() const { return m_bInitialized; }

    void Update(float fDt);
    void Render(NiRenderer* pkRenderer);

    // Called after create/delete to refresh displayed slots
    void Refresh(const std::vector<CharSlot>& rSlots);

    // Show a one-line notice (create result, delete result, etc.)
    void ShowNotice(const char* szMsg);

    // Input -- called from WndProc
    void OnMouseMove  (int iX, int iY);
    void OnLButtonDown(int iX, int iY);
    void OnKeyDown    (int iVK);

    void SetCallbacks(SelectCallback onSelect,
                      CreateCallback onCreate,
                      DeleteCallback onDelete,
                      void* pkCtx);

private:
    enum Mode { MODE_SELECT, MODE_CREATE, MODE_DELETE_CONFIRM };

    bool                m_bInitialized;
    Mode                m_eMode;
    NiRenderer*         m_pkRenderer;

    // Slot panels (max 3 characters)
    static const int    MAX_SLOTS = 3;

    struct SlotPanel {
        NiScreenElementsPtr spBg;
        NiScreenElementsPtr spHighlight;
        bool                bHover;
        bool                bSelected;
        uint32              uiCharId;
        std::string         kLabel;   // "Name  Lv.X  ClassName"
    } m_aSlots[MAX_SLOTS];

    // Buttons
    NiScreenElementsPtr m_spBtnEnter;
    NiScreenElementsPtr m_spBtnCreate;
    NiScreenElementsPtr m_spBtnDelete;
    NiScreenElementsPtr m_spNoticePanel;

    // Create form fields (simple)
    std::string         m_kCreateName;
    uint16              m_uiCreateClass;
    uint8               m_uiCreateHair;
    uint8               m_uiCreateHairColor;
    uint8               m_uiCreateFace;

    std::string         m_kNoticeMsg;
    float               m_fNoticeTimer;

    int                 m_iSelectedSlot;
    int                 m_iMouseX, m_iMouseY;
    int                 m_iScreenW, m_iScreenH;

    SelectCallback      m_cbSelect;
    CreateCallback      m_cbCreate;
    DeleteCallback      m_cbDelete;
    void*               m_pkCbCtx;

    NiTexture*          LoadTex(const char* szName);
    void                BuildSlotPanel(SlotPanel& rPanel, int iIdx,
                                       const CharSlot* pkSlot);
    void                BuildButtons();
    void                RenderSlot(const SlotPanel& rPanel, NiRenderer* pkR);
    void                RenderButton(NiScreenElements* pkBtn, NiRenderer* pkR);
    bool                HitTestRect(float fX, float fY, float fW, float fH,
                                    int iMouseX, int iMouseY) const;

    std::map<std::string, NiTexturePtr> m_kTexCache;
};

} // namespace shine
#endif // SHINE_CLIENT_UI_CHARSELECTUI_H
