// Server/Zone/Party.h
// 24 -- Party / Raid.
// EVIDENCE: PDB_CONFIRMED  symbol: PartyCreate, PartyJoin, PartyLeave, PartyKickOut,
//                                  PartyBreak, PartyContainer, RaidSystem
#ifndef FIESTA_ZONE_PARTY_H
#define FIESTA_ZONE_PARTY_H
#include "ShineObject.h"
#include <vector>
#include <map>

namespace fiesta {

struct Party {
    uint32  uiPartyId;
    CharID  uiLeader;
    std::vector<CharID> kMembers;
    bool    bRaid;
};

class PartyContainer {
public:
    static PartyContainer& Get();
    Party*  Create(CharID leader);
    bool    Join (uint32 uiPartyId, CharID c);
    bool    Leave(CharID c);
    bool    Kick (uint32 uiPartyId, CharID kicker, CharID kicked);
    bool    Break(uint32 uiPartyId, CharID requester);
    Party*  GetByMember(CharID c);
    Party*  Get(uint32 uiPartyId);
    static int32 BonusXpPct(size_t uiSize);   // EV_VERIFY
private:
    PartyContainer() : m_uiNext(1) {}
    std::map<uint32, Party> m_kAll;
    uint32 m_uiNext;
};

class RaidSystem { public: static bool ConvertToRaid(uint32 uiPartyId); };
void PartyCreate (CharID leader);
void PartyJoin   (CharID c, uint32 pid);
void PartyLeave  (CharID c);
void PartyKickOut(CharID kicker, CharID kicked);
void PartyBreak  (CharID requester);

} // namespace fiesta
#endif
