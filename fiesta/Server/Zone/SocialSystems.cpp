#include "SocialSystems.h"
#include "../Shared/ShineLogSystem.h"
namespace fiesta {

bool HolyPromise::Propose (CharID,CharID){ return true; }
bool HolyPromise::Ceremony(CharID,CharID){ return true; }
bool HolyPromise::Summon  (CharID,CharID){ return true; }
void HolyPromiseServer::Tick() {}

static std::map<CharID, std::vector<CharID> > s_kFriends;
static std::map<CharID, uint32>               s_kFriendPoints;
bool Friend::Add(CharID a, CharID b) {
    std::vector<CharID>& v = s_kFriends[a];
    for (size_t i = 0; i < v.size(); ++i) if (v[i] == b) return false;
    v.push_back(b); return true;
}
bool Friend::Remove(CharID a, CharID b) {
    std::vector<CharID>& v = s_kFriends[a];
    for (size_t i = 0; i < v.size(); ++i) if (v[i] == b) { v.erase(v.begin()+i); return true; }
    return false;
}
void Friend::List(CharID a, std::vector<CharID>& r) { r = s_kFriends[a]; }
void Friend::GrantPoint(CharID a, uint32 p) { s_kFriendPoints[a] += p; }

bool Chat::Send(ShinePlayer* pk, ChatChannel c, const std::string& t, const std::string&) {
    if (!pk || !ChatRestrict::IsAllowed(pk, c, t)) return false;
    SHINELOG_DEBUG("Chat[%d] %s: %s", (int)c, pk->GetName().c_str(), t.c_str());
    return true;
}
bool ChatRestrict::IsAllowed(ShinePlayer*, ChatChannel, const std::string& t) {
    if (t.size() > 256) return false;
    return true;
}
int32 SpamerPenaltyDataBox::PenaltySec(uint32 i) { return (int32)i * 60; }

bool CardCollection::Open    (ShinePlayer*, uint32){ return true; }
bool CardCollection::Register(ShinePlayer*, uint32){ return true; }
bool CardCollection::Bookmark(ShinePlayer*, uint32){ return true; }
int32 CardCollection::RewardFor(uint32) { return 1000; }

bool CharacterTitle::Gain (ShinePlayer*, uint32){ return true; }
bool CharacterTitle::Equip(ShinePlayer*, uint32){ return true; }
void CharacterTitleZone::OnLogin(ShinePlayer*) {}

bool EventAttendanceServer::Claim(ShinePlayer*, uint16) { return true; }
bool DailyQuestTimer::ResetIfNewDay(uint64& r) { uint64 prev = r; r = 0; return prev != 0; }
void GMEventManager::Tick() {}
bool FacebookEventReward::ClaimSocialReward(ShinePlayer*) { return false; }

int32 GBHouseTable    ::RentFor(uint16 t) { return 1000 + (int32)t * 500; }
int32 GBTaxRate       ::Pct    (uint16 t) { return 5 + (int32)t; }
int32 GBExchangeMaxCoin::Cap   (uint16 t) { return 10000 * (int32)(t + 1); }

} // namespace fiesta
