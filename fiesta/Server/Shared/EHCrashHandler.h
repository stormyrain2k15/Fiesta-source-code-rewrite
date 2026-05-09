// Server/Shared/EHCrashHandler.h
// 00 Bootstrap -- structured-exception handler with minidump writer.
// EVIDENCE: PDB_CONFIRMED  symbol: EHCrashHandler, MiniDump
#ifndef FIESTA_EHCRASHHANDLER_H
#define FIESTA_EHCRASHHANDLER_H
#include "ShineTypes.h"

namespace fiesta {

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

} // namespace fiesta
#endif
