// Server/Shared/ShineLogSystem.h
// 01 Foundation -- structured log writer. Every service uses this.
// EVIDENCE: PDB_CONFIRMED  symbol: ShineLogSystem, EasyLogSys
#ifndef FIESTA_SHINELOGSYSTEM_H
#define FIESTA_SHINELOGSYSTEM_H
#include "ShineTypes.h"
#include <stdio.h>

namespace fiesta {

enum LogLevel { LOG_DEBUG = 0, LOG_INFO = 1, LOG_WARN = 2, LOG_ERROR = 3, LOG_FATAL = 4 };

class ShineLogSystem {
public:
    static ShineLogSystem& Get();
    void  Init(const char* szServiceName, const char* szLogDir);
    void  Shutdown();
    void  Write(LogLevel eLvl, const char* szFmt, ...);
    void  SetMinLevel(LogLevel e) { m_eMin = e; }
private:
    ShineLogSystem();
    ~ShineLogSystem();
    FILE*    m_pFile;
    LogLevel m_eMin;
    CRITICAL_SECTION m_kCs;
    char     m_szService[64];
};

#define SHINELOG_DEBUG(...) ::fiesta::ShineLogSystem::Get().Write(::fiesta::LOG_DEBUG, __VA_ARGS__)
#define SHINELOG_INFO(...)  ::fiesta::ShineLogSystem::Get().Write(::fiesta::LOG_INFO , __VA_ARGS__)
#define SHINELOG_WARN(...)  ::fiesta::ShineLogSystem::Get().Write(::fiesta::LOG_WARN , __VA_ARGS__)
#define SHINELOG_ERROR(...) ::fiesta::ShineLogSystem::Get().Write(::fiesta::LOG_ERROR, __VA_ARGS__)
#define SHINELOG_FATAL(...) ::fiesta::ShineLogSystem::Get().Write(::fiesta::LOG_FATAL, __VA_ARGS__)

} // namespace fiesta
#endif
