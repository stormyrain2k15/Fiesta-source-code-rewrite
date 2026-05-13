// Server/Shared/Socket_Acceptor.h
// IOCP-based listen socket + per-connection session callbacks.
#ifndef SHINE_SOCKET_ACCEPTOR_H
#define SHINE_SOCKET_ACCEPTOR_H
#include "ShineTypes.h"
#include "PacketBuffer.h"
#include "GPacket.h"
#include <vector>

#pragma comment(lib, "Ws2_32.lib")

namespace shine {

class IOCPManager;

// Subclass per-service (LoginClientSession, WMClientSession, ZoneClientSession, ...).
class IOCPSession {
public:
    IOCPSession();
    virtual ~IOCPSession();

    SOCKET GetSocket() const { return m_kSock; }
    bool   IsConnected() const { return m_kSock != INVALID_SOCKET; }
    void   AttachSocket(SOCKET s);
    void   Close();

    // network -> session
    bool   PostRecv();
    void   OnRecvComplete(DWORD dwBytes);

    // session -> network
    bool   SendPacket(const GPacket& rPkt);
    bool   SendRaw(const void* p, size_t n);

    // user override
    virtual void OnConnect()    {}
    virtual void OnDisconnect() {}
    virtual void OnPacket(const GPacket& rPkt) = 0;

protected:
    SOCKET       m_kSock;
    PacketBuffer m_kRecvBuf;
    OVERLAPPED   m_kOv;
    char         m_aRecvBlock[8192];
    CRITICAL_SECTION m_kSendCs;
    PacketBuffer m_kSendBuf;
    bool         m_bSendInFlight;
    void         FlushSend_NoLock();
};

class Socket_Acceptor {
public:
    typedef IOCPSession* (*SessionFactory)();

    Socket_Acceptor();
    ~Socket_Acceptor();

    bool Start(IOCPManager* pkIOCP, uint16 uiPort, SessionFactory pkFactory,
               bool bLoopbackOnly = false);
    void Stop();
private:
    static unsigned __stdcall AcceptThreadStatic(void* p);
    void AcceptLoop();

    IOCPManager*   m_pkIOCP;
    SOCKET         m_kListen;
    HANDLE         m_hThread;
    SessionFactory m_pkFactory;
    LONG           m_bRun;
    uint16         m_uiPort;
    bool           m_bLoopbackOnly;
};

} // namespace shine
#endif
