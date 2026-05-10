// Client/Engine/ShineConfig.h
// Reads ShineClient.ini and exposes every setting as a typed struct.
// Single point of truth for all client configuration — no magic strings
// scattered through the codebase.
#ifndef SHINE_CLIENT_ENGINE_SHINECONFIG_H
#define SHINE_CLIENT_ENGINE_SHINECONFIG_H

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <string>
#include <map>
#include "../../Server/Shared/ShineTypes.h"

namespace fiesta {

struct ShineConfig {

    // ── [Server] ──────────────────────────────────────────────────────────
    std::string kLoginIP;
    uint16      uiLoginPort;

    std::string kWMIP;
    uint16      uiWMPort;

    std::string kZoneIP;       // Phase 1 direct-connect
    uint16      uiZonePort;

    // ── [Account] ─────────────────────────────────────────────────────────
    std::string kUser;
    std::string kPass;
    uint32      uiCharId;

    // ── [Window] ──────────────────────────────────────────────────────────
    uint32      uiWidth;
    uint32      uiHeight;
    bool        bFullscreen;
    std::string kTitle;
    std::string kRenderer;     // "DX9" or "DX10"
    bool        bVSync;

    // ── [Paths] ───────────────────────────────────────────────────────────
    std::string kBaseDir;
    std::string kResMap;
    std::string kResChar;
    std::string kResItem;
    std::string kResCtrl;
    std::string kResEffect;
    std::string kResMenu;
    std::string kResSound;
    std::string kResSystem;
    std::string kShaderDir;
    std::string kSkinDir;
    std::string kMilesDir;
    std::string kDumpDir;
    std::string kDataDir;
    std::string kResAction;    // ressystem/action/ -- Gamebryo action .dat files linking animations to character NIFs

    // ── [MapPaths] ────────────────────────────────────────────────────────
    // MapId -> folder name under kResMap
    std::map<uint32, std::string> kMapFolders;

    // Resolve a map folder by ID: returns kMapFolders[id] if present,
    // else the numeric ID as a string.
    std::string ResolveMapFolder(uint32 uiMapId) const {
        std::map<uint32,std::string>::const_iterator it = kMapFolders.find(uiMapId);
        if (it != kMapFolders.end()) return it->second;
        char buf[16]; _itoa_s(uiMapId, buf, 10);
        return std::string(buf);
    }

    // Full path helpers -- use these in loaders, not raw strings
    std::string MapPath   (uint32 uiMapId, const char* szFile) const {
        return kResMap + "\\" + ResolveMapFolder(uiMapId) + "\\" + szFile;
    }
    std::string CharPath  (const char* szType, const char* szFile) const {
        return kResChar + "\\" + szType + "\\" + szFile;
    }
    std::string ItemPath  (const char* szInx,  const char* szFile) const {
        return kResItem + "\\" + szInx  + "\\" + szFile;
    }
    std::string MenuPath  (const char* szFile) const {
        return kResMenu + "\\game\\" + szFile;
    }
    std::string EffectPath(const char* szName, const char* szFile) const {
        return kResEffect + "\\" + szName + "\\" + szFile;
    }
    std::string SoundPath (const char* szCat,  const char* szFile) const {
        return kResSound + "\\" + szCat + "\\" + szFile;
    }
    std::string SystemPath(const char* szFile) const {
        return kResSystem + "\\" + szFile;
    }
    std::string ShaderPath(const char* szFile) const {
        return kShaderDir + "\\" + szFile;
    }
    std::string SkinPath  (const char* szFile) const {
        return kSkinDir + "\\" + szFile;
    }
    std::string ActionPath(const char* szFile) const {
        return kResAction + "\\" + szFile;
    }
    std::string DataPath  (const char* szFile) const {
        return kDataDir + "\\" + szFile;
    }

    // ── [Audio] ───────────────────────────────────────────────────────────
    uint32      uiMasterVolume;
    uint32      uiBGMVolume;
    uint32      uiSFXVolume;
    bool        bEnableAudio;

    // ── [Camera] ──────────────────────────────────────────────────────────
    float       fDefaultDist;
    float       fDefaultPitch;
    float       fMinDist;
    float       fMaxDist;
    float       fOrbitSensitivity;

    // ── [Debug] ───────────────────────────────────────────────────────────
    uint32      uiLogLevel;
    std::string kLogFile;
    bool        bSkipLogin;    // Phase 1: bypass login, connect direct to Zone
    bool        bShowCollision;
    bool        bShowBounds;
    bool        bLogPackets;

    // ── Load / singleton ──────────────────────────────────────────────────
    static ShineConfig& Get();

    // Load from ini file. szIniPath = full path to ShineClient.ini.
    // Returns false if file is missing (uses hardcoded defaults instead).
    bool Load(const char* szIniPath);

private:
    ShineConfig();

    // Helpers
    static std::string S(const char* sec, const char* key, const char* def, const char* f) {
        char buf[1024];
        GetPrivateProfileStringA(sec, key, def, buf, sizeof(buf), f);
        return std::string(buf);
    }
    static int I(const char* sec, const char* key, int def, const char* f) {
        return (int)GetPrivateProfileIntA(sec, key, def, f);
    }
    static float F(const char* sec, const char* key, float def, const char* f) {
        std::string s = S(sec, key, "", f);
        return s.empty() ? def : (float)atof(s.c_str());
    }
    static bool B(const char* sec, const char* key, bool def, const char* f) {
        return I(sec, key, def ? 1 : 0, f) != 0;
    }
};

} // namespace fiesta
#endif // SHINE_CLIENT_ENGINE_SHINECONFIG_H
