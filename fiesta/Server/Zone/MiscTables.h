// Server/Zone/MiscTables.h
// 06+ -- Smaller/singleton tables that don't justify their own group class
// but the engine still needs to consult.
// Each accessor caches the underlying ShnFile pointer at Bind() time and
// exposes a Find() helper. Column reads use ShnGetU32/Str/I32 by name so
// missing columns degrade gracefully (zero/empty).
#ifndef SHINE_ZONE_MISCTABLES_H
#define SHINE_ZONE_MISCTABLES_H
#include "../DataReader/ShnRegistry.h"
#include <map>
#include <vector>

namespace shine {

// ----- Damage L vs L gap tables (PvE / EvP / PvP) ---------------------------
// The 20-level-span scaler in AggroList is the *threat* side; these tables
// hold the *damage* side: an attacker N levels above/below the defender
// gets a multiplier.  We mirror them as flat int arrays for hot-path access.
class DamageLvGapTables {
public:
    static DamageLvGapTables& Get();
    void Bind();
    // diff = attacker.level - defender.level (clamped to [-20, +20]).
    int32 PvE(int32 iDiff) const;
    int32 EvP(int32 iDiff) const;
    int32 PvP(int32 iDiff, uint32 uiClassA, uint32 uiClassD) const;
private:
    std::vector<int32> m_kPvE;     // 24 rows
    std::vector<int32> m_kEvP;     // 24 rows
    // PvP is 151x151 -- attacker class x defender class. Stored linear.
    std::vector<int32> m_kPvP;     // 151*151 = 22801
    uint32             m_uiPvPSide;
};

// ----- Action / state singletons -------------------------------------------
class ActionTables {
public:
    static ActionTables& Get();
    void Bind();
    // Returns the consume effect for an item-action slot.
    int32 ItemEffect(uint32 uiID) const;
    // Range-factor multiplier (col Factor). 0..7.
    int32 RangeFactor(uint32 uiKind) const;
private:
    std::map<uint32, int32> m_kEffect;
    std::vector<int32>      m_kRangeFactor;
};

class StateTables {
public:
    static StateTables& Get();
    void Bind();
    // StateField: zone-level abstate (e.g. PK area, KQ buffs)
    int32 FieldEffect(uint32 uiMapID) const;
    // StateItem: the abstate granted while wearing an item
    int32 ItemEffect (uint32 uiItemID) const;
    // StateMob:  the abstate granted by a particular mob species
    int32 MobEffect  (uint32 uiMobID) const;
private:
    std::map<uint32, int32> m_kField;
    std::map<uint32, int32> m_kItem;
    std::map<uint32, int32> m_kMob;
};

// ----- Town portal / produce / gather (worker professions) -----------------
class CraftTables {
public:
    static CraftTables& Get();
    void Bind();
    bool   HasTownPortal(uint32 uiID) const;
    bool   HasGather(uint32 uiID) const;
    bool   HasProduce(uint32 uiID) const;
private:
    std::map<uint32, bool> m_kTP;
    std::map<uint32, bool> m_kGather;
    std::map<uint32, bool> m_kProduce;
};

// ----- HolyPromise reward + Riding singletons -------------------------------
class HPRewardTable {
public:
    static HPRewardTable& Get();
    void Bind();
    // Returns the reward index for `n` consecutive promise count.
    int32 ForCount(uint32 uiCount) const;
private:
    std::map<uint32, int32> m_kRow;
};

class RidingTable {
public:
    static RidingTable& Get();
    void Bind();
    int32 SpeedFor(uint32 uiID) const;
private:
    std::map<uint32, int32> m_kSpeed;
};

// ----- BadName filter (chat / character creation) ---------------------------
class BadNameFilter {
public:
    static BadNameFilter& Get();
    void Bind();
    // Returns true if `rName` contains any disallowed substring.
    bool IsBlocked(const std::string& rName) const;
    size_t Size() const { return m_kPatterns.size(); }
private:
    std::vector<std::string> m_kPatterns;
};

// ----- One-call binder for all of the above --------------------------------
void BindAllMiscTables();

} // namespace shine
#endif
