// Server/Zone/LiveOpsBoosts.cpp
#include "LiveOpsBoosts.h"
#include "AnnounceSystem.h"
#include "../Shared/ShineLogSystem.h"
#include "BattleTunables.h"

namespace fiesta {

static void AnnounceSystem_Broadcast(uint8 uiLevel, const std::string& rText) {
    AnnounceSystem::Broadcast(uiLevel, rText);
}

LiveOpsBoosts& LiveOpsBoosts::Get() { static LiveOpsBoosts s; return s; }

int32 LiveOpsBoosts::ExpRateX1k()   const { return m_iExpX1k   > 0 ? m_iExpX1k   : 1000; }
int32 LiveOpsBoosts::DropRateX1k()  const { return m_iDropX1k  > 0 ? m_iDropX1k  : 1000; }
int32 LiveOpsBoosts::MoneyRateX1k() const { return m_iMoneyX1k > 0 ? m_iMoneyX1k : 1000; }

static uint64 NowMs() { return (uint64)::GetTickCount(); }

void LiveOpsBoosts::StartLuckyHour(uint32 uiDurationSec, int32 uiExpX1k, int32 uiDropX1k) {
    if (uiExpX1k  <= 0) uiExpX1k  = 2000;     // stock fallback: 2.00x
    if (uiDropX1k <= 0) uiDropX1k = 2000;
    m_iExpX1k         = uiExpX1k;
    m_iDropX1k        = uiDropX1k;
    m_iMoneyX1k       = 1000;
    m_uiEndsAtMs      = uiDurationSec ? NowMs() + (uint64)uiDurationSec * 1000ULL : 0;
    m_bLuckyHour      = true;
    m_uiActiveEventNo = kGMEvent_LuckyHour;
    m_kActiveLabel    = "Lucky Hour";

    SHINELOG_INFO("LiveOps: Lucky Hour START exp=%d/1k drop=%d/1k dur=%us",
                  m_iExpX1k, m_iDropX1k, uiDurationSec);
    AnnounceSystem_Broadcast(0, "Lucky Hour has begun! Bonus EXP and Drop rates are active.");
}

bool LiveOpsBoosts::StartEvent(uint32 uiEventNo, uint32 uiDurationSec) {
    switch (uiEventNo) {
    case kGMEvent_LuckyHour:
        StartLuckyHour(uiDurationSec, 2000, 2000);
        return true;
    case kGMEvent_DoubleExp:
        m_iExpX1k = 2000; m_iDropX1k = 1000; m_iMoneyX1k = 1000;
        m_uiEndsAtMs = uiDurationSec ? NowMs() + (uint64)uiDurationSec * 1000ULL : 0;
        m_bLuckyHour = false;
        m_uiActiveEventNo = uiEventNo;
        m_kActiveLabel = "Double EXP";
        AnnounceSystem_Broadcast(0, "Double EXP event is now active.");
        SHINELOG_INFO("LiveOps: Double EXP START dur=%us", uiDurationSec);
        return true;
    case kGMEvent_DoubleDrop:
        m_iExpX1k = 1000; m_iDropX1k = 2000; m_iMoneyX1k = 1000;
        m_uiEndsAtMs = uiDurationSec ? NowMs() + (uint64)uiDurationSec * 1000ULL : 0;
        m_bLuckyHour = false;
        m_uiActiveEventNo = uiEventNo;
        m_kActiveLabel = "Double Drop";
        AnnounceSystem_Broadcast(0, "Double Drop event is now active.");
        SHINELOG_INFO("LiveOps: Double Drop START dur=%us", uiDurationSec);
        return true;
    case kGMEvent_GoldenHour:
        m_iExpX1k = 1000; m_iDropX1k = 1000; m_iMoneyX1k = 2000;
        m_uiEndsAtMs = uiDurationSec ? NowMs() + (uint64)uiDurationSec * 1000ULL : 0;
        m_bLuckyHour = false;
        m_uiActiveEventNo = uiEventNo;
        m_kActiveLabel = "Golden Hour";
        AnnounceSystem_Broadcast(0, "Golden Hour: gold drops are doubled.");
        SHINELOG_INFO("LiveOps: Golden Hour START dur=%us", uiDurationSec);
        return true;
    default:
        return false;
    }
}

bool LiveOpsBoosts::StopEvent(uint32 uiEventNo) {
    if (m_uiActiveEventNo != uiEventNo) return false;
    Stop();
    return true;
}

void LiveOpsBoosts::Stop() {
    if (m_uiActiveEventNo != 0) {
        SHINELOG_INFO("LiveOps: %s END (eventNo=%u)",
                      m_kActiveLabel.c_str(), m_uiActiveEventNo);
        std::string msg = m_kActiveLabel.empty() ? "Event ended." : (m_kActiveLabel + " has ended.");
        AnnounceSystem_Broadcast(0, msg);
    }
    m_iExpX1k         = 1000;
    m_iDropX1k        = 1000;
    m_iMoneyX1k       = 1000;
    m_uiEndsAtMs      = 0;
    m_bLuckyHour      = false;
    m_uiActiveEventNo = 0;
    m_kActiveLabel.clear();
}

void LiveOpsBoosts::Tick() {
    if (m_uiEndsAtMs == 0) return;
    if (NowMs() < m_uiEndsAtMs) return;
    Stop();
}

} // namespace fiesta
