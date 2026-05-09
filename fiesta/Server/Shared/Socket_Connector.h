// Server/Shared/Socket_Connector.h
// IOCP-based outbound client socket. Counterpart to Socket_Acceptor.
// Every cross-service link in the stack (Zone -> CharDB, WM -> CharDB,
// Login -> WM-AccountDB-side, etc.) opens with this. It owns a single
// `IOCPSession` and reconnects on demand.
// Design intent
// =============
//   * Single-shot Connect() does a synchronous TCP connect; if it succeeds
//     the session is bound to the IOCPManager and PostRecv() armed.
//   * IsConnected() reflects the underlying SOCKET state.
//   * Send() is a thin pass-through to IOCPSession::SendPacket so call sites
//     don't have to keep their own pointer to the session.
//   * Close() tears down the session; the next Connect() re-opens.
#ifndef FIESTA_SOCKET_CONNECTOR_H
#define FIESTA_SOCKET_CONNECTOR_H
#include "Socket_Acceptor.h"

namespace fiesta {

class IOCPManager;

class Socket_Connector {
public:
    Socket_Connector();
    ~Socket_Connector();

    // Open and bind to the IOCP. The session is owned by the connector and
    // freed on Close() / dtor. Caller passes a *new*-allocated session.
    bool Connect(IOCPManager* pkIOCP, const std::string& rIp, uint16 uiPort,
                 IOCPSession* pkSession);
    void Close();

    bool IsConnected() const { return m_pkSession && m_pkSession->IsConnected(); }
    IOCPSession* Session() const { return m_pkSession; }

    bool SendPacket(const GPacket& rPkt) {
        return m_pkSession ? m_pkSession->SendPacket(rPkt) : false;
    }
private:
    IOCPManager* m_pkIOCP;
    IOCPSession* m_pkSession;
};

} // namespace fiesta
#endif
