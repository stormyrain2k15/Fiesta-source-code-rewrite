// Client/Sound/ShineSoundMgr.cpp
// Miles Sound System runtime-loaded wrapper.
// Loads mss32.dll at runtime so the exe doesn't hard-depend on Miles.
// If Miles isn't present everything silently falls back to no-op.
#include "ShineSoundMgr.h"
#include "../Engine/MachineOpt.h"
#include "../../Server/Shared/ShineLogSystem.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

namespace shine {

// Miles function pointer signatures (subset we use)
struct MilesAPI {
    HMODULE hDLL;

    // Driver
    typedef HDIGDRIVER (__stdcall* FnOpenDriver)(uint32, int32, int32);
    typedef void       (__stdcall* FnCloseDriver)(HDIGDRIVER);
    // Stream
    typedef HSTREAM    (__stdcall* FnOpenStream)(HDIGDRIVER, const char*, uint32);
    typedef void       (__stdcall* FnCloseStream)(HSTREAM);
    typedef void       (__stdcall* FnStartStream)(HSTREAM);
    typedef void       (__stdcall* FnStopStream)(HSTREAM);
    typedef void       (__stdcall* FnPauseStream)(HSTREAM, int32);
    typedef void       (__stdcall* FnSetStreamVolume)(HSTREAM, int32);
    // Sample
    typedef HSAMPLE    (__stdcall* FnAllocSample)(HDIGDRIVER);
    typedef void       (__stdcall* FnFreeSample)(HSAMPLE);
    typedef int32      (__stdcall* FnLoadFile)(HSAMPLE, const char*);
    typedef void       (__stdcall* FnStartSample)(HSAMPLE);
    typedef void       (__stdcall* FnStopSample)(HSAMPLE);
    typedef void       (__stdcall* FnSetSampleVolume)(HSAMPLE, int32);
    // Master
    typedef void       (__stdcall* FnSetDigitalMaster)(HDIGDRIVER, int32);

    FnOpenDriver        OpenDriver;
    FnCloseDriver       CloseDriver;
    FnOpenStream        OpenStream;
    FnCloseStream       CloseStream;
    FnStartStream       StartStream;
    FnStopStream        StopStream;
    FnPauseStream       PauseStream;
    FnSetStreamVolume   SetStreamVolume;
    FnAllocSample       AllocSample;
    FnFreeSample        FreeSample;
    FnLoadFile          LoadFile;
    FnStartSample       StartSample;
    FnStopSample        StopSample;
    FnSetSampleVolume   SetSampleVolume;
    FnSetDigitalMaster  SetDigitalMaster;

    bool loaded = false;
};

ShineSoundMgr& ShineSoundMgr::Get() { static ShineSoundMgr s; return s; }

ShineSoundMgr::ShineSoundMgr()
    : m_bInitialized(false), m_bHeld(false),
      m_hDriver(NULL), m_hBGMStream(NULL),
      m_uiMasterVol(80), m_uiBGMVol(70), m_uiSFXVol(85),
      m_pkAPI(NULL)
{}

ShineSoundMgr::~ShineSoundMgr() { Shutdown(); }

bool ShineSoundMgr::Init(const std::string& rMilesDir) {
    m_pkAPI = new MilesAPI();
    LoadMilesDLL(rMilesDir);

    if (!m_pkAPI->loaded) {
        SHINELOG_WARN("ShineSoundMgr: Miles not available -- running silent");
        return false;  // non-fatal
    }

    m_hDriver = m_pkAPI->OpenDriver(44100, -1, 0);
    if (!m_hDriver) {
        SHINELOG_WARN("ShineSoundMgr: AIL_Open failed -- running silent");
        return false;
    }

    ApplyOptions();
    m_bInitialized = true;
    SHINELOG_INFO("ShineSoundMgr: initialized");
    return true;
}

void ShineSoundMgr::Shutdown() {
    StopBGM();
    StopAllSFX();
    if (m_pkAPI && m_pkAPI->loaded && m_hDriver) {
        m_pkAPI->CloseDriver(m_hDriver);
        FreeLibrary(m_pkAPI->hDLL);
    }
    delete m_pkAPI;
    m_pkAPI = NULL;
    m_bInitialized = false;
}

void ShineSoundMgr::LoadMilesDLL(const std::string& rMilesDir) {
    std::string kDLL = rMilesDir + "\\mss32.dll";
    m_pkAPI->hDLL = LoadLibraryA(kDLL.c_str());
    if (!m_pkAPI->hDLL) {
        SHINELOG_WARN("ShineSoundMgr: mss32.dll not found at '%s'", kDLL.c_str());
        return;
    }

    // Load function pointers -- names match Miles 6.x/7.x exports
#define MILES_PROC(type, name, export) \
    m_pkAPI->name = (MilesAPI::type)GetProcAddress(m_pkAPI->hDLL, export); \
    if (!m_pkAPI->name) { SHINELOG_WARN("Miles: missing " export); return; }

    MILES_PROC(FnOpenDriver,       OpenDriver,       "AIL_waveOutOpen")
    MILES_PROC(FnCloseDriver,      CloseDriver,      "AIL_waveOutClose")
    MILES_PROC(FnOpenStream,       OpenStream,       "AIL_open_stream")
    MILES_PROC(FnCloseStream,      CloseStream,      "AIL_close_stream")
    MILES_PROC(FnStartStream,      StartStream,      "AIL_start_stream")
    MILES_PROC(FnStopStream,       StopStream,       "AIL_stop_stream")
    MILES_PROC(FnPauseStream,      PauseStream,      "AIL_pause_stream")
    MILES_PROC(FnSetStreamVolume,  SetStreamVolume,  "AIL_set_stream_volume")
    MILES_PROC(FnAllocSample,      AllocSample,      "AIL_allocate_sample_handle")
    MILES_PROC(FnFreeSample,       FreeSample,       "AIL_release_sample_handle")
    MILES_PROC(FnLoadFile,         LoadFile,         "AIL_load_sample_buffer")
    MILES_PROC(FnStartSample,      StartSample,      "AIL_start_sample")
    MILES_PROC(FnStopSample,       StopSample,       "AIL_stop_sample")
    MILES_PROC(FnSetSampleVolume,  SetSampleVolume,  "AIL_set_sample_volume")
    MILES_PROC(FnSetDigitalMaster, SetDigitalMaster, "AIL_digital_master_volume")
#undef MILES_PROC

    m_pkAPI->loaded = true;
    SHINELOG_INFO("ShineSoundMgr: mss32.dll loaded");
}

// ── BGM ───────────────────────────────────────────────────────────────────────

void ShineSoundMgr::PlayBGM(const std::string& rPath) {
    if (!m_bInitialized || m_bHeld) return;
    if (m_kCurrentBGM == rPath) return; // already playing

    StopBGM();
    m_hBGMStream = m_pkAPI->OpenStream(m_hDriver, rPath.c_str(), 0);
    if (!m_hBGMStream) {
        SHINELOG_WARN("ShineSoundMgr: cannot open BGM '%s'", rPath.c_str());
        return;
    }
    m_pkAPI->SetStreamVolume(m_hBGMStream, ScaleVolume(m_uiBGMVol));
    m_pkAPI->StartStream(m_hBGMStream);
    m_kCurrentBGM = rPath;
    SHINELOG_DEBUG("ShineSoundMgr: BGM '%s'", rPath.c_str());
}

void ShineSoundMgr::StopBGM() {
    if (!m_bInitialized || !m_hBGMStream) return;
    m_pkAPI->StopStream(m_hBGMStream);
    m_pkAPI->CloseStream(m_hBGMStream);
    m_hBGMStream = NULL;
    m_kCurrentBGM.clear();
}

void ShineSoundMgr::PauseBGM(bool bPause) {
    if (!m_bInitialized || !m_hBGMStream) return;
    m_pkAPI->PauseStream(m_hBGMStream, bPause ? 1 : 0);
}

// ── SFX ───────────────────────────────────────────────────────────────────────

void ShineSoundMgr::PlaySFX(const std::string& rPath) {
    if (!m_bInitialized || m_bHeld) return;

    std::map<std::string,HSAMPLE>::iterator it = m_kSFXCache.find(rPath);
    HSAMPLE hSample = NULL;

    if (it != m_kSFXCache.end()) {
        hSample = it->second;
    } else {
        hSample = m_pkAPI->AllocSample(m_hDriver);
        if (!hSample) return;
        if (!m_pkAPI->LoadFile(hSample, rPath.c_str())) {
            m_pkAPI->FreeSample(hSample);
            SHINELOG_WARN("ShineSoundMgr: cannot load SFX '%s'", rPath.c_str());
            return;
        }
        m_kSFXCache[rPath] = hSample;
    }

    m_pkAPI->SetSampleVolume(hSample, ScaleVolume(m_uiSFXVol));
    m_pkAPI->StartSample(hSample);
}

void ShineSoundMgr::StopAllSFX() {
    if (!m_bInitialized) return;
    for (std::map<std::string,HSAMPLE>::iterator it = m_kSFXCache.begin();
         it != m_kSFXCache.end(); ++it) {
        if (it->second) {
            m_pkAPI->StopSample(it->second);
            m_pkAPI->FreeSample(it->second);
        }
    }
    m_kSFXCache.clear();
}

// ── Volume / options ──────────────────────────────────────────────────────────

void ShineSoundMgr::SetMasterVolume(uint32 uiVol) {
    m_uiMasterVol = uiVol;
    if (m_bInitialized)
        m_pkAPI->SetDigitalMaster(m_hDriver, ScaleVolume(uiVol));
}

void ShineSoundMgr::SetBGMVolume(uint32 uiVol) {
    m_uiBGMVol = uiVol;
    if (m_bInitialized && m_hBGMStream)
        m_pkAPI->SetStreamVolume(m_hBGMStream, ScaleVolume(uiVol));
}

void ShineSoundMgr::SetSFXVolume(uint32 uiVol) { m_uiSFXVol = uiVol; }

void ShineSoundMgr::ApplyOptions() {
    const MachineOpt::SoundOption& s = MachineOpt::Get().GetSoundOption();
    if (!s.bEnableSound) { Shutdown(); return; }
    SetMasterVolume(s.uiMasterVolume);
    SetBGMVolume   (s.uiBGMVolume);
    SetSFXVolume   (s.uiSFXVolume);
    if (!s.bEnableBGM) StopBGM();
}

void ShineSoundMgr::HoldSound(bool bHold) {
    // Called from WM_ACTIVATE -- matches original SoundMgr::HoldSound
    m_bHeld = bHold;
    if (bHold) PauseBGM(true);
    else       PauseBGM(false);
}

int ShineSoundMgr::ScaleVolume(uint32 uiPct) const {
    // Miles volume: 0-127
    return (int)(uiPct * 127 / 100);
}

} // namespace shine
