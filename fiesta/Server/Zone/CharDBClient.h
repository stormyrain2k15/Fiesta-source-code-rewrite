// Server/Zone/CharDBClient.h
// Zone-side outbound IOCP client to the Character DB exe.
// Closes the loop opened on the CharDB side (Server/DataServer/Character/Main.cpp):
//   Zone CharLogin handler  -- builds NC_INTER_CHAR_DB_QUERY { op=1, charNo }
//                           -- sends via CharDBClient::Get().SendQuery(...)
//   CharDB exe              -- p_Char_Login(charNo) -> NC_INTER_CHAR_DB_RESPONSE
//                           -- replies with row columns
//   CharDBClientSession     -- decodes the response, finds the pending player by
//                              CharID, calls ShinePlayer::LoadFromCharDBRow.
// One singleton TCP link per Zone process. If the exe is offline, queries
// silently fail and the player keeps its provisional fill -- the engine still
// runs.
#ifndef SHINE_ZONE_CHARDB_CLIENT_H
#define SHINE_ZONE_CHARDB_CLIENT_H
#include "../Shared/Socket_Connector.h"
#include "../Shared/IOCPManager.h"
#include "../DataServer/Common/Database.h"
#include <map>
#include <windows.h>

namespace shine {

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

    // ---- Item / Quest / Skill / Friend / Guild persistence ----------
    //
    //   40 = p_Item_Create       (cOwner, itemId, count, slot, endure, inxName)
    //   41 = p_Item_Delete       (itemKey)        -- itemKey lo first, then hi
    //   42 = p_Item_SetOption    (itemKey, optType, optData)
    //   50 = p_Quest_Set         (cid, questNo, status, subStatus)
    //   51 = p_Quest_GetAllDoing (cid)            -- replies with row vector
    //   60 = p_Skill_SetPower    (cid, skillNo, slot, value, isActive)
    //   61 = p_Skill_SetPowerAll (cid)
    //   70 = p_Friend_Del_All    (cid)
    //   80 = p_GuildTournament_Set (gtNo, guildNo, status)
    void ItemCreate      (CharID owner, uint32 uiItemId, uint16 uiCount,
                          uint16 uiSlot, uint16 uiEndure, const std::string& rInx);
    void ItemDelete      (uint64 uiItemKey);
    void ItemSetOption   (uint64 uiItemKey, uint8 uiType, int32 iData);
    void QuestSet        (CharID c, uint32 uiQuestNo, int32 iStatus, int32 iSubStatus);
    void QuestGetDoing   (CharID c);
    void SkillSetPower   (CharID c, uint32 uiSkillNo, int32 iSlot, int32 iValue, bool bActive);
    void SkillSetPowerAll(CharID c);
    void FriendDelAll    (CharID c);
    void GuildTournamentSet(uint32 uiGTNo, uint32 uiGuildNo, int32 iStatus);

    // ---- AbState persistence  ----------------
    //
    //   90 = p_AbState_Set    (cid, uiAbStateId, uiRemainMs)
    //   91 = p_AbState_GetAll (cid)               -- replies with row vector
    //
    // Persists abnormal-state status across logout. Generic stored-proc
    // numbers chosen here; the CharDB exe maps them to the real proc
    // names in `Server/DataServer/Character/Main.cpp`.
    void AbStateSet      (CharID c, uint32 uiAbStateId, uint32 uiRemainMs);
    void AbStateGetAll   (CharID c);

    // Called by CharDBClientSession when an NC_INTER_CHAR_DB_RESPONSE arrives.
    void OnLoginResponse (CharID c, bool bOK, const std::vector<std::string>& rCols);
    void OnLogoutResponse(CharID c, bool bOK);
    void OnEstateLoadResponse(CharID owner, bool bOK,
                              const std::vector<DBRecord>& rRows);
    void OnQuestGetDoingResponse(CharID c, bool bOK,
                                 const std::vector<DBRecord>& rRows);
    // Pass the ack from p_Item_Create back to the matching ShineItem so
    // future Remove() / SetOption() calls can supply the SQL key.
    void NoteItemPending(CharID owner, uint16 uiSlot, void* pkItem);
    void OnItemCreateResponse(bool bOK, uint64 uiKey);

    bool IsConnected() const { return m_kConn.IsConnected(); }

private:
    CharDBClient();
    Socket_Connector            m_kConn;
    CRITICAL_SECTION            m_kCs;
    std::map<CharID, ShinePlayer*> m_kPending;  // charNo -> player awaiting fill
    // FIFO of in-flight Item_Create rows; the response handler pops the
    // oldest entry and stamps its uiDbItemKey. Per-character ordering is
    // preserved because Inventory::Add only enqueues from the IOCP work
    // thread.
    std::vector<void*>          m_kItemPending;
};

} // namespace shine
#endif
