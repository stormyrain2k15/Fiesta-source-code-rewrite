// Server/Shared/IOCPManager.cpp
#include "IOCPManager.h"
#include "Socket_Acceptor.h"
#include "ShineLogSystem.h"
#include <process.h>

namespace fiesta {

// Per-IO context overlay. We tag pkSess via lpCompletionKey.
IOCPManager::IOCPManager() : m_hPort(NULL), m_bRun(0) {}
IOCPManager::~IOCPManager() { Stop(); }

bool IOCPManager::Start(uint32 uiWorkers) {
    WSADATA wsa; if (WSAStartup(MAKEWORD(2,2), &wsa) != 0) return false;
    m_hPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
    if (!m_hPort) return false;
    InterlockedExchange(&m_bRun, 1);
    SYSTEM_INFO si; GetSystemInfo(&si);
    if (uiWorkers == 0) uiWorkers = si.dwNumberOfProcessors * 2;
    for (uint32 i = 0; i < uiWorkers; ++i) {
        unsigned tid; HANDLE h = (HANDLE)_beginthreadex(NULL, 0, &WorkerStatic, this, 0, &tid);
        if (h) m_kWorkers.push_back(h);
    }
    SHINELOG_INFO("IOCP started with %u worker(s)", (uint32)m_kWorkers.size());
    return true;
}

void IOCPManager::Stop() {
    if (!m_hPort) return;
    InterlockedExchange(&m_bRun, 0);
    for (size_t i = 0; i < m_kWorkers.size(); ++i)
        PostQueuedCompletionStatus(m_hPort, 0, 0, NULL);
    for (size_t i = 0; i < m_kWorkers.size(); ++i) {
        WaitForSingleObject(m_kWorkers[i], 5000); CloseHandle(m_kWorkers[i]);
    }
    m_kWorkers.clear();
    CloseHandle(m_hPort); m_hPort = NULL;
    WSACleanup();
}

bool IOCPManager::Bind(IOCPSession* pkSess) {
    if (!pkSess || !m_hPort) return false;
    return CreateIoCompletionPort((HANDLE)pkSess->GetSocket(), m_hPort,
                                  (ULONG_PTR)pkSess, 0) != NULL;
}

unsigned __stdcall IOCPManager::WorkerStatic(void* p) {
    ((IOCPManager*)p)->WorkerLoop(); return 0;
}

void IOCPManager::WorkerLoop() {
    while (m_bRun) {
        DWORD       dwBytes = 0;
        ULONG_PTR   ulKey   = 0;
        OVERLAPPED* pOv     = NULL;
        BOOL ok = GetQueuedCompletionStatus(m_hPort, &dwBytes, &ulKey, &pOv, INFINITE);
        if (!m_bRun) break;
        IOCPSession* pkSess = (IOCPSession*)ulKey;
        if (!pkSess) continue;
        if (!ok || dwBytes == 0) { pkSess->OnDisconnect(); pkSess->Close(); continue; }
        pkSess->OnRecvComplete(dwBytes);
        pkSess->PostRecv();
    }
}

} // namespace fiesta
