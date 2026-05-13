// Client/Network/ShineNetClient.cpp
#include "ShineNetClient.h"
#include "../../Server/Shared/ShineLogSystem.h"
#include <process.h>

namespace shine {

ShineNetClient::ShineNetClient()
    : m_kSock(INVALID_SOCKET), m_hPort(NULL), m_hWorker(NULL),
      m_dwRecvFlags(0), m_bSendInFlight(false), m_bCipherReady(false),
      m_pkSink(NULL), m_bRun(0)
{
    ZeroMemory(&m_kOvRecv, sizeof(m_kOvRecv));
    ZeroMemory(&m_kOvSend, sizeof(m_kOvSend));
    ZeroMemory(m_aRecvBlock, sizeof(m_aRecvBlock));
    InitializeCriticalSection(&m_kSendCs);
}

ShineNetClient::~ShineNetClient() {
    Disconnect();
    DeleteCriticalSection(&m_kSendCs);
}

bool ShineNetClient::Connect(const std::string& rHost, uint16 uiPort, INetPacketSink* pkSink) {
    WSADATA wd;
    WSAStartup(MAKEWORD(2,2), &wd);

    m_pkSink = pkSink;

    m_kSock = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
    if (m_kSock == INVALID_SOCKET) {
        SHINELOG_ERROR("ShineNetClient::Connect WSASocket failed %d", WSAGetLastError());
        return false;
    }

    // Resolve host
    struct hostent* ph = gethostbyname(rHost.c_str());
    if (!ph) {
        SHINELOG_ERROR("ShineNetClient::Connect gethostbyname failed '%s'", rHost.c_str());
        closesocket(m_kSock); m_kSock = INVALID_SOCKET;
        return false;
    }

    SOCKADDR_IN addr;
    ZeroMemory(&addr, sizeof(addr));
    addr.sin_family      = AF_INET;
    addr.sin_port        = htons(uiPort);
    addr.sin_addr.s_addr = *(u_long*)ph->h_addr;

    if (connect(m_kSock, (SOCKADDR*)&addr, sizeof(addr)) == SOCKET_ERROR) {
        SHINELOG_ERROR("ShineNetClient::Connect connect failed %d to %s:%u",
                       WSAGetLastError(), rHost.c_str(), uiPort);
        closesocket(m_kSock); m_kSock = INVALID_SOCKET;
        return false;
    }

    // Bind to IOCP
    m_hPort = CreateIoCompletionPort((HANDLE)m_kSock, NULL, (ULONG_PTR)this, 0);
    if (!m_hPort) {
        SHINELOG_ERROR("ShineNetClient::Connect CreateIoCompletionPort failed");
        closesocket(m_kSock); m_kSock = INVALID_SOCKET;
        return false;
    }

    InterlockedExchange(&m_bRun, 1);
    m_hWorker = (HANDLE)_beginthreadex(NULL, 0, WorkerStatic, this, 0, NULL);
    if (!m_hWorker) {
        SHINELOG_ERROR("ShineNetClient: worker thread failed");
        Disconnect();
        return false;
    }

    PostRecv();

    SHINELOG_INFO("ShineNetClient: connected to %s:%u", rHost.c_str(), uiPort);
    if (m_pkSink) m_pkSink->OnConnected();
    return true;
}

void ShineNetClient::Disconnect() {
    InterlockedExchange(&m_bRun, 0);
    if (m_kSock != INVALID_SOCKET) {
        closesocket(m_kSock);
        m_kSock = INVALID_SOCKET;
    }
    if (m_hPort) {
        PostQueuedCompletionStatus(m_hPort, 0, 0, NULL); // wake worker
        CloseHandle(m_hPort);
        m_hPort = NULL;
    }
    if (m_hWorker) {
        WaitForSingleObject(m_hWorker, 3000);
        CloseHandle(m_hWorker);
        m_hWorker = NULL;
    }
    if (m_pkSink) m_pkSink->OnDisconnected();
}

void ShineNetClient::InitCipher(uint32 uiSeed) {
    m_kCipher.Init(uiSeed);
    m_bCipherReady = true;
    SHINELOG_INFO("ShineNetClient: cipher seeded 0x%08X", uiSeed);
}

bool ShineNetClient::SendPacket(const GPacket& rPkt) {
    PacketBuffer wire;
    rPkt.Pack(wire);
    // Encrypt payload (bytes after the 4-byte header)
    if (m_bCipherReady && wire.Size() > 4) {
        m_kCipher.Encrypt(wire.Data() + 4, wire.Size() - 4);
    }
    EnterCriticalSection(&m_kSendCs);
    m_kSendQueue.WriteBytes(wire.Data(), wire.Size());
    if (!m_bSendInFlight) FlushSend_NoLock();
    LeaveCriticalSection(&m_kSendCs);
    return true;
}

bool ShineNetClient::SendPacket(uint16 uiOpcode) {
    GPacket pkt((NCOpcode)uiOpcode);
    return SendPacket(pkt);
}

bool ShineNetClient::SendPacket(uint16 uiOpcode, const void* pPayload, size_t uiLen) {
    GPacket pkt((NCOpcode)uiOpcode, pPayload, uiLen);
    return SendPacket(pkt);
}

void ShineNetClient::PostRecv() {
    if (m_kSock == INVALID_SOCKET) return;
    WSABUF wb;
    wb.buf = m_aRecvBlock;
    wb.len = sizeof(m_aRecvBlock);
    m_dwRecvFlags = 0;
    ZeroMemory(&m_kOvRecv, sizeof(m_kOvRecv));
    m_kOvRecv.hEvent = (HANDLE)1; // tag recv OV
    DWORD dwRecv = 0;
    int ret = WSARecv(m_kSock, &wb, 1, &dwRecv, &m_dwRecvFlags, &m_kOvRecv, NULL);
    if (ret == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING) {
        SHINELOG_WARN("ShineNetClient::PostRecv WSARecv failed %d", WSAGetLastError());
    }
}

void ShineNetClient::OnRecvComplete(DWORD dwBytes) {
    if (dwBytes == 0) { Disconnect(); return; }

    // Decrypt inbound (skip 4-byte header)
    uint8* pRaw = (uint8*)m_aRecvBlock;
    if (m_bCipherReady && dwBytes > 4) {
        m_kCipher.Decrypt(pRaw + 4, dwBytes - 4);
    }

    m_kRecvBuf.WriteBytes(pRaw, dwBytes);

    // Unpack all complete frames
    GPacket pkt;
    size_t consumed = 0;
    while (GPacket::Unpack(m_kRecvBuf, pkt, consumed)) {
        if (m_pkSink) m_pkSink->OnPacket(pkt);
    }

    PostRecv();
}

void ShineNetClient::FlushSend_NoLock() {
    if (m_kSock == INVALID_SOCKET || m_kSendQueue.Size() == 0) return;
    WSABUF wb;
    wb.buf = (char*)m_kSendQueue.Data();
    wb.len = (ULONG)m_kSendQueue.Size();
    ZeroMemory(&m_kOvSend, sizeof(m_kOvSend));
    DWORD dwSent = 0;
    int ret = WSASend(m_kSock, &wb, 1, &dwSent, 0, &m_kOvSend, NULL);
    if (ret == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING) {
        SHINELOG_WARN("ShineNetClient::FlushSend WSASend failed %d", WSAGetLastError());
        return;
    }
    m_bSendInFlight = true;
}

void ShineNetClient::OnSendComplete() {
    EnterCriticalSection(&m_kSendCs);
    m_kSendQueue.Clear();
    m_bSendInFlight = false;
    LeaveCriticalSection(&m_kSendCs);
}

unsigned __stdcall ShineNetClient::WorkerStatic(void* p) {
    ((ShineNetClient*)p)->WorkerLoop();
    return 0;
}

void ShineNetClient::WorkerLoop() {
    while (InterlockedCompareExchange(&m_bRun, 1, 1)) {
        DWORD       dwBytes = 0;
        ULONG_PTR   uKey    = 0;
        OVERLAPPED* pOv     = NULL;

        BOOL bOk = GetQueuedCompletionStatus(m_hPort, &dwBytes, &uKey, &pOv, 100);
        if (!bOk) {
            if (!pOv) continue; // timeout or spurious
            Disconnect(); break;
        }
        if (!pOv) break; // shutdown sentinel

        if (pOv == &m_kOvRecv) {
            OnRecvComplete(dwBytes);
        } else if (pOv == &m_kOvSend) {
            OnSendComplete();
        }
    }
}

} // namespace shine
