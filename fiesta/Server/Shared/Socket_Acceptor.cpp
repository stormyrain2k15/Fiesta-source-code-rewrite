// Server/Shared/Socket_Acceptor.cpp
#include "Socket_Acceptor.h"
#include "IOCPManager.h"
#include "ShineLogSystem.h"
#include <process.h>
#include <string.h>

namespace fiesta {

// ---------- IOCPSession ----------
IOCPSession::IOCPSession() : m_kSock(INVALID_SOCKET), m_bSendInFlight(false) {
    ZeroMemory(&m_kOv, sizeof(m_kOv));
    InitializeCriticalSection(&m_kSendCs);
}
IOCPSession::~IOCPSession() { Close(); DeleteCriticalSection(&m_kSendCs); }

void IOCPSession::AttachSocket(SOCKET s) { m_kSock = s; }

void IOCPSession::Close() {
    if (m_kSock != INVALID_SOCKET) { closesocket(m_kSock); m_kSock = INVALID_SOCKET; }
}

bool IOCPSession::PostRecv() {
    if (m_kSock == INVALID_SOCKET) return false;
    ZeroMemory(&m_kOv, sizeof(m_kOv));
    WSABUF wb; wb.buf = m_aRecvBlock; wb.len = sizeof(m_aRecvBlock);
    DWORD dwBytes = 0, dwFlags = 0;
    int r = WSARecv(m_kSock, &wb, 1, &dwBytes, &dwFlags, &m_kOv, NULL);
    if (r == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING) { Close(); return false; }
    return true;
}

void IOCPSession::OnRecvComplete(DWORD dwBytes) {
    if (dwBytes == 0) { Close(); return; }
    m_kRecvBuf.WriteBytes(m_aRecvBlock, dwBytes);
    // dispatch any complete frames
    while (true) {
        GPacket kPkt; size_t consumed = 0;
        if (!GPacket::Unpack(m_kRecvBuf, kPkt, consumed)) break;
        // Shift consumed bytes off the front.
        size_t left = m_kRecvBuf.Size() - consumed;
        PacketBuffer kTmp; if (left) kTmp.WriteBytes(m_kRecvBuf.Data() + consumed, left);
        m_kRecvBuf.Clear();
        if (left) m_kRecvBuf.WriteBytes(kTmp.Data(), kTmp.Size());
        OnPacket(kPkt);
    }
}

bool IOCPSession::SendPacket(const GPacket& rPkt) {
    PacketBuffer kFrame; rPkt.Pack(kFrame);
    return SendRaw(kFrame.Data(), kFrame.Size());
}

bool IOCPSession::SendRaw(const void* p, size_t n) {
    if (m_kSock == INVALID_SOCKET || !p || !n) return false;
    EnterCriticalSection(&m_kSendCs);
    m_kSendBuf.WriteBytes(p, n);
    if (!m_bSendInFlight) FlushSend_NoLock();
    LeaveCriticalSection(&m_kSendCs);
    return true;
}

void IOCPSession::FlushSend_NoLock() {
    if (m_kSendBuf.Size() == 0 || m_kSock == INVALID_SOCKET) return;
    // Fire-and-forget blocking send (dedicated send queue is covered by main loop in
    // production; pass-1 keeps this synchronous to keep packet dispatch ordered).
    int r = send(m_kSock, (const char*)m_kSendBuf.Data(), (int)m_kSendBuf.Size(), 0);
    if (r > 0) {
        size_t left = m_kSendBuf.Size() - (size_t)r;
        PacketBuffer kTmp; if (left) kTmp.WriteBytes(m_kSendBuf.Data() + r, left);
        m_kSendBuf.Clear();
        if (left) m_kSendBuf.WriteBytes(kTmp.Data(), kTmp.Size());
    } else if (r == SOCKET_ERROR && WSAGetLastError() != WSAEWOULDBLOCK) {
        Close();
    }
}

// ---------- Socket_Acceptor ----------
Socket_Acceptor::Socket_Acceptor()
    : m_pkIOCP(NULL), m_kListen(INVALID_SOCKET), m_hThread(NULL),
      m_pkFactory(NULL), m_bRun(0), m_uiPort(0) {}

Socket_Acceptor::~Socket_Acceptor() { Stop(); }

bool Socket_Acceptor::Start(IOCPManager* pk, uint16 uiPort, SessionFactory pkFactory) {
    m_pkIOCP = pk; m_uiPort = uiPort; m_pkFactory = pkFactory;
    m_kListen = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (m_kListen == INVALID_SOCKET) return false;
    BOOL bReuse = TRUE;
    setsockopt(m_kListen, SOL_SOCKET, SO_REUSEADDR, (const char*)&bReuse, sizeof(bReuse));
    sockaddr_in sa; ZeroMemory(&sa, sizeof(sa));
    sa.sin_family = AF_INET; sa.sin_addr.s_addr = htonl(INADDR_ANY); sa.sin_port = htons(uiPort);
    if (bind(m_kListen, (sockaddr*)&sa, sizeof(sa)) == SOCKET_ERROR) { Stop(); return false; }
    if (listen(m_kListen, SOMAXCONN) == SOCKET_ERROR) { Stop(); return false; }
    InterlockedExchange(&m_bRun, 1);
    unsigned tid;
    m_hThread = (HANDLE)_beginthreadex(NULL, 0, &Socket_Acceptor::AcceptThreadStatic, this, 0, &tid);
    SHINELOG_INFO("Socket_Acceptor listening on :%u", uiPort);
    return m_hThread != NULL;
}

void Socket_Acceptor::Stop() {
    InterlockedExchange(&m_bRun, 0);
    if (m_kListen != INVALID_SOCKET) { closesocket(m_kListen); m_kListen = INVALID_SOCKET; }
    if (m_hThread) { WaitForSingleObject(m_hThread, 3000); CloseHandle(m_hThread); m_hThread = NULL; }
}

unsigned __stdcall Socket_Acceptor::AcceptThreadStatic(void* p) {
    ((Socket_Acceptor*)p)->AcceptLoop(); return 0;
}

void Socket_Acceptor::AcceptLoop() {
    while (m_bRun) {
        sockaddr_in sa; int sl = sizeof(sa);
        SOCKET s = accept(m_kListen, (sockaddr*)&sa, &sl);
        if (s == INVALID_SOCKET) { if (!m_bRun) break; Sleep(10); continue; }
        IOCPSession* pkSess = m_pkFactory ? m_pkFactory() : NULL;
        if (!pkSess) { closesocket(s); continue; }
        pkSess->AttachSocket(s);
        if (m_pkIOCP) m_pkIOCP->Bind(pkSess);
        pkSess->OnConnect();
        pkSess->PostRecv();
    }
}

} // namespace fiesta
