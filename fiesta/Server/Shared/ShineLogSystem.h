// Server/Shared/ShineLogSystem.h
// structured log writer. Every service uses this.
#ifndef SHINE_SHINELOGSYSTEM_H
#define SHINE_SHINELOGSYSTEM_H
#include "ShineTypes.h"
#include <stdio.h>

namespace shine {

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

#define SHINELOG_DEBUG(...) ::shine::ShineLogSystem::Get().Write(::shine::LOG_DEBUG, __VA_ARGS__)
#define SHINELOG_INFO(...)  ::shine::ShineLogSystem::Get().Write(::shine::LOG_INFO , __VA_ARGS__)
#define SHINELOG_WARN(...)  ::shine::ShineLogSystem::Get().Write(::shine::LOG_WARN , __VA_ARGS__)
#define SHINELOG_ERROR(...) ::shine::ShineLogSystem::Get().Write(::shine::LOG_ERROR, __VA_ARGS__)
#define SHINELOG_FATAL(...) ::shine::ShineLogSystem::Get().Write(::shine::LOG_FATAL, __VA_ARGS__)

} // namespace shine
#endif
