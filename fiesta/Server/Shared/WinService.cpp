// Server/Shared/WinService.cpp
#include "WinService.h"
#include "ShineLogSystem.h"
#include "EHCrashHandler.h"
#include <stdio.h>

namespace fiesta {

WinService* WinService::ms_pInst = NULL;

WinService::WinService(const char* szName)
    : m_hStatus(NULL), m_bStopRequested(0)
{
    strncpy_s(m_szName, sizeof(m_szName), szName ? szName : "ShineService", _TRUNCATE);
    ZeroMemory(&m_kStatus, sizeof(m_kStatus));
    m_kStatus.dwServiceType      = SERVICE_WIN32_OWN_PROCESS;
    m_kStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN;
    ms_pInst = this;
}
WinService::~WinService() { if (ms_pInst == this) ms_pInst = NULL; }

void WINAPI WinService::ServiceMainStatic(DWORD argc, LPSTR* argv) {
    if (ms_pInst) ms_pInst->ServiceMain(argc, argv);
}
DWORD WINAPI WinService::HandlerExStatic(DWORD ctrl, DWORD, LPVOID, LPVOID) {
    return ms_pInst ? ms_pInst->HandlerEx(ctrl) : NO_ERROR;
}
BOOL WINAPI WinService::ConsoleHandlerStatic(DWORD evt) {
    if ((evt == CTRL_C_EVENT || evt == CTRL_BREAK_EVENT || evt == CTRL_CLOSE_EVENT)
        && ms_pInst) {
        ms_pInst->RequestStop();
        return TRUE;
    }
    return FALSE;
}

void WinService::ReportStatus(DWORD dwCurrent, DWORD dwExitCode, DWORD dwWaitHint) {
    static DWORD dwCheck = 1;
    m_kStatus.dwCurrentState = dwCurrent;
    m_kStatus.dwWin32ExitCode = dwExitCode;
    m_kStatus.dwWaitHint = dwWaitHint;
    m_kStatus.dwCheckPoint = (dwCurrent == SERVICE_RUNNING || dwCurrent == SERVICE_STOPPED) ? 0 : dwCheck++;
    if (m_hStatus) SetServiceStatus(m_hStatus, &m_kStatus);
}

DWORD WinService::HandlerEx(DWORD ctrl) {
    switch (ctrl) {
        case SERVICE_CONTROL_STOP:
        case SERVICE_CONTROL_SHUTDOWN:
            ReportStatus(SERVICE_STOP_PENDING, NO_ERROR, 5000);
            RequestStop();
            return NO_ERROR;
        case SERVICE_CONTROL_INTERROGATE:
            return NO_ERROR;
    }
    return NO_ERROR;
}

void WinService::ServiceMain(DWORD, LPSTR*) {
    m_hStatus = RegisterServiceCtrlHandlerExA(m_szName, &WinService::HandlerExStatic, NULL);
    if (!m_hStatus) return;
    ReportStatus(SERVICE_START_PENDING, NO_ERROR, 5000);
    if (!OnStart()) { ReportStatus(SERVICE_STOPPED, ERROR_SERVICE_SPECIFIC_ERROR); return; }
    ReportStatus(SERVICE_RUNNING);
    while (!IsStopRequested()) { Sleep(1000); OnTick(); }
    OnStop();
    ReportStatus(SERVICE_STOPPED);
}

void WinService::RunInteractive() {
    SetConsoleCtrlHandler(&WinService::ConsoleHandlerStatic, TRUE);
    SHINELOG_INFO("[%s] interactive mode -- starting", m_szName);
    if (!OnStart()) { SHINELOG_FATAL("[%s] OnStart failed", m_szName); return; }
    while (!IsStopRequested()) { Sleep(1000); OnTick(); }
    OnStop();
}

int WinService::Run(int argc, char** argv) {
    EHCrashHandler::Install(m_szName, "MiniDumps");
    ShineLogSystem::Get().Init(m_szName, "Logs");

    bool bConsole = false;
    for (int i = 1; i < argc; ++i) if (argv[i] && _stricmp(argv[i], "-console") == 0) bConsole = true;

    if (bConsole) {
        RunInteractive();
    } else {
        SERVICE_TABLE_ENTRYA aSvc[2] = { {0}, {0} };
        aSvc[0].lpServiceName = m_szName;
        aSvc[0].lpServiceProc = &WinService::ServiceMainStatic;
        if (!StartServiceCtrlDispatcherA(aSvc)) {
            DWORD err = GetLastError();
            if (err == ERROR_FAILED_SERVICE_CONTROLLER_CONNECT) RunInteractive();
            else SHINELOG_FATAL("StartServiceCtrlDispatcherA err=%u", err);
        }
    }
    EHCrashHandler::Uninstall();
    ShineLogSystem::Get().Shutdown();
    return 0;
}

} // namespace fiesta
