// Server/Shared/EHCrashHandler.h
// structured-exception handler with minidump writer.
#ifndef SHINE_EHCRASHHANDLER_H
#define SHINE_EHCRASHHANDLER_H
#include "ShineTypes.h"

namespace shine {

class EHCrashHandler {
public:
    static void Install(const char* szServiceName, const char* szDumpDir);
    static void Uninstall();
    static LONG WINAPI OnUnhandled(struct _EXCEPTION_POINTERS* pkExc);
private:
    static char  ms_szService[64];
    static char  ms_szDumpDir[MAX_PATH];
    static void* ms_pPrevFilter;
};

} // namespace shine
#endif
