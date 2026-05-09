// Server/Shared/EHCrashHandler.cpp
#include "EHCrashHandler.h"
#include "ShineLogSystem.h"
#include <DbgHelp.h>
#include <stdio.h>
#pragma comment(lib, "DbgHelp.lib")

namespace fiesta {

char  EHCrashHandler::ms_szService[64] = {0};
char  EHCrashHandler::ms_szDumpDir[MAX_PATH] = {0};
void* EHCrashHandler::ms_pPrevFilter = NULL;

void EHCrashHandler::Install(const char* szSvc, const char* szDir) {
    strncpy_s(ms_szService, sizeof(ms_szService), szSvc ? szSvc : "shine", _TRUNCATE);
    strncpy_s(ms_szDumpDir, sizeof(ms_szDumpDir), szDir ? szDir : ".", _TRUNCATE);
    CreateDirectoryA(ms_szDumpDir, NULL);
    ms_pPrevFilter = (void*)SetUnhandledExceptionFilter(&EHCrashHandler::OnUnhandled);
}

void EHCrashHandler::Uninstall() {
    SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER)ms_pPrevFilter);
    ms_pPrevFilter = NULL;
}

LONG WINAPI EHCrashHandler::OnUnhandled(EXCEPTION_POINTERS* pkExc) {
    SYSTEMTIME st; GetLocalTime(&st);
    char szPath[MAX_PATH];
    sprintf_s(szPath, MAX_PATH, "%s\\%s_%04u%02u%02u_%02u%02u%02u.dmp",
              ms_szDumpDir, ms_szService,
              st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
    HANDLE hFile = CreateFileA(szPath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,
                               FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile != INVALID_HANDLE_VALUE) {
        MINIDUMP_EXCEPTION_INFORMATION mei;
        mei.ThreadId = GetCurrentThreadId();
        mei.ExceptionPointers = pkExc;
        mei.ClientPointers = FALSE;
        MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hFile,
                          MiniDumpNormal, &mei, NULL, NULL);
        CloseHandle(hFile);
    }
    SHINELOG_FATAL("Unhandled SEH 0x%08X at %p -- minidump %s",
                   pkExc->ExceptionRecord->ExceptionCode,
                   pkExc->ExceptionRecord->ExceptionAddress, szPath);
    return EXCEPTION_EXECUTE_HANDLER;
}

} // namespace fiesta
