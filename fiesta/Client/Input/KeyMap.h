// Client/Input/KeyMap.h
// Character key binding system.
// Default layout sourced directly from tKeyMapInit (Key_Remapping.sql).
// Server sends the current layout on login via NC_CHAR_OPTION_KEYMAP_CMD (0x7033).
// Client saves any user remaps to ShineKeys.cfg beside the exe.
// On new account the server has no saved map and sends the tKeyMapInit defaults.
//
// nExtendedKey: 0=no modifier  16=Shift  18=Alt
// nASCIICode:   Windows virtual key code (VK_*)
//
// Function index meanings (from tKeyMapInit + server PDB):
//   0  = F10 (menu/system)       10 = M (map)           27 = W (move forward)
//   1  = ESC                     11 = U (upgrade)        28 = S (move back)
//   2  = C (character)           12 = P (party)          29 = Shift+S
//   3  = V (inventory)           13 = R (ride)           30 = A (strafe left)
//   4  = I (inventory2)          14 = F (friend)         31 = D (strafe right)
//   5  = K (skill)               15 = G (guild)          32 = Space (jump/confirm)
//   6  = L (quest log)           20 = Enter (chat)       40 = Home (camera reset)
//   7  = H (hotbar toggle)       21 = Alt+N              41 = T (target)
//   8  = X (sit/rest)            22 = Alt+G              43 = B (bank)
//   9  = ' (emote)               24 = Alt+P              44 = Shift+Z (screenshot)
//   47-56  = 1-0    (hotbar slots 1-10)
//   59-70  = Shift+1..Shift+0  (hotbar slots 11-20)
//   71-82  = Alt+1..Alt+0     (hotbar slots 21-32)
#ifndef SHINE_CLIENT_INPUT_KEYMAP_H
#define SHINE_CLIENT_INPUT_KEYMAP_H

#include "../../Server/Shared/ShineTypes.h"
#include <string>
#include <vector>

namespace shine {

struct KeyBinding {
    uint8   uiFunctionNo;
    uint8   uiExtendedKey;  // 0=none 16=Shift 18=Alt
    uint8   uiASCIICode;    // Windows VK code
};

class KeyMap {
public:
    static KeyMap& Get();

    // Load from ShineKeys.cfg (overlay on top of defaults).
    // rCfgPath = full path to ShineKeys.cfg beside the exe.
    void Init(const std::string& rCfgPath);

    // Called when NC_CHAR_OPTION_KEYMAP_CMD (0x7033) arrives from server.
    // pData = raw packet body bytes, uiLen = body size.
    // Overwrites current bindings with server data and saves to cfg.
    void OnServerKeyMap(const uint8* pData, uint32 uiLen);

    // Save current bindings to cfg file
    void Save();

    // Reset to tKeyMapInit defaults
    void ResetToDefault();

    // Get binding for a function
    const KeyBinding* GetBinding(uint8 uiFunctionNo) const;

    // Find function number for a key combo (returns 0xFF if not bound)
    uint8 FindFunction(uint8 uiVK, uint8 uiExtended) const;

    // Check if a key combo is currently pressed (uses GetAsyncKeyState)
    bool IsPressed(uint8 uiFunctionNo) const;

    const std::vector<KeyBinding>& Bindings() const { return m_kBindings; }

    // Build the binary blob the server expects for NC_CHAR_OPTION_KEYMAP_CMD
    // (for sending saved keybinds back if server requests them)
    void BuildServerBlob(std::vector<uint8>& rOut) const;

private:
    KeyMap();
    void LoadDefaults();
    void LoadFromCfg();

    std::vector<KeyBinding> m_kBindings;   // indexed by FunctionNo
    std::string             m_kCfgPath;
    bool                    m_bDirty;

    // Default bindings from tKeyMapInit (Key_Remapping.sql)
    static const KeyBinding s_kDefaults[];
    static const uint32     s_uiDefaultCount;
};

} // namespace shine
#endif // SHINE_CLIENT_INPUT_KEYMAP_H
