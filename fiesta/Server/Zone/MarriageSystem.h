// Server/Zone/MarriageSystem.h
// 26 -- Marriage / HolyPromise / Wedding ceremony.
//
// EVIDENCE: PDB_CONFIRMED  symbol: HolyPromise, HolyPromiseServer,
//                                  HolyPromiseSummonGate, MarriageRecord
// EVIDENCE: DATA_CONFIRMED  source: HolyPromiseReward.shn,
//                                  ScenarioBookShelf/Wedding/Wedding.ps
//
// Lifecycle:
//
//   1. *Propose*       - one player asks another. Both must be opposite
//                        sides of the holy-promise gate (no requirement
//                        on race/class/sex; the original game allowed
//                        cross-faction). A pending MarriageRecord is
//                        created with state PROPOSED.
//
//   2. *Accept/Reject* - target accepts -> state ENGAGED, both pay the
//                        ceremony fee. Reject -> record purged.
//
//   3. *Ceremony*      - happens at the wedding NPC; the script
//                        `Wedding.ps` runs and on completion the state
//                        moves to MARRIED and HolyPromiseReward.shn is
//                        consulted for the (count -> reward) gift bag.
//
//   4. *Summon*        - either spouse can call `Summon(spouse)` once
//                        per cooldown; the request hops through
//                        HolyPromiseSummonGate which broadcasts a Y/N
//                        to the partner.
//
//   5. *Divorce*       - either party may cancel; record state DIVORCED.
//                        The pair is unmarked across both characters.
//
// Persistence: marriage state lives on tCharacter (SpouseId column) and
// is hydrated on login via CharDBClient. Ceremony promise count is in
// tHolyPromise (CharDB).
#ifndef FIESTA_ZONE_MARRIAGESYSTEM_H
#define FIESTA_ZONE_MARRIAGESYSTEM_H
#include "ShineObject.h"
#include <map>

namespace fiesta {

enum eMarriageState {
    MS_NONE     = 0,
    MS_PROPOSED = 1,
    MS_ENGAGED  = 2,
    MS_MARRIED  = 3,
    MS_DIVORCED = 4
};

struct MarriageRecord {
    uint32          uiRecID;
    CharID          uiA;
    CharID          uiB;
    eMarriageState  eState;
    uint64          uiCreatedMs;
    uint64          uiCeremonyMs;
    uint32          uiPromiseCount;     // monthly anniversaries claimed
    uint64          uiSummonCdMs;
};

class MarriageServer {
public:
    static MarriageServer& Get();

    bool Propose(CharID a, CharID b);
    bool Accept (CharID a);                   // a is the proposed-to side
    bool Reject (CharID a);
    bool Wed    (CharID a, CharID b, uint64 uiNowMs);   // ceremony complete
    bool Divorce(CharID a);

    // One-tap summon: returns false if no spouse, on cooldown, or busy.
    bool Summon (CharID self, uint64 uiNowMs);

    // Anniversary tick: adds a `promise` and triggers reward grant via
    // HolyPromiseReward.shn. Caller (zone tick) decides cadence.
    void GrantPromiseTick(CharID self, uint64 uiNowMs);

    MarriageRecord* FindByCharID(CharID c);
private:
    MarriageServer() : m_uiNext(1) {}
    std::map<uint32, MarriageRecord> m_kAll;
    std::map<CharID, uint32>         m_kIndex;
    uint32                           m_uiNext;
};

} // namespace fiesta
#endif
