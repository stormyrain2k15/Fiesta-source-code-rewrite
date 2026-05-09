// Server/Shared/WinService.h
// 00 Bootstrap -- Windows Service host base. Each service exe subclasses this.
// EVIDENCE: PDB_CONFIRMED  symbol: WinService, ServerInfo
#ifndef FIESTA_WINSERVICE_H
#define FIESTA_WINSERVICE_H
#include "ShineTypes.h"

namespace fiesta {

class WinService {
public:
    explicit WinService(const char* szServiceName);
    virtual ~WinService();

    // Top-level entry point invoked from each service's main(int,char**).
    int  Run(int argc, char** argv);

    // Subclasses override these.
    virtual bool OnStart()   = 0;     // called after SCM start
    virtual void OnStop()    = 0;     // called on SCM stop / Ctrl-C in console mode
    virtual void OnTick()    {}       // optional periodic tick (1s)

    static WinService* GetInstance() { return ms_pInst; }
    bool   IsStopRequested() const { return m_bStopRequested != 0; }
    void   RequestStop()           { InterlockedExchange(&m_bStopRequested, 1); }
    const char* Name() const { return m_szName; }

protected:
    char m_szName[64];
private:
    static void  WINAPI ServiceMainStatic(DWORD argc, LPSTR* argv);
    static DWORD WINAPI HandlerExStatic(DWORD ctrl, DWORD evt, LPVOID data, LPVOID ctx);
    static BOOL  WINAPI ConsoleHandlerStatic(DWORD evt);

    void  ServiceMain(DWORD argc, LPSTR* argv);
    DWORD HandlerEx(DWORD ctrl);
    void  RunInteractive();
    void  ReportStatus(DWORD dwCurrent, DWORD dwExitCode = NO_ERROR, DWORD dwWaitHint = 0);

    SERVICE_STATUS_HANDLE m_hStatus;
    SERVICE_STATUS        m_kStatus;
    LONG                  m_bStopRequested;
    static WinService*    ms_pInst;
};

} // namespace fiesta
#endif
