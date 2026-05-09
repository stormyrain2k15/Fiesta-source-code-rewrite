// Server/Zone/SocialSystems.h
// HolyPromise, Friend, Chat, CardCollection, Title, Attendance.
#ifndef FIESTA_ZONE_SOCIALSYSTEMS_H
#define FIESTA_ZONE_SOCIALSYSTEMS_H
#include "ShineObject.h"
#include <map>
#include <vector>
#include <string>

namespace fiesta {

class HolyPromise { public:
    static bool Propose (CharID a, CharID b);
    static bool Ceremony(CharID a, CharID b);
    static bool Summon  (CharID self, CharID spouse);
};
class HolyPromiseServer { public: static void Tick(); };

class Friend { public:
    static bool Add   (CharID self, CharID other);
    static bool Remove(CharID self, CharID other);
    static void List  (CharID self, std::vector<CharID>& rOut);
    static void GrantPoint(CharID self, uint32 pts);
};

enum ChatChannel { CC_NORMAL = 0, CC_PARTY = 1, CC_GUILD = 2, CC_WHISPER = 3, CC_SHOUT = 4, CC_ADMIN = 5 };
class Chat        { public: static bool Send(ShinePlayer* pk, ChatChannel c, const std::string& rText, const std::string& rTo = ""); };
class ChatRestrict{ public: static bool IsAllowed(ShinePlayer* pk, ChatChannel c, const std::string& rText); };
class SpamerPenaltyDataBox { public: static int32 PenaltySec(uint32 uiInfractions); };

class CardCollection { public:
    static bool Open  (ShinePlayer* pk, uint32 uiCardItemId);
    static bool Register(ShinePlayer* pk, uint32 uiCardKey);
    static bool Bookmark(ShinePlayer* pk, uint32 uiCardKey);
    static int32 RewardFor(uint32 uiSetKey);
};

class CharacterTitle { public:
    static bool Gain (ShinePlayer* pk, uint32 uiTitleId);
    static bool Equip(ShinePlayer* pk, uint32 uiTitleId);
};
class CharacterTitleZone { public: static void OnLogin(ShinePlayer*); };

class EventAttendanceServer { public: static bool Claim(ShinePlayer* pk, uint16 uiDayIdx); };
class DailyQuestTimer       { public: static bool ResetIfNewDay(uint64& rLastResetMs); };
class GMEventManager        { public: static void Tick(); };
class FacebookEventReward   { public: static bool ClaimSocialReward(ShinePlayer* pk); };

class GBHouseTable    { public: static int32 RentFor(uint16 uiTier); };
class GBTaxRate       { public: static int32 Pct    (uint16 uiTier); };
class GBExchangeMaxCoin { public: static int32 Cap (uint16 uiTier); };

} // namespace fiesta
#endif
