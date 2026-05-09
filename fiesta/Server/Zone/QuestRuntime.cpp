// Server/Zone/QuestRuntime.cpp
#include "QuestRuntime.h"
#include "ShineObject.h"
#include "Inventory.h"
#include "CharDBClient.h"
#include "../Shared/ShineLogSystem.h"
#include "../Shared/GTimer.h"
#include <windows.h>

namespace fiesta {

// Quest status enum mirrors the World00_Character.tQuest schema:
//   0 = INACTIVE  1 = ACTIVE  2 = COMPLETED  3 = ABANDONED
enum { QSTATE_INACTIVE = 0, QSTATE_ACTIVE = 1, QSTATE_COMPLETED = 2, QSTATE_ABANDONED = 3 };

static uint64 NowMs() { return (uint64)GTimer::NowMillis(); }

static const QuestHeader* GetHeader(uint16 h) { return QuestTable::Get().Header(h); }

eQuestResult QuestRuntime::StartQuest(ShinePlayer* pkP, PlayerLog& rL,
                                       uint16 uiHandle, const std::string& rNpc) {
    if (!pkP) return QR_UNKNOWN_HANDLE;
    const QuestHeader* h = GetHeader(uiHandle);
    if (!h) return QR_UNKNOWN_HANDLE;
    if (rL.kActive.find(uiHandle) != rL.kActive.end()) return QR_ALREADY_ACTIVE;
    if (pkP->GetLevel() < h->uiLevLow)  return QR_LEVEL_TOO_LOW;
    if (pkP->GetLevel() > h->uiLevHigh) return QR_LEVEL_TOO_HIGH;
    if (!h->kStartNPC.empty() && _stricmp(h->kStartNPC.c_str(), rNpc.c_str()) != 0)
        return QR_WRONG_NPC;
    PlayerQuestProgress p; p.uiHandle = uiHandle; p.uiStartedAtMs = NowMs();
    rL.kActive[uiHandle] = p;
    // Persist the new ACTIVE row.
    CharDBClient::Get().QuestSet(pkP->GetCharID(), uiHandle, QSTATE_ACTIVE, 0);
    SHINELOG_INFO("Quest %u started cid=%u via NPC '%s'",
                  uiHandle, pkP->GetCharID(), rNpc.c_str());
    return QR_OK;
}

eQuestResult QuestRuntime::CompleteQuest(ShinePlayer* pkP, PlayerLog& rL, Inventory& rInv,
                                          uint16 uiHandle, const std::string& rNpc) {
    if (!pkP) return QR_UNKNOWN_HANDLE;
    const QuestHeader* h = GetHeader(uiHandle);
    if (!h) return QR_UNKNOWN_HANDLE;
    std::map<uint16, PlayerQuestProgress>::iterator it = rL.kActive.find(uiHandle);
    if (it == rL.kActive.end()) return QR_NOT_ACTIVE;
    if (!h->kStopNPC.empty() && _stricmp(h->kStopNPC.c_str(), rNpc.c_str()) != 0)
        return QR_WRONG_NPC;
    if (h->uiTimeLimitMin > 0) {
        uint64 limit = (uint64)h->uiTimeLimitMin * 60000ULL;
        if (NowMs() - it->second.uiStartedAtMs > limit) {
            rL.kActive.erase(it);
            CharDBClient::Get().QuestSet(pkP->GetCharID(), uiHandle, QSTATE_ABANDONED, 0);
            return QR_TIMED_OUT;
        }
    }
    if (!AllGoalsComplete(rL, uiHandle)) return QR_GOALS_INCOMPLETE;

    // Grant rewards.
    const std::vector<QuestRewardRow>& rewards = QuestTable::Get().RewardsFor(uiHandle);
    for (size_t i = 0; i < rewards.size(); ++i) {
        const QuestRewardRow& r = rewards[i];
        if (_stricmp(r.kType.c_str(), "MoneyExp") == 0) {
            rInv.AddMoney((int64)r.uiQuantity);
        }
        SHINELOG_INFO("Quest reward grant: type=%s qty=%u item='%s' upg=%u",
                      r.kType.c_str(), r.uiQuantity, r.kItemName.c_str(), (uint32)r.uiItemUpgrade);
    }
    rL.kActive.erase(it);
    CharDBClient::Get().QuestSet(pkP->GetCharID(), uiHandle, QSTATE_COMPLETED, 0);
    return QR_OK;
}

eQuestResult QuestRuntime::GiveUp(PlayerLog& rL, uint16 uiHandle) {
    std::map<uint16, PlayerQuestProgress>::iterator it = rL.kActive.find(uiHandle);
    if (it == rL.kActive.end()) return QR_NOT_ACTIVE;
    rL.kActive.erase(it);
    return QR_OK;
}

void QuestRuntime::OnMobKill(PlayerLog& rL, const std::string& rMobName) {
    for (std::map<uint16, PlayerQuestProgress>::iterator it = rL.kActive.begin(); it != rL.kActive.end(); ++it) {
        const std::vector<QuestHuntRow>& v = QuestTable::Get().HuntsFor(it->first);
        for (size_t i = 0; i < v.size(); ++i)
            if (_stricmp(v[i].kMob.c_str(), rMobName.c_str()) == 0) {
                uint16& c = it->second.kHuntCount[v[i].kMob];
                if (c < v[i].uiNumber) ++c;
            }
    }
}

void QuestRuntime::OnItemPick(PlayerLog& rL, const std::string& rItemName) {
    for (std::map<uint16, PlayerQuestProgress>::iterator it = rL.kActive.begin(); it != rL.kActive.end(); ++it) {
        const std::vector<QuestLootRow>& v = QuestTable::Get().LootsFor(it->first);
        for (size_t i = 0; i < v.size(); ++i)
            if (_stricmp(v[i].kItem.c_str(), rItemName.c_str()) == 0) {
                uint16& c = it->second.kLootCount[v[i].kItem];
                if (c < v[i].uiNumber) ++c;
            }
    }
}

void QuestRuntime::OnNpcMeet(PlayerLog& rL, const std::string& rNpcName) {
    for (std::map<uint16, PlayerQuestProgress>::iterator it = rL.kActive.begin(); it != rL.kActive.end(); ++it) {
        if (QuestTable::Get().IsMeeting(it->first)) {
            uint16& c = it->second.kMetCount[rNpcName];
            ++c;
        }
    }
}

void QuestRuntime::OnProduce(PlayerLog& rL, const std::string& rItemMade) {
    for (std::map<uint16, PlayerQuestProgress>::iterator it = rL.kActive.begin(); it != rL.kActive.end(); ++it) {
        const std::vector<QuestProduceRow>& v = QuestTable::Get().ProducesFor(it->first);
        for (size_t i = 0; i < v.size(); ++i)
            if (_stricmp(v[i].kToItem.c_str(), rItemMade.c_str()) == 0) {
                uint16& c = it->second.kProduceCount[v[i].kToItem];
                ++c;
            }
    }
}

bool QuestRuntime::AllGoalsComplete(const PlayerLog& rL, uint16 uiHandle) {
    std::map<uint16, PlayerQuestProgress>::const_iterator it = rL.kActive.find(uiHandle);
    if (it == rL.kActive.end()) return false;
    const PlayerQuestProgress& p = it->second;
    const std::vector<QuestHuntRow>&    hunts    = QuestTable::Get().HuntsFor(uiHandle);
    const std::vector<QuestLootRow>&    loots    = QuestTable::Get().LootsFor(uiHandle);
    const std::vector<QuestProduceRow>& produces = QuestTable::Get().ProducesFor(uiHandle);
    for (size_t i = 0; i < hunts.size(); ++i) {
        std::map<std::string, uint16>::const_iterator jt = p.kHuntCount.find(hunts[i].kMob);
        uint16 c = (jt == p.kHuntCount.end()) ? 0 : jt->second;
        if (c < hunts[i].uiNumber) return false;
    }
    for (size_t i = 0; i < loots.size(); ++i) {
        std::map<std::string, uint16>::const_iterator jt = p.kLootCount.find(loots[i].kItem);
        uint16 c = (jt == p.kLootCount.end()) ? 0 : jt->second;
        if (c < loots[i].uiNumber) return false;
    }
    // Produces require at least one match per row.
    for (size_t i = 0; i < produces.size(); ++i) {
        std::map<std::string, uint16>::const_iterator jt = p.kProduceCount.find(produces[i].kToItem);
        if (jt == p.kProduceCount.end() || jt->second == 0) return false;
    }
    return true;
}

} // namespace fiesta
