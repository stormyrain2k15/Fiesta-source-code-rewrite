// Server/Zone/MoreTables.h
// 06+ -- Second batch of typed SHN accessors. Covers Quest, Party, Spamer,
// Toggle/AreaSkill, Set, AccUpgrade, UpgradeInfo, WeaponAttrib,
// UseClassTypeInfo, ShineReward, Pup{Mind,Priority,CaseDesc,Factor},
// Mob{AutoAction,Weapon,Resist}, RaceNameInfo, ReactionType,
// TermExtendMatch, RareMoverRate, RandomOptionCount, SubAbState,
// AbStateSaveTypeInfo, SingleData.
#ifndef FIESTA_ZONE_MORETABLES_H
#define FIESTA_ZONE_MORETABLES_H
#include "../DataReader/ShnRegistry.h"
#include <map>
#include <vector>
#include <string>

namespace fiesta {

// ----- Party bonus tables (XP + drop scale by group size / level diff) ------
class PartyBonusTables {
public:
    static PartyBonusTables& Get();
    void Bind();
    int32 ByLvDiff(int32 iDiff)   const;   // PB_BonusRatio for diff
    int32 ByMember(uint32 uiN)    const;   // PB_BonusRatio for party size N
    int32 ChrLvLimit(uint32 uiLv) const;   // PSE_ExpLimit for char level
private:
    std::map<int32,  int32> m_kByDiff;
    std::map<uint32, int32> m_kByMember;
    std::map<uint32, int32> m_kLimit;
};

// ----- Spamer (anti-spam) tables -------------------------------------------
class SpamerTables {
public:
    static SpamerTables& Get();
    void Bind();
    struct Penalty { uint32 uiChatBlockSec; uint32 uiProbateSec; };
    bool Lookup(uint32 uiPenaltyLv, Penalty& rOut) const;
    bool ReportThreshold(uint32 uiTermSec, uint32 uiNumberThreshold) const;
    bool RepeatRuleHit  (uint32 uiSecond, uint32 uiRepeat) const;
private:
    std::map<uint32, Penalty> m_kPenalty;
    struct Rule { uint32 uiSecond; uint32 uiRepeat; };
    std::vector<Rule>         m_kRules;
    struct ReportTerm { uint32 uiTerm; uint32 uiNumber; std::string kMsg; };
    std::vector<ReportTerm>   m_kReport;
};

// ----- Quest data + dialog + script -----------------------------------------
class QuestTables {
public:
    static QuestTables& Get();
    void Bind();
    const std::string& Dialog (uint32 uiID) const;
    const std::string& Script (uint32 uiID) const;
    const std::string& Species(uint32 uiID) const;  // MobGroupName key
    size_t DialogCount() const { return m_kDialog.size(); }
private:
    std::map<uint32, std::string> m_kDialog;
    std::map<uint32, std::string> m_kScript;
    std::map<uint32, std::string> m_kSpecies;
    std::string                   m_kEmpty;
};

// ----- Toggle skill table ---------------------------------------------------
class ToggleSkillTable {
public:
    static ToggleSkillTable& Get();
    void Bind();
    struct Row { uint32 uiSkillInx; uint32 uiCondition; int32 iValue; };
    const Row* Find(uint32 uiSkillInx) const;
private:
    std::vector<Row>                  m_kRows;
    std::map<uint32, size_t>          m_kBySkill;
};

// ----- Set effects ----------------------------------------------------------
class SetEffectTable {
public:
    static SetEffectTable& Get();
    void Bind();
    struct Row { uint32 uiSetIndex; uint32 uiCount; uint32 uiActionID; };
    // Iterate every set bonus that fires when `uiCount` pieces are equipped.
    void RowsForSet(uint32 uiSetIndex, std::vector<const Row*>& rOut) const;
private:
    std::vector<Row>                  m_kRows;
    std::map<uint32, std::vector<size_t> > m_kBySet;
};

// ----- AccUpgrade / UpgradeInfo --------------------------------------------
class UpgradeTables {
public:
    static UpgradeTables& Get();
    void Bind();
    struct AccRow {
        uint32 uiID;
        uint32 uiCriFail, uiDownFail, uiNormalFail, uiCon, uiLuckySuc;
    };
    struct InfoRow {
        uint32      uiID;
        std::string kInxName;
        uint32      uiUpFactor;
        std::string kUpdata;
    };
    const AccRow*  FindAcc (uint32 uiID) const;
    const InfoRow* FindInfo(uint32 uiID) const;
private:
    std::vector<AccRow>          m_kAcc;       std::map<uint32, size_t> m_kAccById;
    std::vector<InfoRow>         m_kInfo;      std::map<uint32, size_t> m_kInfoById;
};

// ----- WeaponAttrib ---------------------------------------------------------
class WeaponAttribTable {
public:
    static WeaponAttribTable& Get();
    void Bind();
    struct Row {
        uint32 uiWeaponType, uiUsableDegree, uiIsUsableInMoving, uiHitRate;
    };
    const Row* Find(uint32 uiWeaponType) const;
private:
    std::vector<Row>             m_kRows;      std::map<uint32, size_t> m_kByType;
};

// ----- UseClassTypeInfo (item-class -> per-class usable matrix) ------------
class UseClassTypeInfoTable {
public:
    static UseClassTypeInfoTable& Get();
    void Bind();
    // Returns nonzero if the given UseClass row allows the given concrete
    // ClassID to use the item. ClassID lookup uses the documented column
    // headers (Fig, Cfig, War, Gla, Kni, ...).
    bool IsUsable(uint32 uiUseClass, uint32 uiClassID) const;
private:
    // Pre-flattened: uiUseClass -> bitmask of class slots usable.
    std::map<uint32, uint64>     m_kMask;
};

// ----- ShineReward ----------------------------------------------------------
class ShineRewardTable {
public:
    static ShineRewardTable& Get();
    void Bind();
    struct Row {
        uint32 uiHandle;
        uint32 uiRewardType;
        std::string kArgument;
        uint32 uiQuantity;
        uint32 uiUpgrade;
        uint32 uiOptionDegree;
        uint32 uiTitleDegree;
    };
    const Row* Find(uint32 uiHandle) const;
private:
    std::vector<Row>             m_kRows;      std::map<uint32, size_t> m_kByHandle;
};

// ----- Pup follow tables ----------------------------------------------------
class PupAITables {
public:
    static PupAITables& Get();
    void Bind();
    struct Mind     { uint32 uiType; int32 iMin; int32 iMax; };
    struct Priority { uint32 uiType; uint32 uiNum; };
    struct CaseDesc {
        uint32      uiPriorityType;
        uint32      uiCaseType;
        std::string kPupIDX;
        uint32      uiAIType;
        std::string kSMInx;
        uint32      uiActionEffectID;
        std::string kSoundFile;
    };
    struct Factor   {
        uint32 uiMindType;
        uint32 uiFactorConditionType;
        uint32 uiFactorType;
        uint32 uiIsMinus;
        int32  iValue;
    };
    const Mind*     FindMind    (uint32 uiType) const;
    const Priority* FindPriority(uint32 uiType) const;
    void            FactorsByMind(uint32 uiMindType, std::vector<const Factor*>& rOut) const;
    void            CasesByPriority(uint32 uiPrio, std::vector<const CaseDesc*>& rOut) const;
private:
    std::vector<Mind>     m_kMind;     std::map<uint32, size_t> m_kMindByType;
    std::vector<Priority> m_kPrio;     std::map<uint32, size_t> m_kPrioByType;
    std::vector<CaseDesc> m_kCase;
    std::vector<Factor>   m_kFactor;
};

// ----- Mob extras: AutoAction / Weapon / Resist -----------------------------
class MobBattleTables {
public:
    static MobBattleTables& Get();
    void Bind();
    struct AutoActionRow {
        std::string kMobInx;
        uint32 uiAttack, uiTarget, uiActionType, uiStrength, uiEffectRate, uiRange;
        std::string kStateInx;
    };
    struct WeaponRow {
        uint32 uiID;
        std::string kInxName;
        std::string kSkill;
        uint32 uiAtkSpd, uiBlastRate, uiAtkDly, uiSwingTime, uiHitTime;
        uint32 uiAtkType;
        uint32 uiMinWC, uiMaxWC, uiTH, uiMinMA, uiMaxMA, uiMH;
        uint32 uiRange, uiMopAttackTarget, uiHitType;
        std::string kStaName;
        uint32 uiStaStrength, uiStaRate, uiAggroInitialize;
    };
    struct ResistRow {
        std::string kInxName;
        int32 iResDot, iResStun, iResMoveSpeed, iResFear, iResBinding;
        int32 iResReverse, iResMesmerize, iResSeverBone, iResKnockBack, iResTBMinus;
    };
    const WeaponRow* FindWeapon(uint32 uiID)                   const;
    const ResistRow* FindResist(const std::string& rInxName)   const;
    void             AutoActionsByMob(const std::string& rInx,
                                      std::vector<const AutoActionRow*>& rOut) const;
private:
    std::vector<AutoActionRow>   m_kAuto;
    std::vector<WeaponRow>       m_kWeapon;   std::map<uint32, size_t> m_kWeaponById;
    std::vector<ResistRow>       m_kResist;   std::map<std::string, size_t> m_kResistByInx;
};

// ----- Race name + Reaction type + Term-extend match -----------------------
class MiscTables2 {
public:
    static MiscTables2& Get();
    void Bind();
    const std::string& RaceLocalName(uint32 uiRaceID) const;
    int32 Reaction(uint32 uiRAType, const std::string& rMobInx) const; // returns Ecode
    bool  TermExtendMatch(const std::string& rExtendItemIDX,
                          std::string& rOutTermItemIDX) const;
    int32 RareMoverRateAt(size_t uiIndex) const;
    bool  RandomOptionCount(const std::string& rInxName,
                            uint32& uiLimitCountOut, uint32& uiLimitDropRateOut) const;
private:
    std::map<uint32, std::string>                       m_kRace;
    std::map<std::string, int32>                        m_kReaction;   // (RAType<<16 | hash) is overkill -- use 2-key map
    std::map<std::string, std::string>                  m_kTermExtend;
    std::vector<int32>                                  m_kRareRates;
    struct ROC { uint32 uiLimitCount; uint32 uiLimitDropRate; };
    std::map<std::string, ROC>                          m_kROC;
};

// ----- SubAbState / AbStateSaveTypeInfo / SingleData ------------------------
//
// SubAbState.shn is the *effect* table -- one row per (InxName, Strength)
// pair. AbState.kSubAbState links into this table by string, then we
// pick the row whose Strength matches what the source skill stamped.
// Each row carries up to FOUR action slots:
//
//   ActionIndexA + ActionArgA
//   ActionIndexB + ActionArgB     -- 0 = empty slot
//   ActionIndexC + ActionArgC
//   ActionIndexD + ActionArgD
//
// `Type`/`SubType` are the effect category for the dispatcher (in
// `AbStateRuntime.cpp`); `KeepTime` is the per-tier duration in ms
// (multiplied by AbState.KeepTimeRatio when both are non-zero).
class SubAbStateTable {
public:
    static SubAbStateTable& Get();
    void Bind();
    struct Row {
        uint32      uiID;
        std::string kInxName;
        uint32      uiStrength, uiType, uiSubType, uiKeepTimeMs;
        uint32      uiActionIdxA, uiActionIdxB, uiActionIdxC, uiActionIdxD;
        int32       iActionArgA,  iActionArgB,  iActionArgC,  iActionArgD;
    };
    const Row* Find(uint32 uiID) const;
    // Lookup by InxName + Strength tier. Strength=0 returns the
    // lowest-strength row for that name (a sensible default).
    const Row* FindByInx(const std::string& rInx, uint32 uiStrength) const;
    // Iterate every row whose InxName matches; used by the dictionary
    // bootstrap to pre-compute strength counts.
    void       GatherByInx(const std::string& rInx, std::vector<const Row*>& rOut) const;
    size_t     Size() const { return m_kRows.size(); }
private:
    std::vector<Row>             m_kRows;     std::map<uint32, size_t> m_kById;
    std::map<std::string, std::vector<size_t> > m_kByInx;
};

class AbStateSaveTypeInfoTable {
public:
    static AbStateSaveTypeInfoTable& Get();
    void Bind();
    struct Row { uint32 uiSaveType; bool bSaveLink, bSaveDie, bSaveLogoff; };
    const Row* Find(uint32 uiSaveType) const;
private:
    std::vector<Row>             m_kRows;     std::map<uint32, size_t> m_kByType;
};

class SingleDataTable {
public:
    static SingleDataTable& Get();
    void Bind();
    int32 Get(uint32 uiIDX, int32 iDefault) const;
private:
    std::map<uint32, int32>      m_kRows;
};

// ----- AreaSkill richer mapping (to complement SkillTables's Find) ----------
class AreaSkillTable {
public:
    static AreaSkillTable& Get();
    void Bind();
    struct Row {
        std::string kSkillInx;
        uint32 uiStep;
        uint32 uiBMPIndex;
        std::string kImagePin;
        uint32 uiIsDirection;
    };
    void RowsForSkill(const std::string& rSkillInx, std::vector<const Row*>& rOut) const;
private:
    std::vector<Row>             m_kRows;
    std::map<std::string, std::vector<size_t> > m_kBySkill;
};

// ----- One-call binder for everything in this file -------------------------
void BindAllMoreTables();

} // namespace fiesta
#endif
