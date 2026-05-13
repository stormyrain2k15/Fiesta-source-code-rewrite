// Client/Engine/MachineOpt.cpp
#include "MachineOpt.h"
#include "../../Server/Shared/ShineLogSystem.h"
#include <cstdio>

namespace shine {

MachineOpt& MachineOpt::Get() { static MachineOpt s; return s; }

void MachineOpt::Init(const std::string& rCfgPath) {
    m_kCfgPath = rCfgPath;
    Load();
    SHINELOG_INFO("MachineOpt: init from '%s'", rCfgPath.c_str());
}

void MachineOpt::Load() {
    const char* f = m_kCfgPath.c_str();
    if (GetFileAttributesA(f) == INVALID_FILE_ATTRIBUTES) {
        SHINELOG_INFO("MachineOpt: no cfg -- using defaults");
        return;
    }

    // [Graphic]
    m_kGraphic.uiDisplayWidth   = (uint32)II("Graphic","Width",      1280, f);
    m_kGraphic.uiDisplayHeight  = (uint32)II("Graphic","Height",      720, f);
    m_kGraphic.uiBitsPerPixel   = (uint32)II("Graphic","BPP",          32, f);
    m_kGraphic.bFullscreen      = II("Graphic","Fullscreen",             0, f) != 0;
    m_kGraphic.bVSync           = II("Graphic","VSync",                  1, f) != 0;
    m_kGraphic.uiTextureQuality = (uint32)II("Graphic","TextureQuality", 2, f);
    m_kGraphic.uiShadowQuality  = (uint32)II("Graphic","ShadowQuality",  1, f);
    m_kGraphic.fDrawDistance    = FI("Graphic","DrawDistance",        1.0f, f);
    m_kGraphic.bShowGrass       = II("Graphic","ShowGrass",              1, f) != 0;
    m_kGraphic.bShowWeather     = II("Graphic","ShowWeather",            1, f) != 0;
    m_kGraphic.uiAntiAlias      = (uint32)II("Graphic","AntiAlias",      0, f);

    // [Sound]
    m_kSound.bEnableSound       = II("Sound","EnableSound",  1, f) != 0;
    m_kSound.bEnableBGM         = II("Sound","EnableBGM",    1, f) != 0;
    m_kSound.bEnable3DSound     = II("Sound","Enable3D",     1, f) != 0;
    m_kSound.uiMasterVolume     = (uint32)II("Sound","MasterVolume", 80, f);
    m_kSound.uiBGMVolume        = (uint32)II("Sound","BGMVolume",    70, f);
    m_kSound.uiSFXVolume        = (uint32)II("Sound","SFXVolume",    85, f);
    m_kSound.uiVoiceVolume      = (uint32)II("Sound","VoiceVolume",  90, f);

    // [Game]
    m_kGame.bShowDamageNumbers  = II("Game","ShowDamageNumbers", 1, f) != 0;
    m_kGame.bShowPlayerNames    = II("Game","ShowPlayerNames",   1, f) != 0;
    m_kGame.bShowMobNames       = II("Game","ShowMobNames",      1, f) != 0;
    m_kGame.bShowNPCNames       = II("Game","ShowNPCNames",      1, f) != 0;
    m_kGame.bShowHPBars         = II("Game","ShowHPBars",        1, f) != 0;
    m_kGame.bAutoPickup         = II("Game","AutoPickup",        0, f) != 0;
    m_kGame.bSimpleSkillEffect  = II("Game","SimpleSkillEffect", 0, f) != 0;
    m_kGame.uiChatFontSize      = (uint32)II("Game","ChatFontSize",  12, f);
    m_kGame.bShowClock          = II("Game","ShowClock",         1, f) != 0;
}

void MachineOpt::Save() {
    FILE* fp = NULL;
    fopen_s(&fp, m_kCfgPath.c_str(), "w");
    if (!fp) {
        SHINELOG_WARN("MachineOpt::Save: cannot write '%s'", m_kCfgPath.c_str());
        return;
    }

    fprintf(fp, "; ShineOption.cfg -- machine and game options\n\n");

    fprintf(fp, "[Graphic]\n");
    fprintf(fp, "Width=%u\n",           m_kGraphic.uiDisplayWidth);
    fprintf(fp, "Height=%u\n",          m_kGraphic.uiDisplayHeight);
    fprintf(fp, "BPP=%u\n",             m_kGraphic.uiBitsPerPixel);
    fprintf(fp, "Fullscreen=%d\n",      m_kGraphic.bFullscreen ? 1 : 0);
    fprintf(fp, "VSync=%d\n",           m_kGraphic.bVSync ? 1 : 0);
    fprintf(fp, "TextureQuality=%u\n",  m_kGraphic.uiTextureQuality);
    fprintf(fp, "ShadowQuality=%u\n",   m_kGraphic.uiShadowQuality);
    fprintf(fp, "DrawDistance=%.2f\n",  m_kGraphic.fDrawDistance);
    fprintf(fp, "ShowGrass=%d\n",       m_kGraphic.bShowGrass ? 1 : 0);
    fprintf(fp, "ShowWeather=%d\n",     m_kGraphic.bShowWeather ? 1 : 0);
    fprintf(fp, "AntiAlias=%u\n\n",     m_kGraphic.uiAntiAlias);

    fprintf(fp, "[Sound]\n");
    fprintf(fp, "EnableSound=%d\n",     m_kSound.bEnableSound ? 1 : 0);
    fprintf(fp, "EnableBGM=%d\n",       m_kSound.bEnableBGM ? 1 : 0);
    fprintf(fp, "Enable3D=%d\n",        m_kSound.bEnable3DSound ? 1 : 0);
    fprintf(fp, "MasterVolume=%u\n",    m_kSound.uiMasterVolume);
    fprintf(fp, "BGMVolume=%u\n",       m_kSound.uiBGMVolume);
    fprintf(fp, "SFXVolume=%u\n",       m_kSound.uiSFXVolume);
    fprintf(fp, "VoiceVolume=%u\n\n",   m_kSound.uiVoiceVolume);

    fprintf(fp, "[Game]\n");
    fprintf(fp, "ShowDamageNumbers=%d\n", m_kGame.bShowDamageNumbers ? 1 : 0);
    fprintf(fp, "ShowPlayerNames=%d\n",   m_kGame.bShowPlayerNames   ? 1 : 0);
    fprintf(fp, "ShowMobNames=%d\n",      m_kGame.bShowMobNames      ? 1 : 0);
    fprintf(fp, "ShowNPCNames=%d\n",      m_kGame.bShowNPCNames      ? 1 : 0);
    fprintf(fp, "ShowHPBars=%d\n",        m_kGame.bShowHPBars        ? 1 : 0);
    fprintf(fp, "AutoPickup=%d\n",        m_kGame.bAutoPickup        ? 1 : 0);
    fprintf(fp, "SimpleSkillEffect=%d\n", m_kGame.bSimpleSkillEffect ? 1 : 0);
    fprintf(fp, "ChatFontSize=%u\n",      m_kGame.uiChatFontSize);
    fprintf(fp, "ShowClock=%d\n",         m_kGame.bShowClock         ? 1 : 0);

    fclose(fp);
    m_bDirty = false;
    SHINELOG_INFO("MachineOpt: saved to '%s'", m_kCfgPath.c_str());
}

void MachineOpt::ResetToDefault() {
    m_kGraphic = GraphicOption();
    m_kSound   = SoundOption();
    m_kGame    = GameOption();
    m_bDirty   = true;
    Save();
    SHINELOG_INFO("MachineOpt: reset to defaults");
}

} // namespace shine
