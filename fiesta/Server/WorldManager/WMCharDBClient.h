// Server/WorldManager/WMCharDBClient.h
// WM-side outbound client to the CharDB exe. Opcodes 90+ are reserved
// for the cross-zone systems WM owns (guild / friend / mail / shout-block
// / ranking / KQ / event-attendance / daily-quest-reset).
#ifndef FIESTA_WM_CHARDB_CLIENT_H
#define FIESTA_WM_CHARDB_CLIENT_H
#include "../Shared/Socket_Connector.h"
#include "../Shared/IOCPManager.h"
#include "../Shared/ShineTypes.h"
#include <string>
#include <vector>

namespace fiesta {

class WMCharDBClient {
public:
    static WMCharDBClient& Get();

    bool Connect   (IOCPManager* pkIOCP, const std::string& rIp, uint16 uiPort);
    void Disconnect();
    bool IsConnected() const { return m_kConn.IsConnected(); }

    // ---- Guild ----------------------------------------------------------
    bool GuildCreate     (CharID cMaster, const std::string& rName, uint32& uiGuildNoOut);
    bool GuildDissolve   (uint32 uiGuildNo);
    bool GuildAddMember  (uint32 uiGuildNo, CharID c, uint8 uiRank);
    bool GuildDelMember  (uint32 uiGuildNo, CharID c);
    bool GuildSetRank    (uint32 uiGuildNo, CharID c, uint8 uiRank);
    bool GuildAddFunds   (uint32 uiGuildNo, int64 iDelta);
    bool GuildSetEmblem  (uint32 uiGuildNo, uint32 uiEmblemKey);
    bool GuildWarBegin   (uint32 uiAtk, uint32 uiDef);
    bool GuildWarKill    (uint32 uiAtk, uint32 uiDef, CharID killer, CharID killed);
    bool GuildWarEnd     (uint32 uiAtk, uint32 uiDef, uint32 uiWinner);
    bool GuildAcademyJoin (uint32 uiGuildNo, CharID c);
    bool GuildAcademyLeave(uint32 uiGuildNo, CharID c);
    bool GuildTournamentSet(uint32 uiTNo, uint32 uiGuildNo, int32 iStatus);

    // ---- Friend / mail / titles ----------------------------------------
    bool FriendAdd    (CharID a, CharID b);
    bool FriendDel    (CharID a, CharID b);
    bool MailSend     (CharID from, CharID to, const std::string& rTitle,
                       const std::string& rBody, int64 iAttachMoney);

    // ---- KQ / Event / Daily ---------------------------------------------
    bool KQResultSet  (uint32 uiKQID, uint32 uiWinnerKingdom);
    bool EventAttendanceMark(CharID c, uint8 uiDay);
    bool DailyResetTick(uint32 uiDayKey);

    // ---- Ranking --------------------------------------------------------
    bool RankingPublish(uint8 uiCategory, const std::vector<DBRecord>& rRows);

private:
    WMCharDBClient() {}
    Socket_Connector m_kConn;
};

} // namespace fiesta
#endif
