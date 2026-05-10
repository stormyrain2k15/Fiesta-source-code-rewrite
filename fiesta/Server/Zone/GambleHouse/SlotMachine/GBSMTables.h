// Server/Zone/GambleHouse/SlotMachine/GBSMTables.h
// FEATURE: casino-slot -- nine-table SHN family for the slot machine.
//
// One header collects the eight tiny GBSM* tables because they are
// strictly couples (each row keys into the others by GroupID/Card/Bet)
// and shipping them as eight isolated headers would force every
// consumer to include nine files for one slot resolve. The .cpp side
// IS still split per-SHN -- GBSMAll.cpp, GBSMBetCoin.cpp, GBSMCardRate.cpp,
// GBSMCenter.cpp, GBSMGroup.cpp, GBSMJPRate.cpp, GBSMLine.cpp, GBSMNPC.cpp.
// This is the one allowed shared-header concession: the "nine .cpp,
// one .h" pattern matches NA2016's slot-system source layout.
//
// Tables:
//   GBSMAll     (4 cols)  group full-board match weight + JP flag
//   GBSMBetCoin (2 cols)  bet-tier -> chip cost
//   GBSMCardRate(2 cols)  per-symbol weight on the reel strip
//   GBSMCenter  (2 cols)  group center-square bonus weight
//   GBSMGroup   (2 cols)  group -> reel symbol membership
//   GBSMJPRate  (3 cols)  jackpot pool size -> JP odds boost
//   GBSMLine    (3 cols)  group line-payout weight
//   GBSMNPC     (2 cols)  slot-machine-NPC inx -> bet tier
#ifndef FIESTA_ZONE_GAMBLEHOUSE_SLOTMACHINE_GBSMTABLES_H
#define FIESTA_ZONE_GAMBLEHOUSE_SLOTMACHINE_GBSMTABLES_H
#include "../../../../Shared/ShineTypes.h"
#include <map>
#include <string>
#include <vector>

namespace fiesta {

// ----- GBSMAll -----
struct LegacyGBSMAllRow { uint8 uiGroupID; uint8 uiNum; uint16 uiRatioAll; uint8 bIsJP; };
class GBSMAllTable {
public:
    static GBSMAllTable& Get();
    bool Bind();
    const std::vector<LegacyGBSMAllRow>& Rows() const { return m_kRows; }
private:
    GBSMAllTable() {}
    std::vector<LegacyGBSMAllRow> m_kRows;
};

// ----- GBSMBetCoin -----
class GBSMBetCoinTable {
public:
    static GBSMBetCoinTable& Get();
    bool   Bind();
    uint16 ChipCost(uint32 uiBetTier) const;
private:
    GBSMBetCoinTable() {}
    std::map<uint32, uint16> m_kRows;
};

// ----- GBSMCardRate -----
class GBSMCardRateTable {
public:
    static GBSMCardRateTable& Get();
    bool   Bind();
    uint16 WeightFor(uint32 uiCard) const;
    size_t Count() const { return m_kRows.size(); }
private:
    GBSMCardRateTable() {}
    std::map<uint32, uint16> m_kRows;
};

// ----- GBSMCenter -----
class GBSMCenterTable {
public:
    static GBSMCenterTable& Get();
    bool   Bind();
    uint16 RatioFor(uint8 uiGroupID) const;
private:
    GBSMCenterTable() {}
    std::map<uint8, uint16> m_kRows;
};

// ----- GBSMGroup -----
class GBSMGroupTable {
public:
    static GBSMGroupTable& Get();
    bool   Bind();
    // groupID -> set of card-symbol ids belonging to that group
    bool   IsMember(uint8 uiGroupID, uint32 uiCard) const;
    void   GroupsFor(uint32 uiCard, std::vector<uint8>& rOut) const;
private:
    GBSMGroupTable() {}
    std::vector<std::pair<uint8, uint32> > m_kRows;
};

// ----- GBSMJPRate -----
struct LegacyGBSMJPRateRow { uint32 uiMin; uint32 uiMax; uint32 uiRate; };
class GBSMJPRateTable {
public:
    static GBSMJPRateTable& Get();
    bool   Bind();
    uint32 RateForPool(uint32 uiPoolSize) const;
private:
    GBSMJPRateTable() {}
    std::vector<LegacyGBSMJPRateRow> m_kRows;
};

// ----- GBSMLine -----
struct LegacyGBSMLineRow { uint8 uiGroupID; uint8 uiNum; uint16 uiRatioLine; };
class GBSMLineTable {
public:
    static GBSMLineTable& Get();
    bool Bind();
    const std::vector<LegacyGBSMLineRow>& Rows() const { return m_kRows; }
private:
    GBSMLineTable() {}
    std::vector<LegacyGBSMLineRow> m_kRows;
};

// ----- GBSMNPC -----
class GBSMNPCTable {
public:
    static GBSMNPCTable& Get();
    bool   Bind();
    int32  BetTierFor(const std::string& rNpcInx) const;
private:
    GBSMNPCTable() {}
    std::map<std::string, uint32> m_kRows;
};

} // namespace fiesta
#endif
