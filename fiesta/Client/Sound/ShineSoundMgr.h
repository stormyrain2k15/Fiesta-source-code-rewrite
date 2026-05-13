// Client/Sound/ShineSoundMgr.h
// Miles Sound System wrapper -- matches original SoundMgr interface.
// Miles dlls must be in the miles/ directory (from MilesDir in ShineClient.ini).
// Handles BGM streaming (mp3) and SFX playback (.wav).
// Respects MachineOpt sound settings.
#ifndef SHINE_CLIENT_SOUND_SHINESOUNDMGR_H
#define SHINE_CLIENT_SOUND_SHINESOUNDMGR_H

#include "../../Server/Shared/ShineTypes.h"
#include <string>
#include <map>

// Miles Sound System forward declarations
// Avoids including mss.h here -- only included in the .cpp
typedef void* HSAMPLE;
typedef void* HSTREAM;
typedef void* HDIGDRIVER;

namespace shine {

class ShineSoundMgr {
public:
    static ShineSoundMgr& Get();

    // Call at boot. rMilesDir = path to miles\ directory.
    bool Init(const std::string& rMilesDir);
    void Shutdown();

    // BGM -- streams an mp3 from ressound\
    void PlayBGM  (const std::string& rPath);
    void StopBGM  ();
    void PauseBGM (bool bPause);   // called on WM_ACTIVATE/DEACTIVATE

    // SFX -- fire and forget (short .wav)
    void PlaySFX  (const std::string& rPath);
    void StopAllSFX();

    // Volume (0-100)
    void SetMasterVolume (uint32 uiVol);
    void SetBGMVolume    (uint32 uiVol);
    void SetSFXVolume    (uint32 uiVol);

    // Sync volumes from MachineOpt
    void ApplyOptions();

    bool IsInitialized() const { return m_bInitialized; }

    // Original SoundMgr interface -- called by OnWindowActivate
    void HoldSound(bool bHold);

private:
    ShineSoundMgr();
    ~ShineSoundMgr();

    bool            m_bInitialized;
    bool            m_bHeld;
    HDIGDRIVER      m_hDriver;
    HSTREAM         m_hBGMStream;
    std::string     m_kCurrentBGM;
    uint32          m_uiMasterVol;
    uint32          m_uiBGMVol;
    uint32          m_uiSFXVol;

    // SFX cache -- path → sample handle
    std::map<std::string, HSAMPLE> m_kSFXCache;

    // Miles dll function pointers (loaded at runtime)
    struct MilesAPI* m_pkAPI;

    void LoadMilesDLL(const std::string& rMilesDir);
    int  ScaleVolume(uint32 uiPct) const; // 0-100 → Miles 0-127
};

// Global shorthand matching original codebase
#define g_kSoundMgr shine::ShineSoundMgr::Get()

} // namespace shine
#endif // SHINE_CLIENT_SOUND_SHINESOUNDMGR_H
