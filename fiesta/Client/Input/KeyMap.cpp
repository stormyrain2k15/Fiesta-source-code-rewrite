// Client/Input/KeyMap.cpp
// Default bindings verbatim from tKeyMapInit / Key_Remapping.sql
#include "KeyMap.h"
#include "../../Server/Shared/ShineLogSystem.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <cstdio>
#include <cstring>

namespace shine {

// ── Default layout from tKeyMapInit (Key_Remapping.sql) ──────────────────────
// {nFunctionNo, nExtendedKey, nASCIICode}
const KeyBinding KeyMap::s_kDefaults[] = {
    {  0,  0, 121 }, // F10
    {  1,  0,  27 }, // ESC
    {  2,  0,  67 }, // C
    {  3,  0,  86 }, // V
    {  4,  0,  73 }, // I
    {  5,  0,  75 }, // K
    {  6,  0,  76 }, // L
    {  7,  0,  72 }, // H
    {  8,  0,  88 }, // X
    {  9,  0, 222 }, // '
    { 10,  0,  77 }, // M
    { 11,  0,  85 }, // U
    { 12,  0,  80 }, // P
    { 13,  0,  82 }, // R
    { 14,  0,  70 }, // F
    { 15,  0,  71 }, // G
    { 16,  0,   0 }, // unbound
    { 17,  0,   0 }, // unbound
    { 18,  0,   0 }, // unbound
    { 19,  0,   0 }, // unbound
    { 20,  0,  13 }, // Enter
    { 21, 18,  78 }, // Alt+N
    { 22, 18,  71 }, // Alt+G
    { 23,  0,   0 }, // unbound
    { 24, 18,  80 }, // Alt+P
    { 25,  0,   0 }, // unbound
    { 26,  0,   0 }, // unbound
    { 27,  0,  87 }, // W (forward)
    { 28,  0,  83 }, // S (back)
    { 29, 16,  83 }, // Shift+S
    { 30,  0,  65 }, // A (left)
    { 31,  0,  68 }, // D (right)
    { 32,  0,  32 }, // Space
    { 33,  0,   0 }, // unbound
    { 34,  0,  38 }, // Up arrow
    { 35,  0,  40 }, // Down arrow
    { 36,  0,  37 }, // Left arrow
    { 37,  0,  39 }, // Right arrow
    { 38,  0,   0 }, // unbound
    { 39,  0,   0 }, // unbound
    { 40,  0,  36 }, // Home
    { 41,  0,  84 }, // T (target)
    { 42,  0, 245 }, // VK_PROCESSKEY
    { 43,  0,  66 }, // B (bank)
    { 44, 16,  90 }, // Shift+Z (screenshot)
    { 45,  0,   0 }, // unbound
    { 46,  0,  35 }, // End
    { 47,  0,  49 }, // 1
    { 48,  0,  50 }, // 2
    { 49,  0,  51 }, // 3
    { 50,  0,  52 }, // 4
    { 51,  0,  53 }, // 5
    { 52,  0,  54 }, // 6
    { 53,  0,  55 }, // 7
    { 54,  0,  56 }, // 8
    { 55,  0,  57 }, // 9
    { 56,  0,  48 }, // 0
    { 57,  0, 189 }, // -
    { 58,  0, 187 }, // =
    { 59, 16,  49 }, // Shift+1
    { 60, 16,  50 }, // Shift+2
    { 61, 16,  51 }, // Shift+3
    { 62, 16,  52 }, // Shift+4
    { 63, 16,  53 }, // Shift+5
    { 64, 16,  54 }, // Shift+6
    { 65, 16,  55 }, // Shift+7
    { 66, 16,  56 }, // Shift+8
    { 67, 16,  57 }, // Shift+9
    { 68, 16,  48 }, // Shift+0
    { 69, 16, 189 }, // Shift+-
    { 70, 16, 187 }, // Shift+=
    { 71, 18,  49 }, // Alt+1
    { 72, 18,  50 }, // Alt+2
    { 73, 18,  51 }, // Alt+3
    { 74, 18,  52 }, // Alt+4
    { 75, 18,  53 }, // Alt+5
    { 76, 18,  54 }, // Alt+6
    { 77, 18,  55 }, // Alt+7
    { 78, 18,  56 }, // Alt+8
    { 79, 18,  57 }, // Alt+9
    { 80, 18,  48 }, // Alt+0
    { 81, 18, 189 }, // Alt+-
    { 82, 18, 187 }, // Alt+=
    { 83,  0,   0 }, // unbound
    { 84,  0,   0 }, // unbound
    { 85,  0,   0 }, // unbound
    { 86,  0,   0 }, // unbound
    { 87,  0,   0 }, // unbound
    { 88,  0,   0 }, // unbound
    { 89,  0,   0 }, // unbound
    { 90,  0,   0 }, // unbound
    { 91,  0,   0 }, // unbound
    { 92,  0,   0 }, // unbound
    { 93,  0,   0 }, // unbound
    { 94,  0,   0 }, // unbound
};
const uint32 KeyMap::s_uiDefaultCount = 95;

// ── KeyMap ────────────────────────────────────────────────────────────────────

KeyMap::KeyMap() : m_bDirty(false) {
    LoadDefaults();
}

KeyMap& KeyMap::Get() { static KeyMap s; return s; }

void KeyMap::Init(const std::string& rCfgPath) {
    m_kCfgPath = rCfgPath;
    LoadFromCfg(); // overlay saved remaps on defaults
    SHINELOG_INFO("KeyMap: init (%u bindings) from '%s'",
                  (uint32)m_kBindings.size(), rCfgPath.c_str());
}

void KeyMap::LoadDefaults() {
    m_kBindings.resize(s_uiDefaultCount);
    for (uint32 i = 0; i < s_uiDefaultCount; ++i)
        m_kBindings[i] = s_kDefaults[i];
}

void KeyMap::ResetToDefault() {
    LoadDefaults();
    m_bDirty = true;
    Save();
    SHINELOG_INFO("KeyMap: reset to tKeyMapInit defaults");
}

// ── Server packet ─────────────────────────────────────────────────────────────

void KeyMap::OnServerKeyMap(const uint8* pData, uint32 uiLen) {
    // Server sends NC_CHAR_OPTION_KEYMAP_CMD body as an array of
    // {uint8 extendedKey, uint8 asciiCode} pairs, one per function.
    // Pair index == nFunctionNo.
    uint32 uiPairs = uiLen / 2;
    if (uiPairs == 0) return;

    if (uiPairs > (uint32)m_kBindings.size())
        m_kBindings.resize(uiPairs);

    for (uint32 i = 0; i < uiPairs; ++i) {
        m_kBindings[i].uiFunctionNo  = (uint8)i;
        m_kBindings[i].uiExtendedKey = pData[i * 2 + 0];
        m_kBindings[i].uiASCIICode   = pData[i * 2 + 1];
    }

    m_bDirty = true;
    Save();
    SHINELOG_INFO("KeyMap: server sent %u bindings -- saved", uiPairs);
}

void KeyMap::BuildServerBlob(std::vector<uint8>& rOut) const {
    rOut.clear();
    rOut.reserve(m_kBindings.size() * 2);
    for (size_t i = 0; i < m_kBindings.size(); ++i) {
        rOut.push_back(m_kBindings[i].uiExtendedKey);
        rOut.push_back(m_kBindings[i].uiASCIICode);
    }
}

// ── Save / Load ───────────────────────────────────────────────────────────────

void KeyMap::Save() {
    if (m_kCfgPath.empty()) return;

    FILE* f = NULL;
    fopen_s(&f, m_kCfgPath.c_str(), "w");
    if (!f) {
        SHINELOG_WARN("KeyMap::Save: cannot write '%s'", m_kCfgPath.c_str());
        return;
    }

    fprintf(f, "; ShineKeys.cfg -- character key bindings\n");
    fprintf(f, "; Format: FunctionNo = ExtendedKey(0/16/18) VKCode\n");
    fprintf(f, "; ExtendedKey: 0=none  16=Shift  18=Alt\n");
    fprintf(f, "; Delete this file to restore tKeyMapInit defaults.\n\n");
    fprintf(f, "[KeyMap]\n");

    for (size_t i = 0; i < m_kBindings.size(); ++i) {
        fprintf(f, "%u=%u %u\n",
                m_kBindings[i].uiFunctionNo,
                m_kBindings[i].uiExtendedKey,
                m_kBindings[i].uiASCIICode);
    }

    fclose(f);
    m_bDirty = false;
    SHINELOG_INFO("KeyMap: saved %u bindings to '%s'",
                  (uint32)m_kBindings.size(), m_kCfgPath.c_str());
}

void KeyMap::LoadFromCfg() {
    if (m_kCfgPath.empty()) return;

    FILE* f = NULL;
    fopen_s(&f, m_kCfgPath.c_str(), "r");
    if (!f) return; // no saved file -- keep defaults

    char line[128];
    while (fgets(line, sizeof(line), f)) {
        if (line[0] == ';' || line[0] == '[' || line[0] == '\n') continue;
        uint32 fnNo = 0, ext = 0, vk = 0;
        if (sscanf_s(line, "%u=%u %u", &fnNo, &ext, &vk) == 3) {
            if (fnNo < (uint32)m_kBindings.size()) {
                m_kBindings[fnNo].uiFunctionNo  = (uint8)fnNo;
                m_kBindings[fnNo].uiExtendedKey = (uint8)ext;
                m_kBindings[fnNo].uiASCIICode   = (uint8)vk;
            }
        }
    }
    fclose(f);
    SHINELOG_INFO("KeyMap: loaded from '%s'", m_kCfgPath.c_str());
}

// ── Lookups ───────────────────────────────────────────────────────────────────

const KeyBinding* KeyMap::GetBinding(uint8 uiFunctionNo) const {
    if (uiFunctionNo >= (uint8)m_kBindings.size()) return NULL;
    return &m_kBindings[uiFunctionNo];
}

uint8 KeyMap::FindFunction(uint8 uiVK, uint8 uiExtended) const {
    for (size_t i = 0; i < m_kBindings.size(); ++i) {
        if (m_kBindings[i].uiASCIICode   == uiVK &&
            m_kBindings[i].uiExtendedKey  == uiExtended)
            return (uint8)i;
    }
    return 0xFF; // not bound
}

bool KeyMap::IsPressed(uint8 uiFunctionNo) const {
    const KeyBinding* p = GetBinding(uiFunctionNo);
    if (!p || p->uiASCIICode == 0) return false;

    bool bKey = (GetAsyncKeyState(p->uiASCIICode) & 0x8000) != 0;
    if (!bKey) return false;

    // Check modifier
    switch (p->uiExtendedKey) {
    case 16: return (GetAsyncKeyState(VK_SHIFT) & 0x8000) != 0;
    case 18: return (GetAsyncKeyState(VK_MENU)  & 0x8000) != 0;
    default: return true;
    }
}

} // namespace shine
