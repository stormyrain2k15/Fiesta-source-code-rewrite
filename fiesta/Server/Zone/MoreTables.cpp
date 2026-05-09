// Server/Zone/MoreTables.cpp
#include "MoreTables.h"
#include "../Shared/ShineLogSystem.h"

namespace fiesta {

namespace { inline const ShnFile* T(const char* n) { return ShnRegistry::Get().GetTable(n); } }

#define ITER(T) for (size_t _r = 0; _r < (T)->Rows().size(); ++_r)

// =============================================================================
//  PartyBonusTables
// =============================================================================
PartyBonusTables& PartyBonusTables::Get() { static PartyBonusTables s; return s; }
void PartyBonusTables::Bind() {
    if (const ShnFile* t = T("PartyBonusByLvDiff")) ITER(t)
        m_kByDiff[ShnGetI32(*t, _r, "PB_LvDiff")]    = ShnGetI32(*t, _r, "PB_BonusRatio");
    if (const ShnFile* t = T("PartyBonusByMember")) ITER(t)
        m_kByMember[ShnGetU32(*t, _r, "PB_PartyMember")] = ShnGetI32(*t, _r, "PB_BonusRatio");
    if (const ShnFile* t = T("PartyBonusLimit"))  ITER(t)
        m_kLimit[ShnGetU32(*t, _r, "PSE_ChrLv")]     = ShnGetI32(*t, _r, "PSE_ExpLimit");
}
int32 PartyBonusTables::ByLvDiff(int32 iD)  const { std::map<int32, int32>::const_iterator it = m_kByDiff.find(iD);  return it == m_kByDiff.end()  ? 1000 : it->second; }
int32 PartyBonusTables::ByMember(uint32 uN) const { std::map<uint32,int32>::const_iterator it = m_kByMember.find(uN); return it == m_kByMember.end() ? 1000 : it->second; }
int32 PartyBonusTables::ChrLvLimit(uint32 uL) const { std::map<uint32,int32>::const_iterator it = m_kLimit.find(uL); return it == m_kLimit.end() ? 0 : it->second; }

// =============================================================================
//  SpamerTables
// =============================================================================
SpamerTables& SpamerTables::Get() { static SpamerTables s; return s; }
void SpamerTables::Bind() {
    if (const ShnFile* t = T("SpamerPenalty")) ITER(t) {
        Penalty p;
        p.uiChatBlockSec = ShnGetU32(*t, _r, "ChatBlockTime");
        p.uiProbateSec   = ShnGetU32(*t, _r, "ProbateTime");
        m_kPenalty[ShnGetU32(*t, _r, "PenaltyLv")] = p;
    }
    if (const ShnFile* t = T("SpamerPenaltyRule")) ITER(t) {
        Rule r;
        r.uiSecond = ShnGetU32(*t, _r, "SecondRule");
        r.uiRepeat = ShnGetU32(*t, _r, "RepeatRule");
        m_kRules.push_back(r);
    }
    if (const ShnFile* t = T("SpamerReport")) ITER(t) {
        ReportTerm r;
        r.uiTerm   = ShnGetU32(*t, _r, "SR_Term");
        r.uiNumber = ShnGetU32(*t, _r, "SR_Number");
        r.kMsg     = ShnGetStr(*t, _r, "SR_Message");
        m_kReport.push_back(r);
    }
}
bool SpamerTables::Lookup(uint32 uiLv, Penalty& rOut) const {
    std::map<uint32, Penalty>::const_iterator it = m_kPenalty.find(uiLv);
    if (it == m_kPenalty.end()) return false;
    rOut = it->second; return true;
}
bool SpamerTables::ReportThreshold(uint32 uiT, uint32 uiN) const {
    for (size_t i = 0; i < m_kReport.size(); ++i)
        if (m_kReport[i].uiTerm == uiT && m_kReport[i].uiNumber == uiN) return true;
    return false;
}
bool SpamerTables::RepeatRuleHit(uint32 uiS, uint32 uiR) const {
    for (size_t i = 0; i < m_kRules.size(); ++i)
        if (uiS <= m_kRules[i].uiSecond && uiR >= m_kRules[i].uiRepeat) return true;
    return false;
}

// =============================================================================
//  QuestTables
// =============================================================================
QuestTables& QuestTables::Get() { static QuestTables s; return s; }
void QuestTables::Bind() {
    if (const ShnFile* t = T("QuestDialog"))  ITER(t) m_kDialog [ShnGetU32(*t, _r, "ID")] = ShnGetStr(*t, _r, "Dialog");
    if (const ShnFile* t = T("QuestScript"))  ITER(t) m_kScript [ShnGetU32(*t, _r, "ID")] = ShnGetStr(*t, _r, "Script");
    if (const ShnFile* t = T("QuestSpecies")) ITER(t) m_kSpecies[ShnGetU32(*t, _r, "ID")] = ShnGetStr(*t, _r, "MobGroupName");
}
const std::string& QuestTables::Dialog (uint32 uiID) const {
    std::map<uint32, std::string>::const_iterator it = m_kDialog.find(uiID);
    return (it == m_kDialog.end()) ? m_kEmpty : it->second; }
const std::string& QuestTables::Script (uint32 uiID) const {
    std::map<uint32, std::string>::const_iterator it = m_kScript.find(uiID);
    return (it == m_kScript.end()) ? m_kEmpty : it->second; }
const std::string& QuestTables::Species(uint32 uiID) const {
    std::map<uint32, std::string>::const_iterator it = m_kSpecies.find(uiID);
    return (it == m_kSpecies.end()) ? m_kEmpty : it->second; }

// =============================================================================
//  ToggleSkillTable
// =============================================================================
ToggleSkillTable& ToggleSkillTable::Get() { static ToggleSkillTable s; return s; }
void ToggleSkillTable::Bind() {
    if (const ShnFile* t = T("ToggleSkill")) ITER(t) {
        Row r;
        r.uiSkillInx  = ShnGetU32(*t, _r, "TS_SkillInx");
        r.uiCondition = ShnGetU32(*t, _r, "TS_Condition");
        r.iValue      = ShnGetI32(*t, _r, "TS_Value");
        m_kBySkill[r.uiSkillInx] = m_kRows.size();
        m_kRows.push_back(r);
    }
}
const ToggleSkillTable::Row* ToggleSkillTable::Find(uint32 uiSkill) const {
    std::map<uint32, size_t>::const_iterator it = m_kBySkill.find(uiSkill);
    return (it == m_kBySkill.end()) ? NULL : &m_kRows[it->second]; }

// =============================================================================
//  SetEffectTable
// =============================================================================
SetEffectTable& SetEffectTable::Get() { static SetEffectTable s; return s; }
void SetEffectTable::Bind() {
    if (const ShnFile* t = T("SetEffect")) ITER(t) {
        Row r;
        r.uiSetIndex = ShnGetU32(*t, _r, "SetItemIndex");
        r.uiCount    = ShnGetU32(*t, _r, "Count");
        r.uiActionID = ShnGetU32(*t, _r, "ItemActionID");
        m_kBySet[r.uiSetIndex].push_back(m_kRows.size());
        m_kRows.push_back(r);
    }
}
void SetEffectTable::RowsForSet(uint32 uiIdx, std::vector<const Row*>& rOut) const {
    rOut.clear();
    std::map<uint32, std::vector<size_t> >::const_iterator it = m_kBySet.find(uiIdx);
    if (it == m_kBySet.end()) return;
    rOut.reserve(it->second.size());
    for (size_t i = 0; i < it->second.size(); ++i) rOut.push_back(&m_kRows[it->second[i]]);
}

// =============================================================================
//  UpgradeTables
// =============================================================================
UpgradeTables& UpgradeTables::Get() { static UpgradeTables s; return s; }
void UpgradeTables::Bind() {
    if (const ShnFile* t = T("AccUpgrade")) ITER(t) {
        AccRow r;
        r.uiID         = ShnGetU32(*t, _r, "ID");
        r.uiCriFail    = ShnGetU32(*t, _r, "CriFail");
        r.uiDownFail   = ShnGetU32(*t, _r, "DownFail");
        r.uiNormalFail = ShnGetU32(*t, _r, "NormalFail");
        r.uiCon        = ShnGetU32(*t, _r, "nCon");
        r.uiLuckySuc   = ShnGetU32(*t, _r, "LuckySuc");
        m_kAccById[r.uiID] = m_kAcc.size();
        m_kAcc.push_back(r);
    }
    if (const ShnFile* t = T("UpgradeInfo")) ITER(t) {
        InfoRow r;
        r.uiID       = ShnGetU32(*t, _r, "ID");
        r.kInxName   = ShnGetStr(*t, _r, "InxName");
        r.uiUpFactor = ShnGetU32(*t, _r, "UpFactor");
        r.kUpdata    = ShnGetStr(*t, _r, "Updata");
        m_kInfoById[r.uiID] = m_kInfo.size();
        m_kInfo.push_back(r);
    }
}
const UpgradeTables::AccRow*  UpgradeTables::FindAcc (uint32 uiID) const {
    std::map<uint32, size_t>::const_iterator it = m_kAccById.find(uiID);
    return (it == m_kAccById.end()) ? NULL : &m_kAcc[it->second]; }
const UpgradeTables::InfoRow* UpgradeTables::FindInfo(uint32 uiID) const {
    std::map<uint32, size_t>::const_iterator it = m_kInfoById.find(uiID);
    return (it == m_kInfoById.end()) ? NULL : &m_kInfo[it->second]; }

// =============================================================================
//  WeaponAttribTable
// =============================================================================
WeaponAttribTable& WeaponAttribTable::Get() { static WeaponAttribTable s; return s; }
void WeaponAttribTable::Bind() {
    if (const ShnFile* t = T("WeaponAttrib")) ITER(t) {
        Row r;
        r.uiWeaponType       = ShnGetU32(*t, _r, "WeaponType");
        r.uiUsableDegree     = ShnGetU32(*t, _r, "UsableDegree");
        r.uiIsUsableInMoving = ShnGetU32(*t, _r, "IsUsableInMoving");
        r.uiHitRate          = ShnGetU32(*t, _r, "HitRate");
        m_kByType[r.uiWeaponType] = m_kRows.size();
        m_kRows.push_back(r);
    }
}
const WeaponAttribTable::Row* WeaponAttribTable::Find(uint32 t) const {
    std::map<uint32, size_t>::const_iterator it = m_kByType.find(t);
    return (it == m_kByType.end()) ? NULL : &m_kRows[it->second]; }

// =============================================================================
//  UseClassTypeInfoTable
// =============================================================================
UseClassTypeInfoTable& UseClassTypeInfoTable::Get() { static UseClassTypeInfoTable s; return s; }
void UseClassTypeInfoTable::Bind() {
    static const char* kClassCols[] = {
        "Fig","Cfig","War","Gla","Kni","Cle","Hcle","Pal","Hol","Gua",
        "Arc","Harc","Sco","Sha","Ran","Mag","Wmag","Enc","Warl","Wiz",
        "Jok","Chs","Cru","Cls","Ass","Sen","Sav"
    };
    static const size_t kN = sizeof(kClassCols) / sizeof(kClassCols[0]);
    if (const ShnFile* t = T("UseClassTypeInfo")) ITER(t) {
        uint64 mask = 0;
        for (size_t c = 0; c < kN; ++c) {
            if (ShnGetU32(*t, _r, kClassCols[c]) != 0) mask |= (1ULL << c);
        }
        m_kMask[ShnGetU32(*t, _r, "UseClass")] = mask;
    }
}
bool UseClassTypeInfoTable::IsUsable(uint32 uiUseClass, uint32 uiClassID) const {
    std::map<uint32, uint64>::const_iterator it = m_kMask.find(uiUseClass);
    if (it == m_kMask.end() || uiClassID >= 64) return false;
    return (it->second & (1ULL << uiClassID)) != 0;
}

// =============================================================================
//  ShineRewardTable
// =============================================================================
ShineRewardTable& ShineRewardTable::Get() { static ShineRewardTable s; return s; }
void ShineRewardTable::Bind() {
    if (const ShnFile* t = T("ShineReward")) ITER(t) {
        Row r;
        r.uiHandle       = ShnGetU32(*t, _r, "RewardHandle");
        r.uiRewardType   = ShnGetU32(*t, _r, "RewardType");
        r.kArgument      = ShnGetStr(*t, _r, "Argument");
        r.uiQuantity     = ShnGetU32(*t, _r, "Quantity");
        r.uiUpgrade      = ShnGetU32(*t, _r, "Upgrade");
        r.uiOptionDegree = ShnGetU32(*t, _r, "OptionDegree");
        r.uiTitleDegree  = ShnGetU32(*t, _r, "TitleDegree");
        m_kByHandle[r.uiHandle] = m_kRows.size();
        m_kRows.push_back(r);
    }
}
const ShineRewardTable::Row* ShineRewardTable::Find(uint32 h) const {
    std::map<uint32, size_t>::const_iterator it = m_kByHandle.find(h);
    return (it == m_kByHandle.end()) ? NULL : &m_kRows[it->second]; }

// =============================================================================
//  PupAITables
// =============================================================================
PupAITables& PupAITables::Get() { static PupAITables s; return s; }
void PupAITables::Bind() {
    if (const ShnFile* t = T("PupMind")) ITER(t) {
        Mind r;
        r.uiType = ShnGetU32(*t, _r, "PupMindType");
        r.iMin   = ShnGetI32(*t, _r, "MinMind");
        r.iMax   = ShnGetI32(*t, _r, "MaxMind");
        m_kMindByType[r.uiType] = m_kMind.size();
        m_kMind.push_back(r);
    }
    if (const ShnFile* t = T("PupPriority")) ITER(t) {
        Priority r;
        r.uiType = ShnGetU32(*t, _r, "PupPriorityType");
        r.uiNum  = ShnGetU32(*t, _r, "PriorityNum");
        m_kPrioByType[r.uiType] = m_kPrio.size();
        m_kPrio.push_back(r);
    }
    if (const ShnFile* t = T("PupCaseDesc")) ITER(t) {
        CaseDesc r;
        r.uiPriorityType   = ShnGetU32(*t, _r, "PupPriorityType");
        r.uiCaseType       = ShnGetU32(*t, _r, "PupCaseType");
        r.kPupIDX          = ShnGetStr(*t, _r, "PupIDX");
        r.uiAIType         = ShnGetU32(*t, _r, "PupAIType");
        r.kSMInx           = ShnGetStr(*t, _r, "SM_Inx");
        r.uiActionEffectID = ShnGetU32(*t, _r, "ActionEffectID");
        r.kSoundFile       = ShnGetStr(*t, _r, "SoundFile");
        m_kCase.push_back(r);
    }
    if (const ShnFile* t = T("PupFactorCondition")) ITER(t) {
        Factor r;
        r.uiMindType            = ShnGetU32(*t, _r, "PupMindType");
        r.uiFactorConditionType = ShnGetU32(*t, _r, "PupFactorConditionType");
        r.uiFactorType          = ShnGetU32(*t, _r, "PupFactorType");
        r.uiIsMinus             = ShnGetU32(*t, _r, "IsMinus");
        r.iValue                = ShnGetI32(*t, _r, "Value");
        m_kFactor.push_back(r);
    }
}
const PupAITables::Mind*     PupAITables::FindMind    (uint32 uT) const {
    std::map<uint32, size_t>::const_iterator it = m_kMindByType.find(uT);
    return (it == m_kMindByType.end()) ? NULL : &m_kMind[it->second]; }
const PupAITables::Priority* PupAITables::FindPriority(uint32 uT) const {
    std::map<uint32, size_t>::const_iterator it = m_kPrioByType.find(uT);
    return (it == m_kPrioByType.end()) ? NULL : &m_kPrio[it->second]; }
void PupAITables::FactorsByMind(uint32 uM, std::vector<const Factor*>& rOut) const {
    rOut.clear();
    for (size_t i = 0; i < m_kFactor.size(); ++i)
        if (m_kFactor[i].uiMindType == uM) rOut.push_back(&m_kFactor[i]);
}
void PupAITables::CasesByPriority(uint32 uP, std::vector<const CaseDesc*>& rOut) const {
    rOut.clear();
    for (size_t i = 0; i < m_kCase.size(); ++i)
        if (m_kCase[i].uiPriorityType == uP) rOut.push_back(&m_kCase[i]);
}

// =============================================================================
//  MobBattleTables
// =============================================================================
MobBattleTables& MobBattleTables::Get() { static MobBattleTables s; return s; }
void MobBattleTables::Bind() {
    if (const ShnFile* t = T("MobAutoAction")) ITER(t) {
        AutoActionRow r;
        r.kMobInx     = ShnGetStr(*t, _r, "MobInx");
        r.uiAttack    = ShnGetU32(*t, _r, "Attack");
        r.uiTarget    = ShnGetU32(*t, _r, "Target");
        r.uiActionType= ShnGetU32(*t, _r, "ActionType");
        r.kStateInx   = ShnGetStr(*t, _r, "StateInx");
        r.uiStrength  = ShnGetU32(*t, _r, "Strength");
        r.uiEffectRate= ShnGetU32(*t, _r, "EffectRate");
        r.uiRange     = ShnGetU32(*t, _r, "Range");
        m_kAuto.push_back(r);
    }
    if (const ShnFile* t = T("MobWeapon")) ITER(t) {
        WeaponRow r;
        r.uiID            = ShnGetU32(*t, _r, "ID");
        r.kInxName        = ShnGetStr(*t, _r, "InxName");
        r.kSkill          = ShnGetStr(*t, _r, "Skill");
        r.uiAtkSpd        = ShnGetU32(*t, _r, "AtkSpd");
        r.uiBlastRate     = ShnGetU32(*t, _r, "BlastRate");
        r.uiAtkDly        = ShnGetU32(*t, _r, "AtkDly");
        r.uiSwingTime     = ShnGetU32(*t, _r, "SwingTime");
        r.uiHitTime       = ShnGetU32(*t, _r, "HitTime");
        r.uiAtkType       = ShnGetU32(*t, _r, "AtkType");
        r.uiMinWC         = ShnGetU32(*t, _r, "MinWC");
        r.uiMaxWC         = ShnGetU32(*t, _r, "MaxWC");
        r.uiTH            = ShnGetU32(*t, _r, "TH");
        r.uiMinMA         = ShnGetU32(*t, _r, "MinMA");
        r.uiMaxMA         = ShnGetU32(*t, _r, "MaxMA");
        r.uiMH            = ShnGetU32(*t, _r, "MH");
        r.uiRange         = ShnGetU32(*t, _r, "Range");
        r.uiMopAttackTarget=ShnGetU32(*t, _r, "MopAttackTarget");
        r.uiHitType       = ShnGetU32(*t, _r, "HitType");
        r.kStaName        = ShnGetStr(*t, _r, "StaName");
        r.uiStaStrength   = ShnGetU32(*t, _r, "StaStrength");
        r.uiStaRate       = ShnGetU32(*t, _r, "StaRate");
        r.uiAggroInitialize=ShnGetU32(*t, _r, "AggroInitialize");
        m_kWeaponById[r.uiID] = m_kWeapon.size();
        m_kWeapon.push_back(r);
    }
    if (const ShnFile* t = T("MobResist")) ITER(t) {
        ResistRow r;
        r.kInxName      = ShnGetStr(*t, _r, "InxName");
        r.iResDot       = ShnGetI32(*t, _r, "ResDot");
        r.iResStun      = ShnGetI32(*t, _r, "ResStun");
        r.iResMoveSpeed = ShnGetI32(*t, _r, "ResMoveSpeed");
        r.iResFear      = ShnGetI32(*t, _r, "ResFear");
        r.iResBinding   = ShnGetI32(*t, _r, "ResBinding");
        r.iResReverse   = ShnGetI32(*t, _r, "ResReverse");
        r.iResMesmerize = ShnGetI32(*t, _r, "ResMesmerize");
        r.iResSeverBone = ShnGetI32(*t, _r, "ResSeverBone");
        r.iResKnockBack = ShnGetI32(*t, _r, "ResKnockBack");
        r.iResTBMinus   = ShnGetI32(*t, _r, "ResTBMinus");
        m_kResistByInx[r.kInxName] = m_kResist.size();
        m_kResist.push_back(r);
    }
}
const MobBattleTables::WeaponRow* MobBattleTables::FindWeapon(uint32 uID) const {
    std::map<uint32, size_t>::const_iterator it = m_kWeaponById.find(uID);
    return (it == m_kWeaponById.end()) ? NULL : &m_kWeapon[it->second]; }
const MobBattleTables::ResistRow* MobBattleTables::FindResist(const std::string& rN) const {
    std::map<std::string, size_t>::const_iterator it = m_kResistByInx.find(rN);
    return (it == m_kResistByInx.end()) ? NULL : &m_kResist[it->second]; }
void MobBattleTables::AutoActionsByMob(const std::string& rN, std::vector<const AutoActionRow*>& rOut) const {
    rOut.clear();
    for (size_t i = 0; i < m_kAuto.size(); ++i)
        if (m_kAuto[i].kMobInx == rN) rOut.push_back(&m_kAuto[i]);
}

// =============================================================================
//  MiscTables2
// =============================================================================
MiscTables2& MiscTables2::Get() { static MiscTables2 s; return s; }
void MiscTables2::Bind() {
    if (const ShnFile* t = T("RaceNameInfo")) ITER(t)
        m_kRace[ShnGetU32(*t, _r, "RaceID")] = ShnGetStr(*t, _r, "acLocalName");
    if (const ShnFile* t = T("ReactionType")) ITER(t) {
        char k[256];
        const std::string& mob = ShnGetStr(*t, _r, "MobInx");
        uint32 raType = ShnGetU32(*t, _r, "RAType");
        // 2-key into a single map: "<RAType>|<MobInx>"
        sprintf_s(k, sizeof(k), "%u|%s", raType, mob.c_str());
        m_kReaction[k] = ShnGetI32(*t, _r, "Ecode");
    }
    if (const ShnFile* t = T("TermExtendMatch")) ITER(t)
        m_kTermExtend[ShnGetStr(*t, _r, "ExtendItemIDX")] = ShnGetStr(*t, _r, "TermItemIDX");
    if (const ShnFile* t = T("RareMoverRate")) ITER(t)
        m_kRareRates.push_back(ShnGetI32(*t, _r, "RMR_Rate"));
    if (const ShnFile* t = T("RandomOptionCount")) ITER(t) {
        ROC r;
        r.uiLimitCount    = ShnGetU32(*t, _r, "LimitCount");
        r.uiLimitDropRate = ShnGetU32(*t, _r, "LimitDropRate");
        m_kROC[ShnGetStr(*t, _r, "InxName")] = r;
    }
}
const std::string& MiscTables2::RaceLocalName(uint32 uR) const {
    static std::string e;
    std::map<uint32, std::string>::const_iterator it = m_kRace.find(uR);
    return (it == m_kRace.end()) ? e : it->second;
}
int32 MiscTables2::Reaction(uint32 uiRA, const std::string& rMob) const {
    char k[256]; sprintf_s(k, sizeof(k), "%u|%s", uiRA, rMob.c_str());
    std::map<std::string, int32>::const_iterator it = m_kReaction.find(k);
    return (it == m_kReaction.end()) ? 0 : it->second;
}
bool MiscTables2::TermExtendMatch(const std::string& rE, std::string& rOut) const {
    std::map<std::string, std::string>::const_iterator it = m_kTermExtend.find(rE);
    if (it == m_kTermExtend.end()) return false;
    rOut = it->second; return true;
}
int32 MiscTables2::RareMoverRateAt(size_t uiIndex) const {
    return (uiIndex < m_kRareRates.size()) ? m_kRareRates[uiIndex] : 0;
}
bool MiscTables2::RandomOptionCount(const std::string& rN, uint32& uC, uint32& uR) const {
    std::map<std::string, ROC>::const_iterator it = m_kROC.find(rN);
    if (it == m_kROC.end()) { uC = 0; uR = 0; return false; }
    uC = it->second.uiLimitCount; uR = it->second.uiLimitDropRate; return true;
}

// =============================================================================
//  SubAbStateTable / AbStateSaveTypeInfoTable / SingleDataTable
// =============================================================================
SubAbStateTable& SubAbStateTable::Get() { static SubAbStateTable s; return s; }
void SubAbStateTable::Bind() {
    if (const ShnFile* t = T("SubAbState")) ITER(t) {
        Row r;
        r.uiID         = ShnGetU32(*t, _r, "ID");
        r.kInxName     = ShnGetStr(*t, _r, "InxName");
        r.uiStrength   = ShnGetU32(*t, _r, "Strength");
        r.uiType       = ShnGetU32(*t, _r, "Type");
        r.uiSubType    = ShnGetU32(*t, _r, "SubType");
        r.uiKeepTimeMs = ShnGetU32(*t, _r, "KeepTime");
        r.uiActionIdxA = ShnGetU32(*t, _r, "ActionIndexA");
        r.iActionArgA  = ShnGetI32(*t, _r, "ActionArgA");
        r.uiActionIdxB = ShnGetU32(*t, _r, "ActionIndexB");
        r.iActionArgB  = ShnGetI32(*t, _r, "ActionArgB");
        r.uiActionIdxC = ShnGetU32(*t, _r, "ActionIndexC");
        r.iActionArgC  = ShnGetI32(*t, _r, "ActionArgC");
        r.uiActionIdxD = ShnGetU32(*t, _r, "ActionIndexD");
        r.iActionArgD  = ShnGetI32(*t, _r, "ActionArgD");
        m_kById[r.uiID] = m_kRows.size();
        m_kRows.push_back(r);
    }
}
const SubAbStateTable::Row* SubAbStateTable::Find(uint32 uID) const {
    std::map<uint32, size_t>::const_iterator it = m_kById.find(uID);
    return (it == m_kById.end()) ? NULL : &m_kRows[it->second]; }

AbStateSaveTypeInfoTable& AbStateSaveTypeInfoTable::Get() { static AbStateSaveTypeInfoTable s; return s; }
void AbStateSaveTypeInfoTable::Bind() {
    if (const ShnFile* t = T("AbStateSaveTypeInfo")) ITER(t) {
        Row r;
        r.uiSaveType  = ShnGetU32(*t, _r, "AbStateSaveType");
        r.bSaveLink   = ShnGetU32(*t, _r, "IsSaveLink")   != 0;
        r.bSaveDie    = ShnGetU32(*t, _r, "IsSaveDie")    != 0;
        r.bSaveLogoff = ShnGetU32(*t, _r, "IsSaveLogoff") != 0;
        m_kByType[r.uiSaveType] = m_kRows.size();
        m_kRows.push_back(r);
    }
}
const AbStateSaveTypeInfoTable::Row* AbStateSaveTypeInfoTable::Find(uint32 uT) const {
    std::map<uint32, size_t>::const_iterator it = m_kByType.find(uT);
    return (it == m_kByType.end()) ? NULL : &m_kRows[it->second]; }

SingleDataTable& SingleDataTable::Get() { static SingleDataTable s; return s; }
void SingleDataTable::Bind() {
    if (const ShnFile* t = T("SingleData")) ITER(t)
        m_kRows[ShnGetU32(*t, _r, "SingleDataIDX")] = ShnGetI32(*t, _r, "SingleDataValue");
}
int32 SingleDataTable::Get(uint32 uiIDX, int32 iDef) const {
    std::map<uint32, int32>::const_iterator it = m_kRows.find(uiIDX);
    return (it == m_kRows.end()) ? iDef : it->second;
}

// =============================================================================
//  AreaSkillTable
// =============================================================================
AreaSkillTable& AreaSkillTable::Get() { static AreaSkillTable s; return s; }
void AreaSkillTable::Bind() {
    if (const ShnFile* t = T("AreaSkill")) ITER(t) {
        Row r;
        r.kSkillInx    = ShnGetStr(*t, _r, "AS_SkillInx");
        r.uiStep       = ShnGetU32(*t, _r, "AS_Step");
        r.uiBMPIndex   = ShnGetU32(*t, _r, "AS_BMPIndex");
        r.kImagePin    = ShnGetStr(*t, _r, "AS_ImagePin");
        r.uiIsDirection= ShnGetU32(*t, _r, "AS_IsDirection");
        m_kBySkill[r.kSkillInx].push_back(m_kRows.size());
        m_kRows.push_back(r);
    }
}
void AreaSkillTable::RowsForSkill(const std::string& rS, std::vector<const Row*>& rOut) const {
    rOut.clear();
    std::map<std::string, std::vector<size_t> >::const_iterator it = m_kBySkill.find(rS);
    if (it == m_kBySkill.end()) return;
    rOut.reserve(it->second.size());
    for (size_t i = 0; i < it->second.size(); ++i) rOut.push_back(&m_kRows[it->second[i]]);
}

// =============================================================================
//  One-call binder
// =============================================================================
void BindAllMoreTables() {
    PartyBonusTables::Get()        .Bind();
    SpamerTables::Get()            .Bind();
    QuestTables::Get()             .Bind();
    ToggleSkillTable::Get()        .Bind();
    SetEffectTable::Get()          .Bind();
    UpgradeTables::Get()           .Bind();
    WeaponAttribTable::Get()       .Bind();
    UseClassTypeInfoTable::Get()   .Bind();
    ShineRewardTable::Get()        .Bind();
    PupAITables::Get()             .Bind();
    MobBattleTables::Get()         .Bind();
    MiscTables2::Get()             .Bind();
    SubAbStateTable::Get()         .Bind();
    AbStateSaveTypeInfoTable::Get().Bind();
    SingleDataTable::Get()         .Bind();
    AreaSkillTable::Get()          .Bind();
}

#undef ITER
} // namespace fiesta
