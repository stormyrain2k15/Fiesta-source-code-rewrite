// Server/Shared/Socket_Connector.cpp
#include "Socket_Connector.h"
#include "IOCPManager.h"
#include "ShineLogSystem.h"
#include <stdlib.h>

namespace shine {

Socket_Connector::Socket_Connector() : m_pkIOCP(NULL), m_pkSession(NULL) {}
Socket_Connector::~Socket_Connector() { Close(); }

bool Socket_Connector::Connect(IOCPManager* pkIOCP, const std::string& rIp, uint16 uiPort,
                               IOCPSession* pkSession)
{
    if (!pkSession) return false;
    if (m_pkSession) Close();
    m_pkIOCP = pkIOCP;

    SOCKET s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (s == INVALID_SOCKET) { delete pkSession; return false; }

    sockaddr_in sa; ZeroMemory(&sa, sizeof(sa));
    sa.sin_family = AF_INET;
    sa.sin_port   = htons(uiPort);
    sa.sin_addr.s_addr = inet_addr(rIp.c_str());
    if (sa.sin_addr.s_addr == INADDR_NONE) sa.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    if (connect(s, (sockaddr*)&sa, sizeof(sa)) == SOCKET_ERROR) {
        SHINELOG_ERROR("Socket_Connector: connect %s:%u failed (WSA=%d)",
                       rIp.c_str(), (uint32)uiPort, WSAGetLastError());
        closesocket(s); delete pkSession; return false;
    }

    pkSession->AttachSocket(s);
    if (m_pkIOCP) m_pkIOCP->Bind(pkSession);
    pkSession->OnConnect();
    pkSession->PostRecv();
    m_pkSession = pkSession;
    SHINELOG_INFO("Socket_Connector: connected to %s:%u", rIp.c_str(), (uint32)uiPort);
    return true;
}

void Socket_Connector::Close() {
    if (m_pkSession) {
        m_pkSession->OnDisconnect();
        m_pkSession->Close();
        delete m_pkSession;
        m_pkSession = NULL;
    }
    m_pkIOCP = NULL;
}

} // namespace shine
