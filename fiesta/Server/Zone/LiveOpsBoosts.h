// Server/Zone/LiveOpsBoosts.h
// Live-ops boost registry. Holds the multiplicative scalers applied on
// top of MainOption / BattleTunables so a GM can flip a "Lucky Hour"
// (or any other timed event) without a server restart.
//
// All scalers are stored x1000 (1000 = stock 1.00x). A scaler of 0 is
// treated as "no override" by callers so the underlying global rate
// from BattleTunables.h still applies.
//
// Inputs:
//   * GMEventManager (Zone side) on NC_INTER_BROADCAST_CMD kind=2 from WM
//   * AmpersandCommands `&luckyhour <minutes> <expX1k> <dropX1k> [moneyX1k]`
//
// Consumers:
//   * Battle.cpp           -> ExpRateX1k() applied to per-mob EXP grant
//   * ItemSystems.cpp      -> DropRateX1k() applied to DropContext.nGlobalRateX1k
//   * Loot / quest reward  -> MoneyRateX1k() applied to gold drops
//
// Lifecycle: ZoneServer::Tick() calls LiveOpsBoosts::Get().Tick() once a
// second; expired boosts auto-revert to stock.
#ifndef FIESTA_ZONE_LIVEOPSBOOSTS_H
#define FIESTA_ZONE_LIVEOPSBOOSTS_H
#include "../Shared/ShineTypes.h"
#include <string>

namespace fiesta {

// Well-known GMEvent.shn event ids handled specially. Anything outside
// this range falls through to the generic announce-only path.
//
// VERIFY: these ids (1001-1004) are placeholders chosen by this rewrite
// pass. The original NA2016 GMEvent.shn drop has not been catalogued
// row-by-row in this tree yet, so these MUST be reconciled against the
// real EventNo values before going live -- otherwise WM windows from
// the real GMEvent.shn will fall into the unrecognised path and only
// emit a generic banner instead of a boost.
const uint32 kGMEvent_LuckyHour      = 1001;   // EXP + Drop boost
const uint32 kGMEvent_DoubleExp      = 1002;
const uint32 kGMEvent_DoubleDrop     = 1003;
const uint32 kGMEvent_GoldenHour     = 1004;   // Money boost only

class LiveOpsBoosts {
public:
    static LiveOpsBoosts& Get();

    // Stock = 1000 (no change). 0 means "uninitialised, treat as 1000".
    int32 ExpRateX1k()   const;
    int32 DropRateX1k()  const;
    int32 MoneyRateX1k() const;

    bool         IsLuckyHourActive() const { return m_bLuckyHour; }
    const char*  ActiveLabel() const { return m_kActiveLabel.c_str(); }

    // Programmatic start. uiDurationSec=0 keeps the boost open until
    // an explicit Stop() call (used by GMEvent.shn time-window mode).
    void StartLuckyHour(uint32 uiDurationSec, int32 uiExpX1k, int32 uiDropX1k);

    // Generic "GM event no" entry: looks at the event id, picks the
    // appropriate boost shape, and starts it for the configured window.
    // Returns false if the eventNo isn't recognised (caller still
    // announces).
    bool StartEvent(uint32 uiEventNo, uint32 uiDurationSec);
    bool StopEvent (uint32 uiEventNo);

    // Hard reset to stock.
    void Stop();

    // Per-second tick from ZoneServer::Tick(). Reverts to stock once the
    // configured duration elapses.
    void Tick();

private:
    LiveOpsBoosts()
        : m_iExpX1k(1000), m_iDropX1k(1000), m_iMoneyX1k(1000),
          m_uiEndsAtMs(0), m_bLuckyHour(false), m_uiActiveEventNo(0) {}

    int32       m_iExpX1k;
    int32       m_iDropX1k;
    int32       m_iMoneyX1k;
    uint64      m_uiEndsAtMs;            // 0 == open-ended
    bool        m_bLuckyHour;
    uint32      m_uiActiveEventNo;
    std::string m_kActiveLabel;
};

} // namespace fiesta
#endif
