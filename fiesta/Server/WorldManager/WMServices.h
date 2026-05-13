// Server/WorldManager/WMServices.h
// Cross-zone "content" systems owned by WorldManager. Each is a thin
// stateful coordinator -- the heavy data writes flow through WMCharDBClient
// to the CharDB exe.
#ifndef SHINE_WM_SERVICES_H
#define SHINE_WM_SERVICES_H
#include "../Shared/ShineTypes.h"
#include <string>
#include <vector>
#include <map>
#include <set>

namespace shine {

class FriendServer {
public:
    static FriendServer& Get();
    bool Add  (CharID a, CharID b);
    bool Del  (CharID a, CharID b);
    void OnLogin (CharID c, uint16 uiMap, uint16 uiLevel);
    void OnLogout(CharID c);
    void OnLevelChange(CharID c, uint16 uiNewLevel);
    void OnMapChange  (CharID c, uint16 uiNewMap);
private:
    FriendServer() {}
    struct Pres { uint16 uiMap; uint16 uiLevel; bool bOnline; };
    std::map<CharID, Pres>             m_kPresence;
    std::map<CharID, std::set<CharID> > m_kFriends;   // c -> friends-of-c
};

class ChatStealServer {
public:
    static ChatStealServer& Get();
    void RouteShout    (CharID from, const std::string& rChannel, const std::string& rText);
    void RouteWorldYell(CharID from, const std::string& rText);
    bool IsBlocked     (CharID c) const;
    void Block         (CharID c, uint32 uiDurationSecs);
    void UnblockExpired();
private:
    ChatStealServer() {}
    std::map<CharID, uint64> m_kBlockUntil;
};

class PartyFinderServer {
public:
    static PartyFinderServer& Get();
    bool Post  (CharID host, uint8 uiCategory, const std::string& rNote);
    bool Cancel(CharID host);
    void GetList(uint8 uiCategory, std::vector<DBRecord>& rOut) const;
    void Tick();         // prunes posts older than the TTL
private:
    PartyFinderServer() {}
    struct Listing { CharID host; uint8 cat; std::string note; uint64 uiAt; };
    std::vector<Listing> m_kPosts;
};

class RankingServer {
public:
    static RankingServer& Get();
    void Tick();         // hourly publish to CharDB
    void RequestList(uint8 uiCategory, std::vector<DBRecord>& rOut);
private:
    RankingServer() : m_uiNextPublishMs(0) {}
    uint64 m_uiNextPublishMs;
};

class PrisonServer {
public:
    static PrisonServer& Get();
    void RecordPK   (CharID killer, CharID victim);
    bool IsPrisoner (CharID c, uint32* pSecsLeft) const;
    void Tick();         // decrements sentences once a minute
private:
    PrisonServer() : m_uiNextDecMs(0) {}
    std::map<CharID, uint32> m_kSentenceSecs;
    uint64 m_uiNextDecMs;
};

class HolyPromiseServer {
public:
    static HolyPromiseServer& Get();
    bool Promise(CharID a, CharID b);
    bool Break  (CharID a, CharID b);
    bool IsPromised(CharID c, CharID* pPartnerOut) const;
private:
    HolyPromiseServer() {}
    std::map<CharID, CharID> m_kCouple;     // bi-directional
};

class GambleSystemServer {
public:
    static GambleSystemServer& Get();
    bool RollDice (CharID c, uint8 uiSides, uint8 uiCount, std::vector<uint8>& rOut);
    bool SpinSlot (CharID c, uint32& uiPrizeOut);
    void TickCoinExchangeWindow();
private:
    GambleSystemServer() {}
};

class EventAttendanceServer {
public:
    static EventAttendanceServer& Get();
    void OnLogin(CharID c);
    void Tick();         // rolls the day-key at midnight UTC
private:
    EventAttendanceServer() : m_uiCurrentDay(0) {}
    uint32 m_uiCurrentDay;
};

class DailyQuestTimer {
public:
    static DailyQuestTimer& Get();
    void Tick();         // fires p_Daily_Reset at the configured wall-clock
private:
    DailyQuestTimer() : m_uiLastFiredDay(0) {}
    uint32 m_uiLastFiredDay;
};

class NpcScheduleServer {
public:
    static NpcScheduleServer& Get();
    void Tick();         // walks NpcSchedule.shn rows and pushes Spawn/Despawn
};

class GMEventManager {
public:
    static GMEventManager& Get();
    void Tick();         // walks GMEvent table; pushes Start/End to all zones
};

class MatchInstanceDungeonServer {
public:
    static MatchInstanceDungeonServer& Get();
    bool Queue (CharID c, uint32 uiMID);
    bool Cancel(CharID c);
    void Tick();         // tries to assemble parties from the queue
private:
    MatchInstanceDungeonServer() {}
    std::map<uint32, std::vector<CharID> > m_kQueues;
};

class KQServer {
public:
    static KQServer& Get();
    void   Vote        (uint32 uiKQID, uint32 uiKingdom);
    void   Start       (uint32 uiKQID);                   // closes voting, starts timer
    void   Join        (uint32 uiKQID, CharID c);
    void   Leave       (uint32 uiKQID, CharID c);
    void   Finish      (uint32 uiKQID, uint32 uiWinningKingdom);
    void   Tick();
private:
    KQServer() {}
    struct Inst {
        uint32 uiKQID;
        std::map<uint32, uint32>           kVotes;        // kingdom -> count
        std::set<CharID>                   kPlayers;
        uint64                             uiEndsAtMs;
        bool                               bRunning;
    };
    std::vector<Inst> m_kRunning;
};

class DataFileServer {
public:
    static DataFileServer& Get();
    bool LoadAll(const std::string& rDataRoot);    // mirrors the Zone-side
                                                   // ShnRegistry / ZoneAssetLoader
                                                   // boot but on the WM exe
};

void WMServicesTickAll();

} // namespace shine
#endif
