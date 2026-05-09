// Server/DataReader/Schemas.h
// Field ordinals + types are taken verbatim from public SHN documentation.
// Field bodies/comments are short factual labels only.
#ifndef FIESTA_DATAREADER_SCHEMAS_H
#define FIESTA_DATAREADER_SCHEMAS_H
#include "../Shared/ShineTypes.h"
#include "ITableBase.h"
#include "DataBox.h"
#include <vector>
namespace fiesta {

// ItemInfo.shn  -- 91 fields
struct ItemInfoRow {
    uint32 ID;
    char InxName[32];
    char Name[64];
    uint32 Type;
    uint32 Class;
    uint32 MaxLot;
    uint32 Equip;
    uint32 ItemAuctionSubGroup;
    uint8 TwoHand;
    uint32 AtkSpeed;
    uint32 DemandLv;
    uint32 Grade;
    uint32 MinWC;
    uint32 MaxWC;
    uint32 AC;
    uint32 MinMA;
    uint32 MaxMA;
    uint32 MR;
    uint32 WCRate;
    uint32 MARate;
    uint32 ACRate;
    uint32 MRRate;
    uint32 CriRate;
    uint32 CriMinWc;
    uint32 CriMaxWc;
    uint32 CriMinMa;
    uint32 CriMaxMa;
    uint32 CrlTB;
    uint32 MaxHP;
    uint32 MaxSP;
    uint32 MaxAP;
    uint32 dummy;
    uint32 Fig;
    uint32 Cfig;
    uint32 War;
    uint32 Gla;
    uint32 Kni;
    uint32 Cle;
    uint32 Hcle;
    uint32 Pal;
    uint32 Hol;
    uint32 Gua;
    uint32 Arc;
    uint32 Harc;
    uint32 Sco;
    uint32 Sha;
    uint32 Ran;
    uint32 Mag;
    uint32 Wmag;
    uint32 Enc;
    uint32 Warl;
    uint32 Wiz;
    uint32 Jok;
    uint32 Chs;
    uint32 Cru;
    uint32 Cls;
    uint32 Ass;
    uint32 BuyPrice;
    uint32 SellPrice;
    uint32 BuyFame;
    uint32 BuyGToken;
    uint32 BuyGBCoin;
    uint32 WeaponType;
    uint32 ArmorType;
    uint8 UpLimit;
    uint16 UpSucRatio;
    uint16 UpLuckRatio;
    uint8 UpResource;
    uint16 BasicUpInx;
    uint16 AddUpInx;
    uint32 TH;
    uint32 TB;
    uint32 ShieldAC;
    uint32 HitRatePlus;
    uint32 EvaRatePlus;
    uint32 MACriPlus;
    uint32 CriDamPlus;
    uint32 MagCriDamPlus;
    uint8 PutOnBelonged;
    uint8 Belonged;
    uint8 NoDrop;
    uint8 NoSell;
    uint8 NoStorage;
    uint8 NoTrade;
    uint8 NoDelete;
    char TitleName[32];
    uint32 ItemGradeType;
    char ItemUseSkill[32];
    char SetItemIndex[32];
    uint32 ItemFunc;
    uint8 AutoMon;
};
class ItemInfoTab : public ITableBase<ItemInfoRow>, public IDataTable {
public: ItemInfoTab() { ms_pkTable = this; }
    virtual bool Load(DataReader& r);
    virtual void BeforeTerminate() { ITableBase<ItemInfoRow>::BeforeTerminate(); }
    virtual uint32 GetTotal() const { return ITableBase<ItemInfoRow>::GetTotal(); }
    virtual const char* LogicalName() const { return "ItemInfo"; }
};
extern ItemInfoTab g_ItemInfoTab;

// AbState.shn  -- 18 fields
struct AbStateRow {
    uint32 ID;
    char InxName[32];
    uint32 AbStataIndex;
    uint8 KeepTimePower;
    uint8 StateGrade;
    char PartyState1[32];
    char PartyState2[32];
    char PartyState3[32];
    char PartyState4[32];
    char PartyState5[32];
    uint32 PartyRange;
    uint32 PartyEnchantNumber;
    char SubAbState[32];
    uint32 DispelIndex;
    uint32 SubDispelIndex;
    uint8 IsSave;
    char MainStateInx[32];
    uint8 Duplicate;
};
class AbStateTab : public ITableBase<AbStateRow>, public IDataTable {
public: AbStateTab() { ms_pkTable = this; }
    virtual bool Load(DataReader& r);
    virtual void BeforeTerminate() { ITableBase<AbStateRow>::BeforeTerminate(); }
    virtual uint32 GetTotal() const { return ITableBase<AbStateRow>::GetTotal(); }
    virtual const char* LogicalName() const { return "AbState"; }
};
extern AbStateTab g_AbStateTab;

// SubAbState.shn  -- 14 fields
struct SubAbStateRow {
    uint32 ID;
    char InxName[32];
    uint32 Strength;
    uint32 Type;
    uint8 SubType;
    uint32 KeepTime;
    uint32 ActionIndexA;
    uint32 ActionArgA;
    uint32 ActionIndexB;
    uint32 ActionArgB;
    uint32 ActionIndexC;
    uint32 ActionArgC;
    uint32 ActionIndexD;
    uint32 ActionArgD;
};
class SubAbStateTab : public ITableBase<SubAbStateRow>, public IDataTable {
public: SubAbStateTab() { ms_pkTable = this; }
    virtual bool Load(DataReader& r);
    virtual void BeforeTerminate() { ITableBase<SubAbStateRow>::BeforeTerminate(); }
    virtual uint32 GetTotal() const { return ITableBase<SubAbStateRow>::GetTotal(); }
    virtual const char* LogicalName() const { return "SubAbState"; }
};
extern SubAbStateTab g_SubAbStateTab;

// ActiveSkill.shn  -- 114 fields
struct ActiveSkillRow {
    uint32 ID;
    char InxName[32];
    char Name[64];
    uint32 Grade;
    uint32 Step;
    uint32 MaxStep;
    uint32 DemandType;
    char DemandSk[32];
    uint32 UseItem;
    uint32 ItemNumber;
    uint32 ItemOption;
    uint32 DemandItem1;
    uint32 DemandItem2;
    uint32 SP;
    uint32 HP;
    uint32 Range;
    uint32 First;
    uint32 Last;
    uint8 IsMovingSkill;
    uint16 UsableDegree;
    uint16 DirectionRotate;
    uint16 SkillDegree;
    uint32 SkillTargetState;
    uint16 CastTime;
    uint32 DlyTime;
    uint32 DlyGroupNum;
    uint32 DlyTimeGroup;
    uint32 MinWC;
    uint32 MaxWC;
    uint32 MinMA;
    uint32 MaxMA;
    uint32 AC;
    uint32 MR;
    uint32 Area;
    uint32 TargetNumber;
    uint32 dummy;
    uint32 Fig;
    uint32 Cfig;
    uint32 War;
    uint32 Gla;
    uint32 Kni;
    uint32 Cle;
    uint32 Hcle;
    uint32 Pal;
    uint32 Hol;
    uint32 Gua;
    uint32 Arc;
    uint32 Harc;
    uint32 Sco;
    uint32 Sha;
    uint32 Ran;
    uint32 Mag;
    uint32 Wmag;
    uint32 Enc;
    uint32 Warl;
    uint32 Wiz;
    uint32 Jok;
    uint32 Chs;
    uint32 Cru;
    uint32 Cls;
    uint32 Ass;
    char StaNameA[32];
    uint32 StaStrengthA;
    uint32 StaSucRateA;
    char StaNameB[32];
    uint32 StaStrengthB;
    uint32 StaSucRateB;
    char StaNameC[32];
    uint32 StaStrengthC;
    uint32 StaSucRateC;
    char StaNameD[32];
    uint32 StaStrengthD;
    uint32 StaSucRateD;
    uint32 IMPT0;
    uint32 IMPT1;
    uint32 IMPT2;
    uint32 IMPT3;
    uint32 T00;
    uint32 T01;
    uint32 T02;
    uint32 T03;
    uint32 T04;
    uint32 T10;
    uint32 T11;
    uint32 T12;
    uint32 T13;
    uint32 T14;
    uint32 T20;
    uint32 T21;
    uint32 T22;
    uint32 T23;
    uint32 T24;
    uint32 T30;
    uint32 T31;
    uint32 T32;
    uint32 T33;
    uint32 T34;
    uint32 EffectType;
    uint32 SpecialIndexA;
    uint32 SpecialValueA;
    uint32 SpecialIndexB;
    uint32 SpecialValueB;
    uint32 SpecialIndexC;
    uint32 SpecialValueC;
    uint32 SpecialIndexD;
    uint32 SpecialValueD;
    uint32 SpecialIndexE;
    uint32 SpecialValueE;
    char SkillClassifierA[32];
    char SkillClassifierB[32];
    char SkillClassifierC[32];
    uint8 CannotInside;
    uint8 DemandSoul;
    uint16 HitID;
};
class ActiveSkillTab : public ITableBase<ActiveSkillRow>, public IDataTable {
public: ActiveSkillTab() { ms_pkTable = this; }
    virtual bool Load(DataReader& r);
    virtual void BeforeTerminate() { ITableBase<ActiveSkillRow>::BeforeTerminate(); }
    virtual uint32 GetTotal() const { return ITableBase<ActiveSkillRow>::GetTotal(); }
    virtual const char* LogicalName() const { return "ActiveSkill"; }
};
extern ActiveSkillTab g_ActiveSkillTab;

// MobInfo.shn  -- 15 fields
struct MobInfoRow {
    uint32 Field_Name;
    char InxName[32];
    char Name[32];
    uint32 Level;
    uint32 MaxHP;
    uint32 WalkSpeed;
    uint32 RunSpeed;
    uint8 IsNPC;
    uint32 Size;
    uint32 WeaponType;
    uint32 ArmorType;
    uint32 GradeType;
    uint32 Type;
    uint8 IsPlayerSide;
    uint32 AbsoluteSize;
};
class MobInfoTab : public ITableBase<MobInfoRow>, public IDataTable {
public: MobInfoTab() { ms_pkTable = this; }
    virtual bool Load(DataReader& r);
    virtual void BeforeTerminate() { ITableBase<MobInfoRow>::BeforeTerminate(); }
    virtual uint32 GetTotal() const { return ITableBase<MobInfoRow>::GetTotal(); }
    virtual const char* LogicalName() const { return "MobInfo"; }
};
extern MobInfoTab g_MobInfoTab;

// ItemActionEffect.shn  -- 5 fields
struct ItemActionEffectRow {
    uint16 EffectID;
    uint32 EffectTarget;
    uint32 EffectActivity;
    uint16 Value;
    uint16 Area;
};
class ItemActionEffectTab : public ITableBase<ItemActionEffectRow>, public IDataTable {
public: ItemActionEffectTab() { ms_pkTable = this; }
    virtual bool Load(DataReader& r);
    virtual void BeforeTerminate() { ITableBase<ItemActionEffectRow>::BeforeTerminate(); }
    virtual uint32 GetTotal() const { return ITableBase<ItemActionEffectRow>::GetTotal(); }
    virtual const char* LogicalName() const { return "ItemActionEffect"; }
};
extern ItemActionEffectTab g_ItemActionEffectTab;

// ItemActionCondition.shn  -- 6 fields
struct ItemActionConditionRow {
    uint16 ConditionID;
    uint32 SubjectTarget;
    uint32 ObjectTarget;
    uint32 ConditionActivity;
    uint16 ActivityRate;
    uint16 Range;
};
class ItemActionConditionTab : public ITableBase<ItemActionConditionRow>, public IDataTable {
public: ItemActionConditionTab() { ms_pkTable = this; }
    virtual bool Load(DataReader& r);
    virtual void BeforeTerminate() { ITableBase<ItemActionConditionRow>::BeforeTerminate(); }
    virtual uint32 GetTotal() const { return ITableBase<ItemActionConditionRow>::GetTotal(); }
    virtual const char* LogicalName() const { return "ItemActionCondition"; }
};
extern ItemActionConditionTab g_ItemActionConditionTab;

// ItemInfoServer.shn  -- 25 fields
struct ItemInfoServerRow {
    uint32 ID;
    char InxName[32];
    char MarketIndex[20];
    uint32 Rou;
    uint32 Eld;
    uint32 Urg;
    uint32 All;
    uint32 Fer;
    uint32 Kas;
    uint32 Sad;
    uint32 Kor;
    uint32 Ver;
    uint32 Mys;
    char DropGroupA[40];
    char DropGroupB[40];
    char RandomOptionDropGroup[33];
    uint32 Vanish;
    uint32 looting;
    uint16 DropRateKilledByMob;
    uint16 DropRateKilledByPlayer;
    uint32 ISET_Index;
    uint8 KQItem;
    uint8 PK_KQ_USE;
    uint8 KQ_Item_Drop;
    uint8 PreventAttack;
};
class ItemInfoServerTab : public ITableBase<ItemInfoServerRow>, public IDataTable {
public: ItemInfoServerTab() { ms_pkTable = this; }
    virtual bool Load(DataReader& r);
    virtual void BeforeTerminate() { ITableBase<ItemInfoServerRow>::BeforeTerminate(); }
    virtual uint32 GetTotal() const { return ITableBase<ItemInfoServerRow>::GetTotal(); }
    virtual const char* LogicalName() const { return "ItemInfoServer"; }
};
extern ItemInfoServerTab g_ItemInfoServerTab;

// CharacterTitleData.shn  -- 16 fields
struct CharacterTitleDataRow {
    uint32 Type;
    uint32 REMARK;
    char Title0[32];
    uint32 Permit;
    uint32 Refresh;
    uint32 Value0;
    uint32 Fame0;
    char Title1[32];
    uint32 Value1;
    uint32 Fame1;
    char Title2[32];
    uint32 Value2;
    uint32 Fame2;
    char Title3[32];
    uint32 Value3;
    uint32 Fame3;
};
class CharacterTitleDataTab : public ITableBase<CharacterTitleDataRow>, public IDataTable {
public: CharacterTitleDataTab() { ms_pkTable = this; }
    virtual bool Load(DataReader& r);
    virtual void BeforeTerminate() { ITableBase<CharacterTitleDataRow>::BeforeTerminate(); }
    virtual uint32 GetTotal() const { return ITableBase<CharacterTitleDataRow>::GetTotal(); }
    virtual const char* LogicalName() const { return "CharacterTitleData"; }
};
extern CharacterTitleDataTab g_CharacterTitleDataTab;

// CollectCard.shn  -- 4 fields
struct CollectCardRow {
    uint16 CC_CardID;
    char CC_ItemInx[32];
    uint32 CC_CardGradeType;
    uint32 CC_CardMobGroup;
};
class CollectCardTab : public ITableBase<CollectCardRow>, public IDataTable {
public: CollectCardTab() { ms_pkTable = this; }
    virtual bool Load(DataReader& r);
    virtual void BeforeTerminate() { ITableBase<CollectCardRow>::BeforeTerminate(); }
    virtual uint32 GetTotal() const { return ITableBase<CollectCardRow>::GetTotal(); }
    virtual const char* LogicalName() const { return "CollectCard"; }
};
extern CollectCardTab g_CollectCardTab;

// ActionRangeFactor.shn  -- 4 fields
struct ActionRangeFactorRow {
    uint32 ActionRangeIndex;
    uint32 RangeType;
    uint16 RangeStart;
    uint16 RangeEnd;
};
class ActionRangeFactorTab : public ITableBase<ActionRangeFactorRow>, public IDataTable {
public: ActionRangeFactorTab() { ms_pkTable = this; }
    virtual bool Load(DataReader& r);
    virtual void BeforeTerminate() { ITableBase<ActionRangeFactorRow>::BeforeTerminate(); }
    virtual uint32 GetTotal() const { return ITableBase<ActionRangeFactorRow>::GetTotal(); }
    virtual const char* LogicalName() const { return "ActionRangeFactor"; }
};
extern ActionRangeFactorTab g_ActionRangeFactorTab;

// GTIServer.shn  -- 7 fields
struct GTIServerRow {
    uint8 ID;
    char InxName[33];
    uint32 SubjectTarget;
    uint8 EnemyNumber;
    uint32 GTIActionType;
    char Index[32];
    uint8 Value;
};
class GTIServerTab : public ITableBase<GTIServerRow>, public IDataTable {
public: GTIServerTab() { ms_pkTable = this; }
    virtual bool Load(DataReader& r);
    virtual void BeforeTerminate() { ITableBase<GTIServerRow>::BeforeTerminate(); }
    virtual uint32 GetTotal() const { return ITableBase<GTIServerRow>::GetTotal(); }
    virtual const char* LogicalName() const { return "GTIServer"; }
};
extern GTIServerTab g_GTIServerTab;

// GuildTournament.shn  -- 73 fields
struct GuildTournamentRow {
    uint32 GuildTournament;
    uint32 GuildTournamentReward;
    uint32 GuildTournamentRequire;
    uint32 GuildTournamentSkill;
    uint32 GuildTournamentSkillDesc;
    uint32 GuildTournamentLvGap;
    uint32 GuildTournamentOccupy;
    uint32 GuildTournamentMasterBuff;
    uint32 GuildTournamentScore;
    uint32 GTWinScore;
    uint8 isActive;
    uint8 Weeks;
    uint8 Week;
    uint8 Hour;
    uint8 Minute;
    uint8 TermHour;
    uint8 TermMinute;
    uint16 MatchCycleMin;
    uint16 ExploerTimeMin;
    uint16 WaitPlayTimeSec;
    uint16 PlayTime;
    uint16 Deadline;
    uint16 MatchDelay;
    uint16 Match_161;
    uint16 Match_162;
    uint16 Match_8;
    uint16 Match_4;
    uint16 Match_2;
    uint32 Totlal;
    uint8 Rank;
    uint8 RewardGroup;
    uint32 RewardType;
    uint32 Value1;
    uint32 Value2;
    uint32 Value3;
    uint32 IO_Str;
    uint32 IO_Con;
    uint32 IO_Dex;
    uint32 IO_Int;
    uint32 IO_Men;
    uint32 MinLv;
    uint32 MinMem;
    uint32 JoinMoney;
    uint16 MAP_TYPE;
    uint16 Index;
    uint16 DeathPoint;
    char StaName[32];
    uint32 TargetType;
    uint32 DlyTime;
    uint16 MAP_TYPE_1;
    uint16 Index_1;
    uint32 IconIndex;
    char IconFile[32];
    char Name[32];
    char Description[64];
    uint16 MAP_TYPE_2;
    float LvGap;
    uint16 PointRate;
    uint16 MAP_TYPE_3;
    uint32 MyGrade;
    uint32 EnemyGrade0;
    uint32 EnemyGrade1;
    uint32 EnemyGrade2;
    uint32 EnemyGrade3;
    uint32 EnemyGrade4;
    uint32 EnemyGrade5;
    uint32 EnemyGrade6;
    uint16 MAP_TYPE_4;
    uint16 OccupyTime;
    uint16 Score;
    uint16 MAP_TYPE_5;
    char StateName[32];
    uint32 WinScore;
};
class GuildTournamentTab : public ITableBase<GuildTournamentRow>, public IDataTable {
public: GuildTournamentTab() { ms_pkTable = this; }
    virtual bool Load(DataReader& r);
    virtual void BeforeTerminate() { ITableBase<GuildTournamentRow>::BeforeTerminate(); }
    virtual uint32 GetTotal() const { return ITableBase<GuildTournamentRow>::GetTotal(); }
    virtual const char* LogicalName() const { return "GuildTournament"; }
};
extern GuildTournamentTab g_GuildTournamentTab;

// StateField.shn  -- 3 fields
struct StateFieldRow {
    char AbStateInx[32];
    uint32 MapName;
    uint32 StateSet;
};
class StateFieldTab : public ITableBase<StateFieldRow>, public IDataTable {
public: StateFieldTab() { ms_pkTable = this; }
    virtual bool Load(DataReader& r);
    virtual void BeforeTerminate() { ITableBase<StateFieldRow>::BeforeTerminate(); }
    virtual uint32 GetTotal() const { return ITableBase<StateFieldRow>::GetTotal(); }
    virtual const char* LogicalName() const { return "StateField"; }
};
extern StateFieldTab g_StateFieldTab;

// MIDungeon.shn  -- 11 fields
struct MIDungeonRow {
    uint32 MID_MapInx;
    uint8 MID_MinLv;
    uint8 MID_MaxLv;
    uint8 MID_TankNum;
    uint8 MID_HealNum;
    uint8 MID_DealNum;
    uint8 MID_MinUser;
    uint8 MID_MaxUser;
    uint8 MID_MakeParty;
    uint8 MID_CompleteGroup;
    uint16 MID_StartTimeM;
};
class MIDungeonTab : public ITableBase<MIDungeonRow>, public IDataTable {
public: MIDungeonTab() { ms_pkTable = this; }
    virtual bool Load(DataReader& r);
    virtual void BeforeTerminate() { ITableBase<MIDungeonRow>::BeforeTerminate(); }
    virtual uint32 GetTotal() const { return ITableBase<MIDungeonRow>::GetTotal(); }
    virtual const char* LogicalName() const { return "MIDungeon"; }
};
extern MIDungeonTab g_MIDungeonTab;

// MIDServer.shn  -- 3 fields
struct MIDServerRow {
    uint32 MID_MapInx;
    char MID_ScriptLanguage[32];
    char MID_PR_Inx[32];
};
class MIDServerTab : public ITableBase<MIDServerRow>, public IDataTable {
public: MIDServerTab() { ms_pkTable = this; }
    virtual bool Load(DataReader& r);
    virtual void BeforeTerminate() { ITableBase<MIDServerRow>::BeforeTerminate(); }
    virtual uint32 GetTotal() const { return ITableBase<MIDServerRow>::GetTotal(); }
    virtual const char* LogicalName() const { return "MIDServer"; }
};
extern MIDServerTab g_MIDServerTab;

// ChargedEffect.shn  -- 6 fields
struct ChargedEffectRow {
    uint16 Handle;
    char ItemID[32];
    uint16 KeepTime_Hour;
    uint32 EffectEnum;
    uint16 EffectValue;
    uint8 StaStrength;
};
class ChargedEffectTab : public ITableBase<ChargedEffectRow>, public IDataTable {
public: ChargedEffectTab() { ms_pkTable = this; }
    virtual bool Load(DataReader& r);
    virtual void BeforeTerminate() { ITableBase<ChargedEffectRow>::BeforeTerminate(); }
    virtual uint32 GetTotal() const { return ITableBase<ChargedEffectRow>::GetTotal(); }
    virtual const char* LogicalName() const { return "ChargedEffect"; }
};
extern ChargedEffectTab g_ChargedEffectTab;

// AbStateView.shn  -- 21 fields
struct AbStateViewRow {
    uint32 ID;
    char inxName[32];
    uint32 icon;
    char iconFile[32];
    char Descript[256];
    uint8 R;
    uint8 G;
    uint8 B;
    char AniIndex[32];
    char effName[32];
    uint32 EffNamePos;
    uint8 EffRefresh;
    char LoopEffect[32];
    uint32 LoopEffPos;
    char LastEffect[32];
    uint32 LastEffectPos;
    char DOTEffect[32];
    uint32 DOTEffectPos;
    char IconSort[16];
    uint32 TypeSort;
    uint8 View;
};
class AbStateViewTab : public ITableBase<AbStateViewRow>, public IDataTable {
public: AbStateViewTab() { ms_pkTable = this; }
    virtual bool Load(DataReader& r);
    virtual void BeforeTerminate() { ITableBase<AbStateViewRow>::BeforeTerminate(); }
    virtual uint32 GetTotal() const { return ITableBase<AbStateViewRow>::GetTotal(); }
    virtual const char* LogicalName() const { return "AbStateView"; }
};
extern AbStateViewTab g_AbStateViewTab;

// SetEffect.shn  -- 3 fields
struct SetEffectRow {
    char SetItemIndex[32];
    uint8 Count;
    uint16 ItemActionID;
};
class SetEffectTab : public ITableBase<SetEffectRow>, public IDataTable {
public: SetEffectTab() { ms_pkTable = this; }
    virtual bool Load(DataReader& r);
    virtual void BeforeTerminate() { ITableBase<SetEffectRow>::BeforeTerminate(); }
    virtual uint32 GetTotal() const { return ITableBase<SetEffectRow>::GetTotal(); }
    virtual const char* LogicalName() const { return "SetEffect"; }
};
extern SetEffectTab g_SetEffectTab;

// GradeItemOption.shn  -- 20 fields
struct GradeItemOptionRow {
    uint32 ItemIndex;
    uint16 STR;
    uint16 CON;
    uint16 DEX;
    uint16 INT;
    uint16 MEN;
    uint16 ResistPoison;
    uint16 ResistDeaseas;
    uint16 ResistCurse;
    uint16 ResistMoveSpdDown;
    uint16 Critical;
    uint16 ToHitRate;
    uint16 ToHitPlus;
    uint16 ToBlockRate;
    uint16 ToBlockPlus;
    uint16 MaxHP;
    uint16 MaxSP;
    uint16 MoveSpdRate;
    uint16 AbsoluteAttack;
    uint16 PickupLimit;
};
class GradeItemOptionTab : public ITableBase<GradeItemOptionRow>, public IDataTable {
public: GradeItemOptionTab() { ms_pkTable = this; }
    virtual bool Load(DataReader& r);
    virtual void BeforeTerminate() { ITableBase<GradeItemOptionRow>::BeforeTerminate(); }
    virtual uint32 GetTotal() const { return ITableBase<GradeItemOptionRow>::GetTotal(); }
    virtual const char* LogicalName() const { return "GradeItemOption"; }
};
extern GradeItemOptionTab g_GradeItemOptionTab;

// =============================================================================
// MobInfoServer.shn / ItemDropTable.shn / RandomOption.shn -- runtime drop pipeline.
// Minimal-field stubs so DropResolver / EquipSummaryBuilder compile against
// real ITableBase plumbing. Full schemas are filled in by the SchemaGen
// pass once the SHN headers are confirmed.
// =============================================================================
struct MobInfoServerRow {
    uint16 InxNo;            // mob species id (links to MobInfo.shn.InxNo)
    uint16 DropItemListIdx;  // drop-table id (links to ItemDropTable.DropTableID)
    uint16 ResistanceIdx;    // links to MobResist.shn
    uint16 BehaviorIdx;      // links to MobBehavior id (PS / Lua)
    uint16 RoamIdx;          // links to MobRoam id
    uint16 AttackSeqIdx;     // links to MobAttackSequence id
    uint16 ActionSetIdx;     // links to MobSetting/Action id

    // EnemyDetectType: 0=passive, !=0 active; AggroList consults this gate.
    // DetectCha:       aggro radius (game units); the AI uses this as the
    //                  "noticed me" distance before any level-gap test.
    // FollowCha:       chase distance; once the target leaves this radius
    //                  the mob disengages and the aggro list decays.
    uint32 EnemyDetectType;
    uint16 DetectCha;
    uint32 FollowCha;
};
class MobInfoServerTab : public ITableBase<MobInfoServerRow>, public IDataTable {
public: MobInfoServerTab() { ms_pkTable = this; }
    virtual bool Load(DataReader& r);
    virtual void BeforeTerminate() { ITableBase<MobInfoServerRow>::BeforeTerminate(); }
    virtual uint32 GetTotal() const { return ITableBase<MobInfoServerRow>::GetTotal(); }
    virtual const char* LogicalName() const { return "MobInfoServer"; }
};
extern MobInfoServerTab g_MobInfoServerTab;

struct ItemDropTableRow {
    uint16      DropTableID;
    std::string ItemGroupIdx;
    uint16      Permill;
    uint16      MinQty;
    uint16      MaxQty;
};
class ItemDropTableTab : public ITableBase<ItemDropTableRow>, public IDataTable {
public: ItemDropTableTab() { ms_pkTable = this; }
    virtual bool Load(DataReader& r);
    virtual void BeforeTerminate() { ITableBase<ItemDropTableRow>::BeforeTerminate(); }
    virtual uint32 GetTotal() const { return ITableBase<ItemDropTableRow>::GetTotal(); }
    virtual const char* LogicalName() const { return "ItemDropTable"; }
};
extern ItemDropTableTab g_ItemDropTableTab;

void RegisterAllSchemaTabs();
} // namespace fiesta
#endif