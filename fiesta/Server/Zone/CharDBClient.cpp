// Server/Zone/CharDBClient.cpp
#include "CharDBClient.h"
#include "ShineObject.h"
#include "../DataServer/Common/Database.h"   // DBRecord
#include "../Shared/PacketBuffer.h"
#include "../Shared/GPacket.h"
#include "../Shared/ShineLogSystem.h"
#include "../Common/NETCOMMAND.h"
#include "../Common/SendPacket.h"

namespace fiesta {

// ---------------------------------------------------------------------------
//  CharDBClientSession -- glue between the raw IOCPSession and the registry
//  in CharDBClient.
// ---------------------------------------------------------------------------
class CharDBClientSession : public IOCPSession {
public:
    virtual void OnConnect()    { SHINELOG_INFO("Zone->CharDB connected"); }
    virtual void OnDisconnect() { SHINELOG_WARN("Zone->CharDB disconnected"); }
    virtual void OnPacket(const GPacket& rPkt) {
        if (rPkt.GetOpcode() != NC_INTER_CHAR_DB_RESPONSE) return;
        PacketBuffer body = rPkt.Body();
        uint8 op = 0, ok = 0;
        body.ReadU8(op); body.ReadU8(ok);
        if (op == 1) {
            // CharLogin response: uint16 nCols, then nCols ASCIIZ strings
            std::vector<std::string> cols;
            if (ok) {
                uint16 nCols = 0; body.ReadU16(nCols);
                cols.reserve(nCols);
                for (uint16 i = 0; i < nCols; ++i) {
                    std::string s; body.ReadString(s); cols.push_back(s);
                }
            }
            // CharID is column 0 in the projection.
            CharID c = cols.empty() ? 0 : (CharID)strtoul(cols[0].c_str(), NULL, 10);
            CharDBClient::Get().OnLoginResponse(c, ok != 0, cols);
        } else if (op == 2) {
            // CharLogout response carries no row; we don't track the cid here
            // because logout is fire-and-forget. Pass 0 as a sentinel.
            CharDBClient::Get().OnLogoutResponse(0, ok != 0);
        }
    }
};

// ---------------------------------------------------------------------------
//  CharDBClient
// ---------------------------------------------------------------------------
CharDBClient& CharDBClient::Get() { static CharDBClient s; return s; }

CharDBClient::CharDBClient() { InitializeCriticalSection(&m_kCs); }

bool CharDBClient::Connect(IOCPManager* pkIOCP, const std::string& rIp, uint16 uiPort) {
    return m_kConn.Connect(pkIOCP, rIp, uiPort, new CharDBClientSession());
}

void CharDBClient::Disconnect() {
    EnterCriticalSection(&m_kCs);
    m_kPending.clear();
    LeaveCriticalSection(&m_kCs);
    m_kConn.Close();
}

bool CharDBClient::QueryCharLogin(CharID c, ShinePlayer* pkP) {
    if (!pkP) return false;
    if (!IsConnected()) {
        SHINELOG_WARN("CharDBClient: offline -- player cid=%u keeps provisional fill", c);
        return false;
    }
    EnterCriticalSection(&m_kCs);
    m_kPending[c] = pkP;
    LeaveCriticalSection(&m_kCs);

    PacketBuffer body;
    body.WriteU8(1);       // op = login
    body.WriteU32(c);
    GPacket kPkt; kPkt.SetOpcode(NC_INTER_CHAR_DB_QUERY);
    kPkt.Body().WriteBytes(body.Data(), body.Size());
    return m_kConn.SendPacket(kPkt);
}

void CharDBClient::QueryCharLogout(CharID c) {
    if (!IsConnected()) return;
    PacketBuffer body;
    body.WriteU8(2);       // op = logout
    body.WriteU32(c);
    GPacket kPkt; kPkt.SetOpcode(NC_INTER_CHAR_DB_QUERY);
    kPkt.Body().WriteBytes(body.Data(), body.Size());
    m_kConn.SendPacket(kPkt);
}

void CharDBClient::OnLoginResponse(CharID c, bool bOK, const std::vector<std::string>& rCols) {
    ShinePlayer* pkP = NULL;
    EnterCriticalSection(&m_kCs);
    std::map<CharID, ShinePlayer*>::iterator it = m_kPending.find(c);
    if (it != m_kPending.end()) { pkP = it->second; m_kPending.erase(it); }
    LeaveCriticalSection(&m_kCs);

    if (!pkP) {
        SHINELOG_WARN("CharDBClient: stray login response cid=%u", c);
        return;
    }
    if (!bOK) {
        SHINELOG_WARN("CharDBClient: p_Char_Login FAIL cid=%u (keep provisional fill)", c);
        return;
    }
    DBRecord rec; rec.kCols = rCols;
    pkP->LoadFromCharDBRow(rec);
    SHINELOG_INFO("CharDBClient: cid=%u populated from CharDB (lvl=%u class=%u)",
                  c, (uint32)pkP->GetLevel(), (uint32)pkP->GetClass());
}

void CharDBClient::OnLogoutResponse(CharID, bool) {
    // No state to update; pass-through.
}

// ---------------------------------------------------------------------------
//  Marriage / HolyPromise persistence (fire-and-forget). The CharDB exe
//  routes each opcode to its SQLP_Wedding / SQLP_HolyPromise handler.
// ---------------------------------------------------------------------------
#define DBC_SEND2(op, a, b) do {                                            \
    if (!IsConnected()) return;                                             \
    PacketBuffer body;                                                      \
    body.WriteU8((op)); body.WriteU32((a)); body.WriteU32((b));             \
    GPacket kPkt; kPkt.SetOpcode(NC_INTER_CHAR_DB_QUERY);                   \
    kPkt.Body().WriteBytes(body.Data(), body.Size());                       \
    m_kConn.SendPacket(kPkt);                                               \
} while (0)

#define DBC_SEND1(op, c) do {                                               \
    if (!IsConnected()) return;                                             \
    PacketBuffer body;                                                      \
    body.WriteU8((op)); body.WriteU32((c));                                 \
    GPacket kPkt; kPkt.SetOpcode(NC_INTER_CHAR_DB_QUERY);                   \
    kPkt.Body().WriteBytes(body.Data(), body.Size());                       \
    m_kConn.SendPacket(kPkt);                                               \
} while (0)

void CharDBClient::WeddingPropose      (CharID a, CharID b) { DBC_SEND2(10, a, b); }
void CharDBClient::WeddingCancelPropose(CharID a, CharID b) { DBC_SEND2(11, a, b); }
void CharDBClient::WeddingDo           (CharID a, CharID b) { DBC_SEND2(12, a, b); }
void CharDBClient::WeddingDivorce      (CharID c)           { DBC_SEND1(13, c);    }
void CharDBClient::HolyPromiseSet      (CharID a, CharID b) { DBC_SEND2(20, a, b); }
void CharDBClient::HolyPromiseSetDate  (CharID c)           { DBC_SEND1(21, c);    }
void CharDBClient::HolyPromiseDelChar  (CharID c)           { DBC_SEND1(22, c);    }

void CharDBClient::EstateCreate(CharID owner, uint32 uiHouseId, uint32 uiTier) {
    if (!IsConnected()) return;
    PacketBuffer body;
    body.WriteU8 (30);
    body.WriteU32(owner);
    body.WriteU32(uiHouseId);
    body.WriteU32(uiTier);
    GPacket kPkt; kPkt.SetOpcode(NC_INTER_CHAR_DB_QUERY);
    kPkt.Body().WriteBytes(body.Data(), body.Size());
    m_kConn.SendPacket(kPkt);
}
void CharDBClient::EstateDemolish(CharID owner) { DBC_SEND1(31, owner); }
void CharDBClient::EstateSave(CharID owner, const uint8* pData, size_t uiLen) {
    if (!IsConnected() || !pData) return;
    PacketBuffer body;
    body.WriteU8 (32);
    body.WriteU32(owner);
    body.WriteU32((uint32)uiLen);
    body.WriteBytes(pData, uiLen);
    GPacket kPkt; kPkt.SetOpcode(NC_INTER_CHAR_DB_QUERY);
    kPkt.Body().WriteBytes(body.Data(), body.Size());
    m_kConn.SendPacket(kPkt);
}
void CharDBClient::EstateLoad(CharID owner) { DBC_SEND1(33, owner); }

#undef DBC_SEND2
#undef DBC_SEND1

} // namespace fiesta
