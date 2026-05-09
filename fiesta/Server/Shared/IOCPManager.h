// Server/Shared/IOCPManager.h
// IO completion port worker pool.
#ifndef FIESTA_IOCPMANAGER_H
#define FIESTA_IOCPMANAGER_H
#include "ShineTypes.h"
#include <vector>

namespace fiesta {

class IOCPSession;

class IOCPManager {
public:
    IOCPManager();
    ~IOCPManager();

    bool   Start(uint32 uiWorkerThreads = 0);
    void   Stop();
    bool   Bind(IOCPSession* pkSess);
    HANDLE GetPort() const { return m_hPort; }
private:
    static unsigned __stdcall WorkerStatic(void* p);
    void   WorkerLoop();

    HANDLE              m_hPort;
    std::vector<HANDLE> m_kWorkers;
    LONG                m_bRun;
};

} // namespace fiesta
#endif
