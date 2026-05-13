// Client/Network/ShineNetClient.h
// IOCP-backed outbound TCP socket for Shine client.
// Mirrors the server's IOCPSession architecture — same wire frame, same
// stream cipher, same PacketBuffer plumbing — but as a connect-out client
// rather than an accept-in server.
//
// Wire frame (identical to server side):
//   [uint16 length][uint16 NC opcode][payload bytes ...]
//
// Encryption:
//   Session stream cipher keyed from NC_USER_SEED_ACK (server sends seed
//   after connect). Encrypt outbound, decrypt inbound, symmetric.
//
// Threading:
//   Single IOCP completion port; one worker thread per client instance.
//   All OnPacket() callbacks fire on that worker thread.
//   SendPacket() is thread-safe (CRITICAL_SECTION guarded send queue).
#ifndef SHINE_CLIENT_NETWORK_SHINENETCLIENT_H
#define SHINE_CLIENT_NETWORK_SHINENETCLIENT_H

#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#include <windows.h>
#include <string>
#include "../../Server/Shared/ShineTypes.h"
#include "../../Server/Shared/PacketBuffer.h"
#include "../../Server/Shared/GPacket.h"
#include "../../Server/Shared/PacketEncrypt.h"

#pragma comment(lib, "Ws2_32.lib")

namespace shine {

// Callback interface — implement per state machine (Login, WM, Zone).
class INetPacketSink {
public:
    virtual ~INetPacketSink() {}
    virtual void OnConnected()                      = 0;
    virtual void OnDisconnected()                   = 0;
    virtual void OnPacket(const GPacket& rPkt)      = 0;
};

class ShineNetClient {
public:
    ShineNetClient();
    ~ShineNetClient();

    // Connect to host:port, register callback sink, start IOCP worker.
    bool Connect(const std::string& rHost, uint16 uiPort, INetPacketSink* pkSink);
    void Disconnect();
    bool IsConnected() const { return m_kSock != INVALID_SOCKET; }

    // Queue a packet for async send (thread-safe).
    bool SendPacket(const GPacket& rPkt);
    bool SendPacket(uint16 uiOpcode);
    bool SendPacket(uint16 uiOpcode, const void* pPayload, size_t uiLen);

    // Seed the stream cipher (called on NC_USER_SEED_ACK).
    void InitCipher(uint32 uiSeed);

private:
    // IOCP internals
    SOCKET              m_kSock;
    HANDLE              m_hPort;
    HANDLE              m_hWorker;
    OVERLAPPED          m_kOvRecv;
    OVERLAPPED          m_kOvSend;
    char                m_aRecvBlock[8192];
    DWORD               m_dwRecvFlags;

    PacketBuffer        m_kRecvBuf;     // accumulates partial frames
    PacketBuffer        m_kSendQueue;   // pending outbound bytes
    CRITICAL_SECTION    m_kSendCs;
    bool                m_bSendInFlight;

    PacketEncrypt       m_kCipher;
    bool                m_bCipherReady;

    INetPacketSink*     m_pkSink;
    volatile LONG       m_bRun;

    // Worker thread entry
    static unsigned __stdcall WorkerStatic(void* p);
    void   WorkerLoop();

    // Called by worker on recv completion
    void   OnRecvComplete(DWORD dwBytes);
    void   PostRecv();

    // Called by worker on send completion
    void   OnSendComplete();
    void   FlushSend_NoLock();
};

} // namespace shine
#endif // SHINE_CLIENT_NETWORK_SHINENETCLIENT_H
