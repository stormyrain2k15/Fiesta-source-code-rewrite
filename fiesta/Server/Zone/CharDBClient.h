// Server/Zone/CharDBClient.h
// 06 -- Zone-side outbound IOCP client to the Character DB exe.
//
// Closes the loop opened on the CharDB side (Server/DataServer/Character/Main.cpp):
//
//   Zone CharLogin handler  -- builds NC_INTER_CHAR_DB_QUERY { op=1, charNo }
//                           -- sends via CharDBClient::Get().SendQuery(...)
//   CharDB exe              -- p_Char_Login(charNo) -> NC_INTER_CHAR_DB_RESPONSE
//                           -- replies with row columns
//   CharDBClientSession     -- decodes the response, finds the pending player by
//                              CharID, calls ShinePlayer::LoadFromCharDBRow.
//
// One singleton TCP link per Zone process. If the exe is offline, queries
// silently fail and the player keeps its provisional fill -- the engine still
// runs.
#ifndef FIESTA_ZONE_CHARDB_CLIENT_H
#define FIESTA_ZONE_CHARDB_CLIENT_H
#include "../Shared/Socket_Connector.h"
#include "../Shared/IOCPManager.h"
#include <map>
#include <windows.h>

namespace fiesta {

class ShinePlayer;

class CharDBClient {
public:
    static CharDBClient& Get();

    bool Connect(IOCPManager* pkIOCP, const std::string& rIp, uint16 uiPort);
    void Disconnect();

    // Issue p_Char_Login for a player whose handle is already attached to
    // the Zone, and remember the handle so the response handler can finish
    // populating the row.
    bool QueryCharLogin(CharID c, ShinePlayer* pkP);

    // Issue p_Char_Logout (fire-and-forget).
    void QueryCharLogout(CharID c);

    // ---- Marriage / HolyPromise (SQLP_Wedding + SQLP_HolyPromise) -----
    // All routes fire-and-forget; the CharDB exe answers asynchronously
    // through OnRoutine. Op codes:
    //
    //   10 = p_Char_Wedding_Propose         (cFrom, cTo)
    //   11 = p_Char_Wedding_Cancel_Propose  (cFrom, cTo)
    //   12 = p_Char_Wedding_Do              (cA, cB)
    //   13 = p_Char_Wedding_Divorce_Do      (c)
    //   20 = p_HolyPromise_Set              (cA, cB)
    //   21 = p_HolyPromise_SetDate          (c)
    //   22 = p_HolyPromise_DelChar          (c)
    void WeddingPropose      (CharID a, CharID b);
    void WeddingCancelPropose(CharID a, CharID b);
    void WeddingDo           (CharID a, CharID b);
    void WeddingDivorce      (CharID c);
    void HolyPromiseSet      (CharID a, CharID b);
    void HolyPromiseSetDate  (CharID c);
    void HolyPromiseDelChar  (CharID c);

    // ---- Estate (tEstate / tEstateFurniture, custom procs) ------------
    //
    //   30 = p_Estate_Create   (uiOwner, uiHouseId, uiTier)
    //   31 = p_Estate_Demolish (uiOwner)
    //   32 = p_Estate_Save     (uiOwner, blob)   -- blob = serialized placements
    //   33 = p_Estate_Load     (uiOwner)
    void EstateCreate  (CharID owner, uint32 uiHouseId, uint32 uiTier);
    void EstateDemolish(CharID owner);
    void EstateSave    (CharID owner, const uint8* pData, size_t uiLen);
    void EstateLoad    (CharID owner);

    // Called by CharDBClientSession when an NC_INTER_CHAR_DB_RESPONSE arrives.
    void OnLoginResponse (CharID c, bool bOK, const std::vector<std::string>& rCols);
    void OnLogoutResponse(CharID c, bool bOK);

    bool IsConnected() const { return m_kConn.IsConnected(); }

private:
    CharDBClient();
    Socket_Connector            m_kConn;
    CRITICAL_SECTION            m_kCs;
    std::map<CharID, ShinePlayer*> m_kPending;  // charNo -> player awaiting fill
};

} // namespace fiesta
#endif
