// Server/Zone/TypedSchemaConsumers.h
// Pass 1.25 -- consumers for the typed-schema rows (Schemas.h) that had
// zero runtime references. Every field on every row this file touches is
// projected into a runtime predicate / accessor / event so nothing in the
// SHN is parsed-but-unused.
//
// Tables wired here:
//   * ItemActionEffect.shn      -- 5 cols
//   * ItemActionCondition.shn   -- 6 cols
//   * CharacterTitleData.shn    -- 16 cols
//   * ActionRangeFactor.shn     -- 4 cols
//   * GTIServer.shn             -- 7 cols
//   * StateField.shn            -- 3 cols
//   * MIDungeon.shn             -- 11 cols
//   * MIDServer.shn             -- 3 cols
//   * SubAbState.shn            -- 14 cols
//
// EVIDENCE: Schemas.h column ordering taken verbatim. Runtime usage points
// noted inline.
#ifndef FIESTA_ZONE_TYPED_SCHEMA_CONSUMERS_H
#define FIESTA_ZONE_TYPED_SCHEMA_CONSUMERS_H
#include "../Shared/ShineTypes.h"
#include <string>
#include <vector>
#include <map>

namespace fiesta {

class ShinePlayer;

// ----- ItemAction chain -----------------------------------------------------
// ItemAction.shn (already in ItemTables) -> Condition + Effect ID. Each
// pointer drains into one of the tables below to decide *whether* the
// item activates (Condition) and *what* it does (Effect).
class ItemActionResolver {
public:
    // Returns true iff the supplied ItemAction.uiCondition row passes:
    //   * subject target satisfies SubjectTarget gate
    //   * object target satisfies ObjectTarget gate
    //   * ConditionActivity (timing) currently active
    //   * ActivityRate roll succeeds (permille)
    //   * caller within Range
    static bool ConditionFires(uint16 uiConditionID, ShinePlayer* pkSubject, ShinePlayer* pkObject, int32 iDistance);
    // Applies the ItemActionEffect:
    //   EffectTarget : 0=self 1=target 2=party 3=enemy 4=area
    //   EffectActivity: enum into a small switch (heal/damage/dispel/...)
    //   Value         : magnitude
    //   Area          : AoE radius (only honoured for EffectTarget=4)
    static void EffectApply(uint16 uiEffectID, ShinePlayer* pkSelf, ShinePlayer* pkTarget);
};

// ----- CharacterTitleData ---------------------------------------------------
// Per `Type` row: 4 stair-stepped (Title, Value, Fame) tiers gated by
// `Permit` and refreshed every `Refresh` seconds. Server picks the highest
// tier the player qualifies for and stamps it as their active title.
class TitleProgression {
public:
    static TitleProgression& Get();
    void Bind();
    // Returns the index name of the tier this player currently qualifies
    // for under `uiType`. Empty string if none reach the threshold.
    std::string ResolveTitle(uint32 uiType, uint32 uiPlayerValue, uint32 uiPlayerFame) const;
    // True if this Type's `Permit` flag tags it as a publicly visible
    // title (versus admin-only / reward-only titles).
    bool IsPermitted(uint32 uiType) const;
    // Refresh cadence (seconds) the client expects between progression
    // checks. 0 = static (no refresh).
    uint32 RefreshSecs(uint32 uiType) const;
private:
    TitleProgression() {}
    struct Tier   { std::string kInx; uint32 uiValue; uint32 uiFame; };
    struct Family { uint32 uiPermit; uint32 uiRefresh; Tier aTier[4]; };
    std::map<uint32, Family> m_kByType;
};

// ----- ActionRangeFactor ----------------------------------------------------
// Action range bands. Server hit tests pick the matching `RangeType` row
// and clamp damage / accuracy by RangeStart..RangeEnd.
class ActionRangeFactor {
public:
    static ActionRangeFactor& Get();
    void Bind();
    // Given (rangeType, distance) -> matching ActionRangeIndex (0 if none).
    uint32 IndexFor(uint32 uiRangeType, uint32 uiDistance) const;
private:
    ActionRangeFactor() {}
    struct Band { uint32 uiIndex; uint32 uiType; uint32 uiStart; uint32 uiEnd; };
    std::vector<Band> m_kBands;
};

// ----- GTIServer ------------------------------------------------------------
// "GTI" = Goblin / event sub-actions on monster behaviour. The 7 cols form
// a (target, enemyCount, action, payloadInx, payloadValue) routing table.
class GTISystem {
public:
    static GTISystem& Get();
    void Bind();
    struct Action { uint8 uiID; std::string kInxName; uint32 uiSubject; uint8 uiEnemyNum; uint32 uiActionType; std::string kPayloadInx; uint8 uiValue; };
    const Action* Find(uint8 uiID) const;
    size_t Count() const { return m_kRows.size(); }
private:
    GTISystem() {}
    std::vector<Action>                m_kRows;
    std::map<uint8, size_t>            m_kById;
};

// ----- StateField -----------------------------------------------------------
// Per-map persistent AbState that auto-applies to anyone in the field
// (e.g. desert map -> "Heat" sub-state). Three cols: AbStateInx, MapName,
// StateSet.
class StateFieldTable {
public:
    static StateFieldTable& Get();
    void Bind();
    // Returns the AbState inx-name auto-applied on this map (empty=none).
    const std::string& AutoApplyOn(const std::string& rMap) const;
    // Returns the StateSet (group key) configured for the map.
    uint32 StateSet(const std::string& rMap) const;
private:
    StateFieldTable() {}
    struct Row { std::string kAbStateInx; uint32 uiStateSet; };
    std::map<std::string, Row> m_kByMap;
    std::string                m_kEmpty;
};

// ----- MIDungeon + MIDServer -----------------------------------------------
// Mini-instance dungeon directory. Combined view: per-map row carries the
// gating data (level, party-comp, timer) plus the script-language hint and
// the PR (PineScript) entry-point name.
class MIDirectory {
public:
    static MIDirectory& Get();
    void Bind();
    struct Entry {
        uint32      uiMapInx;
        uint8       uiMinLv, uiMaxLv;
        uint8       uiTankNum, uiHealNum, uiDealNum;
        uint8       uiMinUser, uiMaxUser;
        uint8       uiMakeParty;
        uint8       uiCompleteGroup;
        uint16      uiStartTimeMin;
        // From MIDServer.shn:
        std::string kScriptLanguage;   // "Lua" or "PS"
        std::string kPRInx;            // PineScript routine to invoke
    };
    const Entry* Find(uint32 uiMapInx) const;
    bool   PartyMeetsRequirement(uint32 uiMapInx, uint16 uiAvgLv,
                                 uint8 uiTanks, uint8 uiHeals, uint8 uiDeals,
                                 uint8 uiTotal) const;
private:
    MIDirectory() {}
    std::map<uint32, Entry> m_kByMap;
};

// ----- SubAbState -----------------------------------------------------------
// Status sub-effects with up to 4 (ActionIndex, ActionArg) tuples per row.
// Consumers: AbnormalState, RuleOfEngagement (status-driven damage scalars).
class SubAbStateRegistry {
public:
    static SubAbStateRegistry& Get();
    void Bind();
    struct Row {
        uint32      uiID;
        std::string kInxName;
        uint32      uiStrength;
        uint32      uiType;
        uint8       uiSubType;
        uint32      uiKeepTimeMs;
        uint32      aActionIndex[4];
        uint32      aActionArg  [4];
    };
    const Row* Find(uint32 uiID)                 const;
    const Row* FindByInx(const std::string& rIn) const;
    size_t Count() const { return m_kRows.size(); }
private:
    SubAbStateRegistry() {}
    std::vector<Row>                 m_kRows;
    std::map<uint32, size_t>         m_kById;
    std::map<std::string, size_t>    m_kByInx;
};

// One-call binder. Add to BindAllExtendedTables() boot path.
void BindTypedSchemaConsumers();

} // namespace fiesta
#endif
