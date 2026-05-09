// Server/WorldManager/GambleSystemServer.cpp
// Cross-zone gambling backend. Two flavours:
//   * RollDice -- generic n-sided / k-count dice roll, used by /dice and
//                 by the dice-game NPC. Outcomes are computed on the WM so
//                 every zone sees the same canonical result.
//   * SpinSlot -- GB slot machine. Prize table is loaded from
//                 GambleItemTable.shn; this routine consults the table and
//                 returns a per-spin item id (0 = no prize).
// CoinExchange has a server-side window (open/close based on real time);
// TickCoinExchangeWindow flips the flag every minute.
#include "WMServices.h"
#include "../DataReader/ShnRegistry.h"
#include "../Shared/well512.h"
#include "../Shared/GTimer.h"
#include "../Shared/ShineLogSystem.h"

namespace fiesta {

GambleSystemServer& GambleSystemServer::Get() { static GambleSystemServer s; return s; }

// Single shared RNG, well-512 seeded once at first call.
static well512* GambleRng() {
    static well512 s_kRng;
    static bool    s_bInit = false;
    if (!s_bInit) { s_kRng.Seed((uint32)GTimer::NowMillis()); s_bInit = true; }
    return &s_kRng;
}

bool GambleSystemServer::RollDice(CharID c, uint8 uiSides, uint8 uiCount,
                                  std::vector<uint8>& rOut) {
    if (c == 0 || uiSides == 0 || uiCount == 0) return false;
    if (uiCount > 32) uiCount = 32;
    rOut.clear();
    well512* pkRng = GambleRng();
    for (uint8 i = 0; i < uiCount; ++i) {
        uint32 v = pkRng->NextRange(uiSides);
        rOut.push_back((uint8)(v + 1));
    }
    return true;
}

bool GambleSystemServer::SpinSlot(CharID c, uint32& uiPrizeOut) {
    uiPrizeOut = 0;
    if (c == 0) return false;
    // Pull weighted prize entries from GambleItemTable.shn.
    const ShnFile* pkT = ShnRegistry::Get().GetTable("GambleItemTable");
    if (!pkT || pkT->RecordCount() == 0) return false;
    // Total weight pass.
    uint64 uiTotal = 0;
    for (uint32 i = 0; i < pkT->RecordCount(); ++i) {
        int32 w = ShnGetI32(*pkT, i, "Weight");
        if (w > 0) uiTotal += (uint32)w;
    }
    if (uiTotal == 0) return false;
    well512* pkRng = GambleRng();
    uint64 uiPick = (uint64)pkRng->Next() % uiTotal;
    uint64 uiAcc  = 0;
    for (uint32 i = 0; i < pkT->RecordCount(); ++i) {
        int32 w = ShnGetI32(*pkT, i, "Weight");
        if (w <= 0) continue;
        uiAcc += (uint32)w;
        if (uiPick < uiAcc) {
            int32 itemId = ShnGetI32(*pkT, i, "ItemID");
            uiPrizeOut = (uint32)itemId;
            return true;
        }
    }
    return false;
}

void GambleSystemServer::TickCoinExchangeWindow() {
    // Real-time window: open between minutes [0..15] and [30..45] of every
    // hour. The flag is consumed by the per-zone NPC dialog.
    SYSTEMTIME st; GetLocalTime(&st);
    static bool s_bOpen = false;
    bool bOpenNow = (st.wMinute < 15) || (st.wMinute >= 30 && st.wMinute < 45);
    if (bOpenNow != s_bOpen) {
        s_bOpen = bOpenNow;
        SHINELOG_INFO("CoinExchange window %s", bOpenNow ? "OPEN" : "CLOSED");
    }
}

} // namespace fiesta
