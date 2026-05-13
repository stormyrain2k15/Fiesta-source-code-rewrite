// Server/WorldManager/WMServices.cpp
// Single-call master ticker for every cross-zone subsystem on the WM.
// WorldManagerService::OnTick() calls WMServicesTickAll() once per main-
// loop iteration; each subsystem's own Tick() is responsible for its own
// scheduling (rate-limit, decrement cadence, etc.).
#include "WMServices.h"
#include "GuildServer.h"

namespace shine {

void WMServicesTickAll() {
    PartyFinderServer        ::Get().Tick();
    RankingServer            ::Get().Tick();
    PrisonServer             ::Get().Tick();
    EventAttendanceServer    ::Get().Tick();
    DailyQuestTimer          ::Get().Tick();
    NpcScheduleServer        ::Get().Tick();
    GMEventManager           ::Get().Tick();
    MatchInstanceDungeonServer::Get().Tick();
    KQServer                 ::Get().Tick();
    GuildServer              ::Get().Tick();

    // Single-shot side-effects: chat-block expiry, gambling window flip.
    ChatStealServer          ::Get().UnblockExpired();
    GambleSystemServer       ::Get().TickCoinExchangeWindow();
}

} // namespace shine
