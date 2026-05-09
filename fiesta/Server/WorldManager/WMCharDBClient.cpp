// Server/WorldManager/WMCharDBClient.cpp
//
// All these methods enqueue an inter-server packet to the CharDB exe and
// return immediately; the boolean return is "did we successfully push the
// query onto the IOCP wire?" not "did the SQL succeed?". Live status is
// emitted as ack packets the dispatcher logs but does not block on; cross-
// zone guild state therefore tolerates short DB outages without losing the
// in-process model.
#include "WMCharDBClient.h"
#include "../Shared/PacketBuffer.h"
#include "../Shared/GPacket.h"
#include "../Shared/ShineLogSystem.h"
#include "../Common/NETCOMMAND.h"

namespace fiesta {

class WMCharDBClientSession : public IOCPSession {
public:
    virtual void OnPacket(const GPacket&) {}    // CharDB exe acks logged on its side
};

WMCharDBClient& WMCharDBClient::Get() { static WMCharDBClient s; return s; }

bool WMCharDBClient::Connect(IOCPManager* pkIOCP, const std::string& rIp, uint16 uiPort) {
    return m_kConn.Connect(pkIOCP, rIp, uiPort, new WMCharDBClientSession());
}
void WMCharDBClient::Disconnect() { m_kConn.Close(); }

#define WM_OP_GUILD_CREATE         90
#define WM_OP_GUILD_DISSOLVE       91
#define WM_OP_GUILD_ADD_MEMBER     92
#define WM_OP_GUILD_DEL_MEMBER     93
#define WM_OP_GUILD_SET_RANK       94
#define WM_OP_GUILD_ADD_FUNDS      95
#define WM_OP_GUILD_SET_EMBLEM     96
#define WM_OP_GUILD_WAR_BEGIN      97
#define WM_OP_GUILD_WAR_KILL       98
#define WM_OP_GUILD_WAR_END        99
#define WM_OP_GUILD_ACADEMY_JOIN  100
#define WM_OP_GUILD_ACADEMY_LEAVE 101
#define WM_OP_GUILD_TOURN_SET     102
#define WM_OP_FRIEND_ADD          110
#define WM_OP_FRIEND_DEL          111
#define WM_OP_MAIL_SEND           120
#define WM_OP_KQ_RESULT_SET       130
#define WM_OP_EVENT_ATTEND        140
#define WM_OP_DAILY_RESET         150
#define WM_OP_RANKING_PUBLISH     160

#define SEND_BEGIN(opv)                                                     \
    if (!IsConnected()) return false;                                       \
    PacketBuffer body; body.WriteU8((uint8)(opv))
#define SEND_END()                                                          \
    GPacket kPkt; kPkt.SetOpcode(NC_INTER_CHAR_DB_QUERY);                   \
    kPkt.Body().WriteBytes(body.Data(), body.Size());                       \
    m_kConn.SendPacket(kPkt);                                               \
    return true

// ---- Guild ---------------------------------------------------------------
bool WMCharDBClient::GuildCreate(CharID cMaster, const std::string& rName, uint32& uiGuildNoOut) {
    uiGuildNoOut = 0;
    SEND_BEGIN(WM_OP_GUILD_CREATE);
    body.WriteU32(cMaster); body.WriteString(rName);
    SEND_END();
}
bool WMCharDBClient::GuildDissolve(uint32 g) {
    SEND_BEGIN(WM_OP_GUILD_DISSOLVE); body.WriteU32(g); SEND_END();
}
bool WMCharDBClient::GuildAddMember(uint32 g, CharID c, uint8 r) {
    SEND_BEGIN(WM_OP_GUILD_ADD_MEMBER); body.WriteU32(g); body.WriteU32(c); body.WriteU8(r); SEND_END();
}
bool WMCharDBClient::GuildDelMember(uint32 g, CharID c) {
    SEND_BEGIN(WM_OP_GUILD_DEL_MEMBER); body.WriteU32(g); body.WriteU32(c); SEND_END();
}
bool WMCharDBClient::GuildSetRank(uint32 g, CharID c, uint8 r) {
    SEND_BEGIN(WM_OP_GUILD_SET_RANK); body.WriteU32(g); body.WriteU32(c); body.WriteU8(r); SEND_END();
}
bool WMCharDBClient::GuildAddFunds(uint32 g, int64 d) {
    SEND_BEGIN(WM_OP_GUILD_ADD_FUNDS); body.WriteU32(g); body.WriteI64(d); SEND_END();
}
bool WMCharDBClient::GuildSetEmblem(uint32 g, uint32 k) {
    SEND_BEGIN(WM_OP_GUILD_SET_EMBLEM); body.WriteU32(g); body.WriteU32(k); SEND_END();
}
bool WMCharDBClient::GuildWarBegin(uint32 a, uint32 d) {
    SEND_BEGIN(WM_OP_GUILD_WAR_BEGIN); body.WriteU32(a); body.WriteU32(d); SEND_END();
}
bool WMCharDBClient::GuildWarKill(uint32 a, uint32 d, CharID k, CharID kd) {
    SEND_BEGIN(WM_OP_GUILD_WAR_KILL); body.WriteU32(a); body.WriteU32(d);
    body.WriteU32(k); body.WriteU32(kd); SEND_END();
}
bool WMCharDBClient::GuildWarEnd(uint32 a, uint32 d, uint32 w) {
    SEND_BEGIN(WM_OP_GUILD_WAR_END); body.WriteU32(a); body.WriteU32(d); body.WriteU32(w); SEND_END();
}
bool WMCharDBClient::GuildAcademyJoin(uint32 g, CharID c) {
    SEND_BEGIN(WM_OP_GUILD_ACADEMY_JOIN); body.WriteU32(g); body.WriteU32(c); SEND_END();
}
bool WMCharDBClient::GuildAcademyLeave(uint32 g, CharID c) {
    SEND_BEGIN(WM_OP_GUILD_ACADEMY_LEAVE); body.WriteU32(g); body.WriteU32(c); SEND_END();
}
bool WMCharDBClient::GuildTournamentSet(uint32 t, uint32 g, int32 s) {
    SEND_BEGIN(WM_OP_GUILD_TOURN_SET); body.WriteU32(t); body.WriteU32(g); body.WriteI32(s); SEND_END();
}

// ---- Friend / Mail -------------------------------------------------------
bool WMCharDBClient::FriendAdd(CharID a, CharID b) {
    SEND_BEGIN(WM_OP_FRIEND_ADD); body.WriteU32(a); body.WriteU32(b); SEND_END();
}
bool WMCharDBClient::FriendDel(CharID a, CharID b) {
    SEND_BEGIN(WM_OP_FRIEND_DEL); body.WriteU32(a); body.WriteU32(b); SEND_END();
}
bool WMCharDBClient::MailSend(CharID f, CharID t, const std::string& title,
                              const std::string& bodyText, int64 attach) {
    SEND_BEGIN(WM_OP_MAIL_SEND); body.WriteU32(f); body.WriteU32(t);
    body.WriteString(title); body.WriteString(bodyText); body.WriteI64(attach);
    SEND_END();
}

// ---- KQ / Event / Daily / Ranking ---------------------------------------
bool WMCharDBClient::KQResultSet(uint32 kq, uint32 winKingdom) {
    SEND_BEGIN(WM_OP_KQ_RESULT_SET); body.WriteU32(kq); body.WriteU32(winKingdom); SEND_END();
}
bool WMCharDBClient::EventAttendanceMark(CharID c, uint8 day) {
    SEND_BEGIN(WM_OP_EVENT_ATTEND); body.WriteU32(c); body.WriteU8(day); SEND_END();
}
bool WMCharDBClient::DailyResetTick(uint32 dayKey) {
    SEND_BEGIN(WM_OP_DAILY_RESET); body.WriteU32(dayKey); SEND_END();
}
bool WMCharDBClient::RankingPublish(uint8 cat, const std::vector<DBRecord>& rRows) {
    SEND_BEGIN(WM_OP_RANKING_PUBLISH); body.WriteU8(cat);
    body.WriteU16((uint16)rRows.size());
    for (size_t i = 0; i < rRows.size(); ++i) {
        body.WriteU16((uint16)rRows[i].kCols.size());
        for (size_t j = 0; j < rRows[i].kCols.size(); ++j)
            body.WriteString(rRows[i].kCols[j]);
    }
    SEND_END();
}

#undef SEND_BEGIN
#undef SEND_END

} // namespace fiesta
