// Client/Engine/MachineOpt.h
// Machine and graphics options -- mirrors the original MachineOpt system.
// Reads/writes ShineOption.cfg beside the exe.
// Separate from ShineConfig (server/path config) -- this owns everything
// the player can change in the options screen: resolution, fullscreen,
// sound volumes, draw distance, shadow quality etc.
//
// Save file: ShineOption.cfg (INI format)
#ifndef SHINE_CLIENT_ENGINE_MACHINEOPT_H
#define SHINE_CLIENT_ENGINE_MACHINEOPT_H

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <string>
#include "../../Server/Shared/ShineTypes.h"

namespace shine {

class MachineOpt {
public:
    // ── Graphics options ──────────────────────────────────────────────────
    struct GraphicOption {
        uint32  uiDisplayWidth;
        uint32  uiDisplayHeight;
        uint32  uiBitsPerPixel;     // 16 or 32
        bool    bFullscreen;
        bool    bVSync;
        uint32  uiTextureQuality;   // 0=low 1=medium 2=high
        uint32  uiShadowQuality;    // 0=off 1=low 2=high
        float   fDrawDistance;      // world draw distance multiplier
        bool    bShowGrass;
        bool    bShowWeather;
        uint32  uiAntiAlias;        // 0=off 1=2x 2=4x

        GraphicOption()
            : uiDisplayWidth(1280), uiDisplayHeight(720),
              uiBitsPerPixel(32), bFullscreen(false), bVSync(true),
              uiTextureQuality(2), uiShadowQuality(1),
              fDrawDistance(1.0f), bShowGrass(true), bShowWeather(true),
              uiAntiAlias(0) {}
    };

    // ── Sound options ─────────────────────────────────────────────────────
    struct SoundOption {
        bool    bEnableSound;
        bool    bEnableBGM;
        bool    bEnable3DSound;
        uint32  uiMasterVolume;     // 0-100
        uint32  uiBGMVolume;        // 0-100
        uint32  uiSFXVolume;        // 0-100
        uint32  uiVoiceVolume;      // 0-100 (TTS voice)

        SoundOption()
            : bEnableSound(true), bEnableBGM(true), bEnable3DSound(true),
              uiMasterVolume(80), uiBGMVolume(70),
              uiSFXVolume(85), uiVoiceVolume(90) {}
    };

    // ── Game options ──────────────────────────────────────────────────────
    struct GameOption {
        bool    bShowDamageNumbers;
        bool    bShowPlayerNames;
        bool    bShowMobNames;
        bool    bShowNPCNames;
        bool    bShowHPBars;
        bool    bAutoPickup;
        bool    bSimpleSkillEffect;  // reduce particle counts on low-end
        uint32  uiChatFontSize;
        bool    bShowClock;

        GameOption()
            : bShowDamageNumbers(true), bShowPlayerNames(true),
              bShowMobNames(true), bShowNPCNames(true),
              bShowHPBars(true), bAutoPickup(false),
              bSimpleSkillEffect(false), uiChatFontSize(12),
              bShowClock(true) {}
    };

    // ── Singleton ─────────────────────────────────────────────────────────
    static MachineOpt& Get();

    void Init(const std::string& rCfgPath);
    void Save();
    void ResetToDefault();

    GraphicOption& Graphic() { return m_kGraphic; }
    SoundOption&   Sound()   { return m_kSound; }
    GameOption&    Game()    { return m_kGame; }

    const GraphicOption& GetGraphicOption() const { return m_kGraphic; }
    const SoundOption&   GetSoundOption()   const { return m_kSound; }
    const GameOption&    GetGameOption()    const { return m_kGame; }

    bool IsFullscreen()    const { return m_kGraphic.bFullscreen; }
    uint32 GetWidth()      const { return m_kGraphic.uiDisplayWidth; }
    uint32 GetHeight()     const { return m_kGraphic.uiDisplayHeight; }

    bool IsDirty() const { return m_bDirty; }
    void SetDirty()      { m_bDirty = true; }

private:
    MachineOpt() : m_bDirty(false) {}
    void Load();

    static std::string SI(const char* s, const char* k, const char* d, const char* f) {
        char buf[256]; GetPrivateProfileStringA(s,k,d,buf,sizeof(buf),f);
        return std::string(buf);
    }
    static int II(const char* s, const char* k, int d, const char* f) {
        return (int)GetPrivateProfileIntA(s,k,d,f);
    }
    static float FI(const char* s, const char* k, float d, const char* f) {
        std::string v = SI(s,k,"",f);
        return v.empty() ? d : (float)atof(v.c_str());
    }

    GraphicOption   m_kGraphic;
    SoundOption     m_kSound;
    GameOption      m_kGame;
    std::string     m_kCfgPath;
    bool            m_bDirty;
};

} // namespace shine
#endif // SHINE_CLIENT_ENGINE_MACHINEOPT_H
