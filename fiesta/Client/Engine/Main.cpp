// Client/Engine/Main.cpp
// WinMain: load ShineClient.ini via ShineConfig, build ClientConfig, run.
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "ClientApp.h"
#include "ShineConfig.h"
#include "../../Server/Shared/ShineLogSystem.h"

int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int) {
    // Resolve ini path beside the exe
    char szExeDir[MAX_PATH];
    GetModuleFileNameA(NULL, szExeDir, MAX_PATH);
    char* pSlash = strrchr(szExeDir, '\\');
    if (pSlash) *(pSlash + 1) = '\0';
    std::string kIni = std::string(szExeDir) + "ShineClient.ini";

    fiesta::ShineConfig& cfg = fiesta::ShineConfig::Get();
    cfg.Load(kIni.c_str());

    // Bridge ShineConfig into ClientConfig
    fiesta::ClientConfig cc;
    cc.kLoginIp    = cfg.kLoginIP;
    cc.uiLoginPort = cfg.uiLoginPort;
    cc.kUser       = cfg.kUser;
    cc.kPass       = cfg.kPass;
    cc.uiCharId    = cfg.uiCharId;
    cc.kAssetRoot  = cfg.kBaseDir;
    cc.uiWidth     = cfg.uiWidth;
    cc.uiHeight    = cfg.uiHeight;
    cc.bSkipLogin  = cfg.bSkipLogin;
    cc.kZoneIP     = cfg.kZoneIP;
    cc.uiZonePort  = cfg.uiZonePort;

    fiesta::ClientApp app;
    if (!app.Init(hInst, cc)) {
        MessageBoxA(NULL,
            "Shine failed to initialize.\nSee ShineClient.log for details.",
            "Shine", MB_OK | MB_ICONERROR);
        return 1;
    }

    int iRet = app.Run();
    app.Shutdown();
    return iRet;
}
