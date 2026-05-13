// Client/Engine/ShineConfig.cpp
#include "ShineConfig.h"
#include "../../Server/Shared/ShineLogSystem.h"

namespace shine {

ShineConfig& ShineConfig::Get() {
    static ShineConfig s;
    return s;
}

ShineConfig::ShineConfig()
    : uiLoginPort(9010), uiWMPort(28000), uiZonePort(9020),
      uiCharId(1),
      uiWidth(1280), uiHeight(720), bFullscreen(false), bVSync(true),
      uiMasterVolume(80), uiBGMVolume(70), uiSFXVolume(85), bEnableAudio(true),
      fDefaultDist(600.0f), fDefaultPitch(-45.0f),
      fMinDist(100.0f), fMaxDist(3000.0f), fOrbitSensitivity(0.4f),
      uiLogLevel(2), bSkipLogin(false),
      bShowCollision(false), bShowBounds(false), bLogPackets(false)
{
    // Hardcoded defaults (used when ini is missing)
    kLoginIP  = "127.0.0.1";
    kWMIP     = "127.0.0.1";
    kZoneIP   = "127.0.0.1";
    kUser     = "admin";
    kPass     = "admin";
    kTitle    = "Shine";
    kRenderer = "DX9";
    kBaseDir  = ".";
    kResMap    = ".\\resmap";
    kResChar   = ".\\reschar";
    kResItem   = ".\\resitem";
    kResCtrl   = ".\\resctrl";
    kResEffect = ".\\reseffect";
    kResMenu   = ".\\resmenu";
    kResSound  = ".\\ressound";
    kResSystem = ".\\ressystem";
    kShaderDir = ".\\shader";
    kSkinDir   = ".\\Skin";
    kMilesDir  = ".\\miles";
    kDumpDir   = ".\\Dump";
    kDataDir   = ".\\Data\\Shine";
    kResAction = ".\\ressystem\\action";
    kLogFile   = "ShineClient.log";
}

bool ShineConfig::Load(const char* szIniPath) {
    const char* f = szIniPath;

    // Verify file exists
    if (GetFileAttributesA(szIniPath) == INVALID_FILE_ATTRIBUTES) {
        SHINELOG_WARN("ShineConfig: '%s' not found -- using defaults", szIniPath);
        return false;
    }

    // [Server]
    kLoginIP      = S("Server", "LoginIP",   "127.0.0.1", f);
    uiLoginPort   = (uint16)I("Server", "LoginPort",   9010, f);
    kWMIP         = S("Server", "WMIP",      "127.0.0.1", f);
    uiWMPort      = (uint16)I("Server", "WMPort",     28000, f);
    kZoneIP       = S("Server", "ZoneIP",    "127.0.0.1", f);
    uiZonePort    = (uint16)I("Server", "ZonePort",    9020, f);

    // [Account]
    kUser    = S("Account", "User",   "admin", f);
    kPass    = S("Account", "Pass",   "admin", f);
    uiCharId = (uint32)I("Account", "CharId", 1, f);

    // [Window]
    uiWidth     = (uint32)I("Window", "Width",      1280, f);
    uiHeight    = (uint32)I("Window", "Height",      720, f);
    bFullscreen = B("Window", "Fullscreen",    false, f);
    kTitle      = S("Window", "Title",       "Shine", f);
    kRenderer   = S("Window", "Renderer",     "DX9", f);
    bVSync      = B("Window", "VSync",         true, f);

    // [Paths]
    kBaseDir   = S("Paths", "BaseDir",    ".",              f);
    kResMap    = S("Paths", "ResMap",     ".\\resmap",      f);
    kResChar   = S("Paths", "ResChar",    ".\\reschar",     f);
    kResItem   = S("Paths", "ResItem",    ".\\resitem",     f);
    kResCtrl   = S("Paths", "ResCtrl",    ".\\resctrl",     f);
    kResEffect = S("Paths", "ResEffect",  ".\\reseffect",   f);
    kResMenu   = S("Paths", "ResMenu",    ".\\resmenu",     f);
    kResSound  = S("Paths", "ResSound",   ".\\ressound",    f);
    kResSystem = S("Paths", "ResSystem",  ".\\ressystem",   f);
    kShaderDir = S("Paths", "ShaderDir",  ".\\shader",      f);
    kSkinDir   = S("Paths", "SkinDir",    ".\\Skin",        f);
    kMilesDir  = S("Paths", "MilesDir",   ".\\miles",       f);
    kDumpDir   = S("Paths", "DumpDir",    ".\\Dump",        f);
    kDataDir   = S("Paths", "DataDir",    ".\\Data\\Shine", f);
    kResAction = S("Paths", "ResAction",  ".\\ressystem\\action", f);

    // [MapPaths] -- enumerate all keys in the section
    // GetPrivateProfileSection gives us "key=value\0key=value\0\0"
    kMapFolders.clear();
    char szSection[8192];
    GetPrivateProfileSectionA("MapPaths", szSection, sizeof(szSection), f);
    char* p = szSection;
    while (*p) {
        std::string kv(p);
        size_t eq = kv.find('=');
        if (eq != std::string::npos) {
            uint32 uiId = (uint32)atoi(kv.substr(0, eq).c_str());
            std::string kFolder = kv.substr(eq + 1);
            if (uiId > 0 && !kFolder.empty())
                kMapFolders[uiId] = kFolder;
        }
        p += kv.size() + 1;
    }

    // [Audio]
    uiMasterVolume = (uint32)I("Audio", "MasterVolume", 80,  f);
    uiBGMVolume    = (uint32)I("Audio", "BGMVolume",    70,  f);
    uiSFXVolume    = (uint32)I("Audio", "SFXVolume",    85,  f);
    bEnableAudio   = B("Audio", "EnableAudio", true, f);

    // [Camera]
    fDefaultDist     = F("Camera", "DefaultDist",     600.0f, f);
    fDefaultPitch    = F("Camera", "DefaultPitch",    -45.0f, f);
    fMinDist         = F("Camera", "MinDist",         100.0f, f);
    fMaxDist         = F("Camera", "MaxDist",        3000.0f, f);
    fOrbitSensitivity= F("Camera", "OrbitSensitivity",  0.4f, f);

    // [Debug]
    uiLogLevel     = (uint32)I("Debug", "LogLevel",      2, f);
    kLogFile       = S("Debug", "LogFile", "ShineClient.log", f);
    bSkipLogin     = B("Debug", "SkipLogin",   false, f);
    bShowCollision = B("Debug", "ShowCollision",false, f);
    bShowBounds    = B("Debug", "ShowBounds",  false, f);
    bLogPackets    = B("Debug", "LogPackets",  false, f);

    // Diagnostics
    SHINELOG_INFO("ShineConfig loaded: %s", szIniPath);
    SHINELOG_INFO("  Server  : login=%s:%u  wm=%s:%u  zone=%s:%u",
                  kLoginIP.c_str(), uiLoginPort,
                  kWMIP.c_str(),    uiWMPort,
                  kZoneIP.c_str(),  uiZonePort);
    SHINELOG_INFO("  Account : user=%s charId=%u", kUser.c_str(), uiCharId);
    SHINELOG_INFO("  Window  : %ux%u fs=%d renderer=%s",
                  uiWidth, uiHeight, bFullscreen, kRenderer.c_str());
    SHINELOG_INFO("  Paths   : base=%s data=%s map=%s",
                  kBaseDir.c_str(), kDataDir.c_str(), kResMap.c_str());
    SHINELOG_INFO("  MapPaths: %u entries", (uint32)kMapFolders.size());
    SHINELOG_INFO("  Debug   : logLevel=%u skipLogin=%d logPackets=%d",
                  uiLogLevel, bSkipLogin, bLogPackets);

    return true;
}

} // namespace shine
