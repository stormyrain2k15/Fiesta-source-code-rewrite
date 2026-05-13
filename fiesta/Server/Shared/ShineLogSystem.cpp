// Server/Shared/ShineLogSystem.cpp
#include "ShineLogSystem.h"
#include <stdarg.h>
#include <time.h>
#include <string.h>

namespace shine {

static const char* LvlName(LogLevel e) {
    switch (e) {
        case LOG_DEBUG: return "DBG";
        case LOG_INFO:  return "INF";
        case LOG_WARN:  return "WRN";
        case LOG_ERROR: return "ERR";
        case LOG_FATAL: return "FTL";
    }
    return "?";
}

ShineLogSystem& ShineLogSystem::Get() { static ShineLogSystem s; return s; }

ShineLogSystem::ShineLogSystem() : m_pFile(NULL), m_eMin(LOG_INFO) {
    InitializeCriticalSection(&m_kCs);
    m_szService[0] = 0;
}

ShineLogSystem::~ShineLogSystem() {
    Shutdown();
    DeleteCriticalSection(&m_kCs);
}

void ShineLogSystem::Init(const char* szServiceName, const char* szLogDir) {
    EnterCriticalSection(&m_kCs);
    if (m_pFile) { fclose(m_pFile); m_pFile = NULL; }
    strncpy_s(m_szService, sizeof(m_szService), szServiceName ? szServiceName : "shine", _TRUNCATE);
    char szPath[MAX_PATH];
    SYSTEMTIME st; GetLocalTime(&st);
    if (szLogDir && *szLogDir) CreateDirectoryA(szLogDir, NULL);
    sprintf_s(szPath, MAX_PATH, "%s\\%s_%04u%02u%02u.log",
              (szLogDir && *szLogDir) ? szLogDir : ".", m_szService,
              st.wYear, st.wMonth, st.wDay);
    fopen_s(&m_pFile, szPath, "ab");
    LeaveCriticalSection(&m_kCs);
}

void ShineLogSystem::Shutdown() {
    EnterCriticalSection(&m_kCs);
    if (m_pFile) { fclose(m_pFile); m_pFile = NULL; }
    LeaveCriticalSection(&m_kCs);
}

void ShineLogSystem::Write(LogLevel eLvl, const char* szFmt, ...) {
    if (eLvl < m_eMin) return;
    SYSTEMTIME st; GetLocalTime(&st);
    char szLine[2048];
    int n = sprintf_s(szLine, sizeof(szLine), "[%02u:%02u:%02u.%03u %s %s] ",
                      st.wHour, st.wMinute, st.wSecond, st.wMilliseconds,
                      LvlName(eLvl), m_szService);
    if (n < 0) return;
    va_list ap; va_start(ap, szFmt);
    int m = _vsnprintf_s(szLine + n, sizeof(szLine) - n, _TRUNCATE, szFmt, ap);
    va_end(ap);
    int total = n + (m > 0 ? m : 0);
    if (total < (int)sizeof(szLine) - 2) { szLine[total++] = '\r'; szLine[total++] = '\n'; }
    EnterCriticalSection(&m_kCs);
    if (m_pFile) { fwrite(szLine, 1, total, m_pFile); fflush(m_pFile); }
    LeaveCriticalSection(&m_kCs);
    OutputDebugStringA(szLine);
}

} // namespace shine
