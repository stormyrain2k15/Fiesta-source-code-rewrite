// Client/Engine/Main.cpp
// WinMain -- load config, init Gamebryo via NiApplication::Create().
// Matches the original client pattern:
//   NiApplication::Create() returns ShineApp*
//   NiApplication::Run() drives the message loop
//   Everything else flows through ShineFrameWorkMgr
#include "ShineApp.h"
#include "ShineConfig.h"
#include "MachineOpt.h"
#include "../Input/KeyMap.h"
#include "../UI/UILayout.h"
#include "../../Server/Shared/ShineLogSystem.h"
#include <NiSystem.h>

int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int) {
    // ── Resolve exe directory ─────────────────────────────────────────────
    char szExeDir[MAX_PATH];
    GetModuleFileNameA(NULL, szExeDir, MAX_PATH);
    char* pSlash = strrchr(szExeDir, '\\');
    if (pSlash) *(pSlash+1) = '\0';
    std::string kExeDir(szExeDir);

    // ── Init logging ──────────────────────────────────────────────────────
    // splog_init equivalent -- just write to ShineClient.log for now
    SHINELOG_INFO("Shine client starting");

    // ── Load configs ──────────────────────────────────────────────────────
    shine::ShineConfig::Get().Load((kExeDir + "ShineClient.ini").c_str());
    shine::MachineOpt::Get().Init(kExeDir + "ShineOption.cfg");
    shine::KeyMap::Get().Init(kExeDir + "ShineKeys.cfg");
    shine::UILayout::Get().Init(kExeDir + "ShineUI.cfg");

    // ── Boot Gamebryo ─────────────────────────────────────────────────────
    // NiApplication::Run() calls NiApplication::Create() internally,
    // which calls our ShineApp factory.
    NiInit();
    int iRet = NiApplication::Run(hInst);
    NiShutdown();

    SHINELOG_INFO("Shine client exit code %d", iRet);
    return iRet;
}
