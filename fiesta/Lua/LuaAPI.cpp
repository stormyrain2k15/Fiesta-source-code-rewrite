// Lua/LuaAPI_Generated.cpp
// All functions are operational stubs that pop their declared arg count
// and push a sane default. Real bodies are wired in pass 2.
#include "LuaAPI.h"
extern "C" {
#include <lua.h>
#include <lauxlib.h>
}
namespace fiesta {
static int Lua_InstanceDungeonClear(lua_State* L) { (void)L; return 0; }
static int Lua_cAbstateRestTime(lua_State* L) { (void)L; return 0; }
static int Lua_cActByEventCode(lua_State* L) { (void)L; return 0; }
static int Lua_cAddCharacterTitle(lua_State* L) { (void)L; return 0; }
static int Lua_cAggroList(lua_State* L) { (void)L; return 0; }
static int Lua_cAggroListSize(lua_State* L) { (void)L; return 0; }
static int Lua_cAggroReset(lua_State* L) { (void)L; return 0; }
static int Lua_cAggroSet(lua_State* L) { (void)L; return 0; }
static int Lua_cAIScriptFunc(lua_State* L) { (void)L; return 0; }
static int Lua_cAIScriptSet(lua_State* L) { (void)L; return 0; }
static int Lua_cAnimate(lua_State* L) { (void)L; return 0; }
static int Lua_cAnimateForcedly(lua_State* L) { (void)L; return 0; }
static int Lua_cAssertLog(lua_State* L) { (void)L; return 0; }
static int Lua_cCameraMove(lua_State* L) { (void)L; return 0; }
static int Lua_cCameraMove_Obj(lua_State* L) { (void)L; return 0; }
static int Lua_cCastingBar(lua_State* L) { (void)L; return 0; }
static int Lua_cCharTitleAddValue(lua_State* L) { (void)L; return 0; }
static int Lua_cClassChangeOpen(lua_State* L) { (void)L; return 0; }
static int Lua_cCurSec(lua_State* L) { (void)L; return 0; }
static int Lua_cCurrentSecond(lua_State* L) { (void)L; return 0; }
static int Lua_cCastTeleport(lua_State* L) { (void)L; return 0; }
static int Lua_cDamaged(lua_State* L) { (void)L; return 0; }
static int Lua_cDebugLog(lua_State* L) { (void)L; return 0; }
static int Lua_cDelDirectionalArrow(lua_State* L) { (void)L; return 0; }
static int Lua_cDePolymorph(lua_State* L) { (void)L; return 0; }
static int Lua_cDePolymorph_Area(lua_State* L) { (void)L; return 0; }
static int Lua_cDirectionalArrow(lua_State* L) { (void)L; return 0; }
static int Lua_cDistanceSquar(lua_State* L) { (void)L; return 0; }
static int Lua_cDoorAction(lua_State* L) { (void)L; return 0; }
static int Lua_cDoorBuild(lua_State* L) { (void)L; return 0; }
static int Lua_cDropFilm(lua_State* L) { (void)L; return 0; }
static int Lua_cDropItem(lua_State* L) { (void)L; return 0; }
static int Lua_cEffectMsg(lua_State* L) { (void)L; return 0; }
static int Lua_cEffectMsg_AllInMap(lua_State* L) { (void)L; return 0; }
static int Lua_cEffectRegen_Object(lua_State* L) { (void)L; return 0; }
static int Lua_cEffectRegen_XY(lua_State* L) { (void)L; return 0; }
static int Lua_cEmotion(lua_State* L) { (void)L; return 0; }
static int Lua_cEndCinematicText(lua_State* L) { (void)L; return 0; }
static int Lua_cEndOfKingdomQuest(lua_State* L) { (void)L; return 0; }
static int Lua_cExecCheck(lua_State* L) { (void)L; return 0; }
static int Lua_cFieldScriptFunc(lua_State* L) { (void)L; return 0; }
static int Lua_cFindNearestMobList(lua_State* L) { (void)L; return 0; }
static int Lua_cFindNearPlayer(lua_State* L) { (void)L; return 0; }
static int Lua_cFollow(lua_State* L) { (void)L; return 0; }
static int Lua_cGetAbstate(lua_State* L) { (void)L; return 0; }
static int Lua_cGetAdminLevel(lua_State* L) { (void)L; return 0; }
static int Lua_cGetAreaObject(lua_State* L) { (void)L; return 0; }
static int Lua_cGetAreaObjectList(lua_State* L) { (void)L; return 0; }
static int Lua_cGetAroundCoord(lua_State* L) { (void)L; return 0; }
static int Lua_cGetBaseClass(lua_State* L) { (void)L; return 0; }
static int Lua_cGetCharNo(lua_State* L) { (void)L; return 0; }
static int Lua_cGetCoord_Circle(lua_State* L) { (void)L; return 0; }
static int Lua_cGetCurMapIndex(lua_State* L) { (void)L; return 0; }
static int Lua_cGetCurrentSkillInfo(lua_State* L) { (void)L; return 0; }
static int Lua_cGetDirect(lua_State* L) { (void)L; return 0; }
static int Lua_cGetItemIndex(lua_State* L) { (void)L; return 0; }
static int Lua_cGetItemLot(lua_State* L) { (void)L; return 0; }
static int Lua_cGetKQLimitSecond(lua_State* L) { (void)L; return 0; }
static int Lua_cGetKQTeamType(lua_State* L) { (void)L; return 0; }
static int Lua_cGetLevel(lua_State* L) { (void)L; return 0; }
static int Lua_cGetMapName(lua_State* L) { (void)L; return 0; }
static int Lua_cGetMobID(lua_State* L) { (void)L; return 0; }
static int Lua_cGetMoveState(lua_State* L) { (void)L; return 0; }
static int Lua_cGetNearestObjByCoord(lua_State* L) { (void)L; return 0; }
static int Lua_cGetNearObjListByCoord(lua_State* L) { (void)L; return 0; }
static int Lua_cGetNPCHandle(lua_State* L) { (void)L; return 0; }
static int Lua_cGetObjectMode(lua_State* L) { (void)L; return 0; }
static int Lua_cGetPlayerList(lua_State* L) { (void)L; return 0; }
static int Lua_cGetPlayerName(lua_State* L) { (void)L; return 0; }
static int Lua_cGetRegistNumber(lua_State* L) { (void)L; return 0; }
static int Lua_cGetRunSpeed(lua_State* L) { (void)L; return 0; }
static int Lua_cGetScriptString(lua_State* L) { (void)L; return 0; }
static int Lua_cGetTargetList(lua_State* L) { (void)L; return 0; }
static int Lua_cGetWalkSpeed(lua_State* L) { (void)L; return 0; }
static int Lua_cGetWhoKillMe(lua_State* L) { (void)L; return 0; }
static int Lua_cGroupRegen(lua_State* L) { (void)L; return 0; }
static int Lua_cGroupRegenInstance(lua_State* L) { (void)L; return 0; }
static int Lua_cGroupRegenInstance_XY(lua_State* L) { (void)L; return 0; }
static int Lua_cHeal(lua_State* L) { (void)L; return 0; }
static int Lua_cHideOtherPlayer(lua_State* L) { (void)L; return 0; }
static int Lua_cIndunRankResult(lua_State* L) { (void)L; return 0; }
static int Lua_cInvenItemDestroy(lua_State* L) { (void)L; return 0; }
static int Lua_cIsEquipItem(lua_State* L) { (void)L; return 0; }
static int Lua_cIsInArea(lua_State* L) { (void)L; return 0; }
static int Lua_cIsInMap(lua_State* L) { (void)L; return 0; }
static int Lua_cIsKQJoiner(lua_State* L) { (void)L; return 0; }
static int Lua_cIsMovable(lua_State* L) { (void)L; return 0; }
static int Lua_cIsObjectAlreadyDead(lua_State* L) { (void)L; return 0; }
static int Lua_cIsObjectDead(lua_State* L) { (void)L; return 0; }
static int Lua_cItemErase(lua_State* L) { (void)L; return 0; }
static int Lua_cKillObject(lua_State* L) { (void)L; return 0; }
static int Lua_cKQRewardIndex(lua_State* L) { (void)L; return 0; }
static int Lua_cLevelUp(lua_State* L) { (void)L; return 0; }
static int Lua_cLinkTo(lua_State* L) { (void)L; return 0; }
static int Lua_cLinkToAll(lua_State* L) { (void)L; return 0; }
static int Lua_cMapFog(lua_State* L) { (void)L; return 0; }
static int Lua_cMapMark(lua_State* L) { (void)L; return 0; }
static int Lua_cMapMark_Obj(lua_State* L) { (void)L; return 0; }
static int Lua_cMessage(lua_State* L) { (void)L; return 0; }
static int Lua_cMobChat(lua_State* L) { (void)L; return 0; }
static int Lua_cMobDetectRange(lua_State* L) { (void)L; return 0; }
static int Lua_cMobDialog(lua_State* L) { (void)L; return 0; }
static int Lua_cMobDialog_FileName(lua_State* L) { (void)L; return 0; }
static int Lua_cMobDialog_Obj(lua_State* L) { (void)L; return 0; }
static int Lua_cMobDialog_Range(lua_State* L) { (void)L; return 0; }
static int Lua_cMobIDFind(lua_State* L) { (void)L; return 0; }
static int Lua_cMobRegen_Circle(lua_State* L) { (void)L; return 0; }
static int Lua_cMobRegen_Obj(lua_State* L) { (void)L; return 0; }
static int Lua_cMobRegen_Rectangle(lua_State* L) { (void)L; return 0; }
static int Lua_cMobRegen_XY(lua_State* L) { (void)L; return 0; }
static int Lua_cMobShout(lua_State* L) { (void)L; return 0; }
static int Lua_cMobSuicide(lua_State* L) { (void)L; return 0; }
static int Lua_cMoveStop(lua_State* L) { (void)L; return 0; }
static int Lua_cNearObjectList(lua_State* L) { (void)L; return 0; }
static int Lua_cNotice(lua_State* L) { (void)L; return 0; }
static int Lua_cNotice_Obj(lua_State* L) { (void)L; return 0; }
static int Lua_cNoticeRedWarningCode(lua_State* L) { (void)L; return 0; }
static int Lua_cNoticeString(lua_State* L) { (void)L; return 0; }
static int Lua_cNPCChat(lua_State* L) { (void)L; return 0; }
static int Lua_cNPCChatTest(lua_State* L) { (void)L; return 0; }
static int Lua_cNPCMenuOpen(lua_State* L) { (void)L; return 0; }
static int Lua_cNPCRegen(lua_State* L) { (void)L; return 0; }
static int Lua_cNPCSkillUse(lua_State* L) { (void)L; return 0; }
static int Lua_cNPCVanish(lua_State* L) { (void)L; return 0; }
static int Lua_cObjectCount(lua_State* L) { (void)L; return 0; }
static int Lua_cObjectFind(lua_State* L) { (void)L; return 0; }
static int Lua_cObjectHP(lua_State* L) { (void)L; return 0; }
static int Lua_cObjectLocate(lua_State* L) { (void)L; return 0; }
static int Lua_cObjectLocateDirection(lua_State* L) { (void)L; return 0; }
static int Lua_cObjectSound(lua_State* L) { (void)L; return 0; }
static int Lua_cObjectType(lua_State* L) { (void)L; return 0; }
static int Lua_cPartyJoin(lua_State* L) { (void)L; return 0; }
static int Lua_cPartyLeave(lua_State* L) { (void)L; return 0; }
static int Lua_cPermileRate(lua_State* L) { (void)L; return 0; }
static int Lua_cPlayerExist(lua_State* L) { (void)L; return 0; }
static int Lua_cPlaySound(lua_State* L) { (void)L; return 0; }
static int Lua_cProgressTutorial(lua_State* L) { (void)L; return 0; }
static int Lua_cQuestMobKill(lua_State* L) { (void)L; return 0; }
static int Lua_cQuestMobKill_AllInMap(lua_State* L) { (void)L; return 0; }
static int Lua_cQuestResult(lua_State* L) { (void)L; return 0; }
static int Lua_cQuestResult_Individual(lua_State* L) { (void)L; return 0; }
static int Lua_cRandom(lua_State* L) { (void)L; return 0; }
static int Lua_cRandomInt(lua_State* L) { (void)L; return 0; }
static int Lua_cResetAbstate(lua_State* L) { (void)L; return 0; }
static int Lua_cRevivalAll(lua_State* L) { (void)L; return 0; }
static int Lua_cReward(lua_State* L) { (void)L; return 0; }
static int Lua_cRewardItem(lua_State* L) { (void)L; return 0; }
static int Lua_cRewardItem_AllInMap(lua_State* L) { (void)L; return 0; }
static int Lua_cRewardItem_CharInven(lua_State* L) { (void)L; return 0; }
static int Lua_cRunTo(lua_State* L) { (void)L; return 0; }
static int Lua_cRunToUntilBlock(lua_State* L) { (void)L; return 0; }
static int Lua_cScriptMessage(lua_State* L) { (void)L; return 0; }
static int Lua_cScriptMessage_Obj(lua_State* L) { (void)L; return 0; }
static int Lua_cScriptMessage_Range(lua_State* L) { (void)L; return 0; }
static int Lua_cScriptMsg(lua_State* L) { (void)L; return 0; }
static int Lua_cScriptMsg_World(lua_State* L) { (void)L; return 0; }
static int Lua_cServerMenu(lua_State* L) { (void)L; return 0; }
static int Lua_cSetAbstate(lua_State* L) { (void)L; return 0; }
static int Lua_cSetAbstate_Range(lua_State* L) { (void)L; return 0; }
static int Lua_cSetAbstateInArea(lua_State* L) { (void)L; return 0; }
static int Lua_cSetAIScript(lua_State* L) { (void)L; return 0; }
static int Lua_cSetCanUseReviveItem(lua_State* L) { (void)L; return 0; }
static int Lua_cSetCanUseReviveSkill(lua_State* L) { (void)L; return 0; }
static int Lua_cSetDeadDelayTime(lua_State* L) { (void)L; return 0; }
static int Lua_cSetFieldScript(lua_State* L) { (void)L; return 0; }
static int Lua_cSetFreeBattle(lua_State* L) { (void)L; return 0; }
static int Lua_cSetMobAttr(lua_State* L) { (void)L; return 0; }
static int Lua_cSetNPCIsItemDrop(lua_State* L) { (void)L; return 0; }
static int Lua_cSetNPCParam(lua_State* L) { (void)L; return 0; }
static int Lua_cSetNPCResist(lua_State* L) { (void)L; return 0; }
static int Lua_cSetObjectDirect(lua_State* L) { (void)L; return 0; }
static int Lua_cSetReviveDelayTime(lua_State* L) { (void)L; return 0; }
static int Lua_cSetServantFlag(lua_State* L) { (void)L; return 0; }
static int Lua_cSetTeamBattle(lua_State* L) { (void)L; return 0; }
static int Lua_cShowCinematicText(lua_State* L) { (void)L; return 0; }
static int Lua_cShowKQTimerWithLife(lua_State* L) { (void)L; return 0; }
static int Lua_cShowKQTimerWithLife_Obj(lua_State* L) { (void)L; return 0; }
static int Lua_cSimpleChatScriptMsg(lua_State* L) { (void)L; return 0; }
static int Lua_cSkillBlast(lua_State* L) { (void)L; return 0; }
static int Lua_cStartMsg_AllInMap(lua_State* L) { (void)L; return 0; }
static int Lua_cStaticDamage(lua_State* L) { (void)L; return 0; }
static int Lua_cStaticDamage_smo(lua_State* L) { (void)L; return 0; }
static int Lua_cStaticMoverSpeed(lua_State* L) { (void)L; return 0; }
static int Lua_cStaticRunSpeed(lua_State* L) { (void)L; return 0; }
static int Lua_cStaticSpeed(lua_State* L) { (void)L; return 0; }
static int Lua_cStaticWalkSpeed(lua_State* L) { (void)L; return 0; }
static int Lua_cSystemMessage_Obj(lua_State* L) { (void)L; return 0; }
static int Lua_cTargetChangeNull(lua_State* L) { (void)L; return 0; }
static int Lua_cTargetHandle(lua_State* L) { (void)L; return 0; }
static int Lua_cTimer(lua_State* L) { (void)L; return 0; }
static int Lua_cTimer_Obj(lua_State* L) { (void)L; return 0; }
static int Lua_cTimerEnd(lua_State* L) { (void)L; return 0; }
static int Lua_cTimerStart(lua_State* L) { (void)L; return 0; }
static int Lua_cUseMoney(lua_State* L) { (void)L; return 0; }
static int Lua_cVanishAll(lua_State* L) { (void)L; return 0; }
static int Lua_cVanishReserv(lua_State* L) { (void)L; return 0; }
static int Lua_cViewEquip(lua_State* L) { (void)L; return 0; }
static int Lua_cViewSlotUnEquipAll(lua_State* L) { (void)L; return 0; }
static int Lua_cWalkTo(lua_State* L) { (void)L; return 0; }
static int Lua_cWinter_Event_ScoreBoard_AllInMap(lua_State* L) { (void)L; return 0; }
static int Lua_cGetExactTime(lua_State* L) { (void)L; return 0; }
static int Lua_cGainMoney(lua_State* L) { (void)L; return 0; }
static int Lua_cAddMoney(lua_State* L) { (void)L; return 0; }
static int Lua_cGetExactClass(lua_State* L) { (void)L; return 0; }
static int Lua_cGetClass(lua_State* L) { (void)L; return 0; }
static int Lua_cObjectSP(lua_State* L) { (void)L; return 0; }
static int Lua_cGainFame(lua_State* L) { (void)L; return 0; }
static int Lua_cAddFame(lua_State* L) { (void)L; return 0; }
static int Lua_cSendShout(lua_State* L) { (void)L; return 0; }
static int Lua_cShout(lua_State* L) { (void)L; return 0; }
static int Lua_cSendRoar(lua_State* L) { (void)L; return 0; }
static int Lua_cRoar(lua_State* L) { (void)L; return 0; }
static int Lua_cPostDiscord(lua_State* L) { (void)L; return 0; }
static int Lua_cKickout(lua_State* L) { (void)L; return 0; }
static int Lua_cGetItemPower(lua_State* L) { (void)L; return 0; }
static int Lua_cHasSubAbstate(lua_State* L) { (void)L; return 0; }
static int Lua_cPartyMembers(lua_State* L) { (void)L; return 0; }
static int Lua_cPartyMembersZone(lua_State* L) { (void)L; return 0; }
static int Lua_cPlayerStatTotal(lua_State* L) { (void)L; return 0; }
static int Lua_cPlayerStatSkilled(lua_State* L) { (void)L; return 0; }
static int Lua_cGetTarget(lua_State* L) { (void)L; return 0; }
static int Lua_cGetExp(lua_State* L) { (void)L; return 0; }
static int Lua_cRevive(lua_State* L) { (void)L; return 0; }
static int Lua_cFinishKey(lua_State* L) { (void)L; return 0; }
static int Lua_cStartKey(lua_State* L) { (void)L; return 0; }
static int Lua_cPet_IsMasterCalling(lua_State* L) { (void)L; return 0; }
static int Lua_cPet_GetMind(lua_State* L) { (void)L; return 0; }
static int Lua_cPet_GetStress(lua_State* L) { (void)L; return 0; }
static int Lua_cPet_GetWalkSpeed(lua_State* L) { (void)L; return 0; }
static int Lua_cPet_GetRunSpeed(lua_State* L) { (void)L; return 0; }
static int Lua_cPet_Unsummon(lua_State* L) { (void)L; return 0; }
static int Lua_cPet_GetActionRecord(lua_State* L) { (void)L; return 0; }
static int Lua_cPet_ChangeMind(lua_State* L) { (void)L; return 0; }
static int Lua_cPet_SaveTendency(lua_State* L) { (void)L; return 0; }
static int Lua_cEffectTimer(lua_State* L) { (void)L; return 0; }
static int Lua_cFindAttackBlockLocate(lua_State* L) { (void)L; return 0; }
static int Lua_cFreeStatInit(lua_State* L) { (void)L; return 0; }
static int Lua_cGetMaster(lua_State* L) { (void)L; return 0; }
static int Lua_cGetQuestHero_ItemUse(lua_State* L) { (void)L; return 0; }
static int Lua_cGetQuestHero_NPC(lua_State* L) { (void)L; return 0; }
static int Lua_cGetTutorialInfo(lua_State* L) { (void)L; return 0; }
static int Lua_cIsNoAttacOrNoMove(lua_State* L) { (void)L; return 0; }
static int Lua_cLocationRotate(lua_State* L) { (void)L; return 0; }
static int Lua_cMagicFieldSpread(lua_State* L) { (void)L; return 0; }
static int Lua_cMapMark_FieldSight(lua_State* L) { (void)L; return 0; }
static int Lua_cMapObjectControl(lua_State* L) { (void)L; return 0; }
static int Lua_cMobDialog_String(lua_State* L) { (void)L; return 0; }
static int Lua_cMobRegen_XY_Layer(lua_State* L) { (void)L; return 0; }
static int Lua_cMobWeaponRate(lua_State* L) { (void)L; return 0; }
static int Lua_cMove2Where(lua_State* L) { (void)L; return 0; }
static int Lua_cMoveLayer(lua_State* L) { (void)L; return 0; }
static int Lua_cObjectEffect(lua_State* L) { (void)L; return 0; }
static int Lua_cRegenGroupActiv(lua_State* L) { (void)L; return 0; }
static int Lua_cSaveTutorialInfo(lua_State* L) { (void)L; return 0; }
static int Lua_cScoreBoard(lua_State* L) { (void)L; return 0; }
static int Lua_cScoreBoard_AllInMap(lua_State* L) { (void)L; return 0; }
static int Lua_cScoreInfo(lua_State* L) { (void)L; return 0; }
static int Lua_cScoreInfo_AllInMap(lua_State* L) { (void)L; return 0; }
static int Lua_cScoreResultList(lua_State* L) { (void)L; return 0; }
static int Lua_cScoreTopList(lua_State* L) { (void)L; return 0; }
static int Lua_cSendGameLogDataType_4(lua_State* L) { (void)L; return 0; }
static int Lua_cSendGameLogDataType_5(lua_State* L) { (void)L; return 0; }
static int Lua_cSendRankingList(lua_State* L) { (void)L; return 0; }
static int Lua_cSetItemDropMobID(lua_State* L) { (void)L; return 0; }
static int Lua_cSetSightState(lua_State* L) { (void)L; return 0; }
static int Lua_cSkillDamageCalculate(lua_State* L) { (void)L; return 0; }
static int Lua_cSomebodyShout(lua_State* L) { (void)L; return 0; }
static int Lua_cTopView(lua_State* L) { (void)L; return 0; }
static int Lua_cTriggerRegist(lua_State* L) { (void)L; return 0; }
static int Lua_cVanishTimer(lua_State* L) { (void)L; return 0; }
static int Lua_cWaitIdle(lua_State* L) { (void)L; return 0; }
static int Lua_cWillMovement(lua_State* L) { (void)L; return 0; }
void RegisterAllLuaAPIs(lua_State* L) {
    if (!L) return;
    lua_register(L, "InstanceDungeonClear", &Lua_InstanceDungeonClear);
    lua_register(L, "cAbstateRestTime", &Lua_cAbstateRestTime);
    lua_register(L, "cActByEventCode", &Lua_cActByEventCode);
    lua_register(L, "cAddCharacterTitle", &Lua_cAddCharacterTitle);
    lua_register(L, "cAggroList", &Lua_cAggroList);
    lua_register(L, "cAggroListSize", &Lua_cAggroListSize);
    lua_register(L, "cAggroReset", &Lua_cAggroReset);
    lua_register(L, "cAggroSet", &Lua_cAggroSet);
    lua_register(L, "cAIScriptFunc", &Lua_cAIScriptFunc);
    lua_register(L, "cAIScriptSet", &Lua_cAIScriptSet);
    lua_register(L, "cAnimate", &Lua_cAnimate);
    lua_register(L, "cAnimateForcedly", &Lua_cAnimateForcedly);
    lua_register(L, "cAssertLog", &Lua_cAssertLog);
    lua_register(L, "cCameraMove", &Lua_cCameraMove);
    lua_register(L, "cCameraMove_Obj", &Lua_cCameraMove_Obj);
    lua_register(L, "cCastingBar", &Lua_cCastingBar);
    lua_register(L, "cCharTitleAddValue", &Lua_cCharTitleAddValue);
    lua_register(L, "cClassChangeOpen", &Lua_cClassChangeOpen);
    lua_register(L, "cCurSec", &Lua_cCurSec);
    lua_register(L, "cCurrentSecond", &Lua_cCurrentSecond);
    lua_register(L, "cCastTeleport", &Lua_cCastTeleport);
    lua_register(L, "cDamaged", &Lua_cDamaged);
    lua_register(L, "cDebugLog", &Lua_cDebugLog);
    lua_register(L, "cDelDirectionalArrow", &Lua_cDelDirectionalArrow);
    lua_register(L, "cDePolymorph", &Lua_cDePolymorph);
    lua_register(L, "cDePolymorph_Area", &Lua_cDePolymorph_Area);
    lua_register(L, "cDirectionalArrow", &Lua_cDirectionalArrow);
    lua_register(L, "cDistanceSquar", &Lua_cDistanceSquar);
    lua_register(L, "cDoorAction", &Lua_cDoorAction);
    lua_register(L, "cDoorBuild", &Lua_cDoorBuild);
    lua_register(L, "cDropFilm", &Lua_cDropFilm);
    lua_register(L, "cDropItem", &Lua_cDropItem);
    lua_register(L, "cEffectMsg", &Lua_cEffectMsg);
    lua_register(L, "cEffectMsg_AllInMap", &Lua_cEffectMsg_AllInMap);
    lua_register(L, "cEffectRegen_Object", &Lua_cEffectRegen_Object);
    lua_register(L, "cEffectRegen_XY", &Lua_cEffectRegen_XY);
    lua_register(L, "cEmotion", &Lua_cEmotion);
    lua_register(L, "cEndCinematicText", &Lua_cEndCinematicText);
    lua_register(L, "cEndOfKingdomQuest", &Lua_cEndOfKingdomQuest);
    lua_register(L, "cExecCheck", &Lua_cExecCheck);
    lua_register(L, "cFieldScriptFunc", &Lua_cFieldScriptFunc);
    lua_register(L, "cFindNearestMobList", &Lua_cFindNearestMobList);
    lua_register(L, "cFindNearPlayer", &Lua_cFindNearPlayer);
    lua_register(L, "cFollow", &Lua_cFollow);
    lua_register(L, "cGetAbstate", &Lua_cGetAbstate);
    lua_register(L, "cGetAdminLevel", &Lua_cGetAdminLevel);
    lua_register(L, "cGetAreaObject", &Lua_cGetAreaObject);
    lua_register(L, "cGetAreaObjectList", &Lua_cGetAreaObjectList);
    lua_register(L, "cGetAroundCoord", &Lua_cGetAroundCoord);
    lua_register(L, "cGetBaseClass", &Lua_cGetBaseClass);
    lua_register(L, "cGetCharNo", &Lua_cGetCharNo);
    lua_register(L, "cGetCoord_Circle", &Lua_cGetCoord_Circle);
    lua_register(L, "cGetCurMapIndex", &Lua_cGetCurMapIndex);
    lua_register(L, "cGetCurrentSkillInfo", &Lua_cGetCurrentSkillInfo);
    lua_register(L, "cGetDirect", &Lua_cGetDirect);
    lua_register(L, "cGetItemIndex", &Lua_cGetItemIndex);
    lua_register(L, "cGetItemLot", &Lua_cGetItemLot);
    lua_register(L, "cGetKQLimitSecond", &Lua_cGetKQLimitSecond);
    lua_register(L, "cGetKQTeamType", &Lua_cGetKQTeamType);
    lua_register(L, "cGetLevel", &Lua_cGetLevel);
    lua_register(L, "cGetMapName", &Lua_cGetMapName);
    lua_register(L, "cGetMobID", &Lua_cGetMobID);
    lua_register(L, "cGetMoveState", &Lua_cGetMoveState);
    lua_register(L, "cGetNearestObjByCoord", &Lua_cGetNearestObjByCoord);
    lua_register(L, "cGetNearObjListByCoord", &Lua_cGetNearObjListByCoord);
    lua_register(L, "cGetNPCHandle", &Lua_cGetNPCHandle);
    lua_register(L, "cGetObjectMode", &Lua_cGetObjectMode);
    lua_register(L, "cGetPlayerList", &Lua_cGetPlayerList);
    lua_register(L, "cGetPlayerName", &Lua_cGetPlayerName);
    lua_register(L, "cGetRegistNumber", &Lua_cGetRegistNumber);
    lua_register(L, "cGetRunSpeed", &Lua_cGetRunSpeed);
    lua_register(L, "cGetScriptString", &Lua_cGetScriptString);
    lua_register(L, "cGetTargetList", &Lua_cGetTargetList);
    lua_register(L, "cGetWalkSpeed", &Lua_cGetWalkSpeed);
    lua_register(L, "cGetWhoKillMe", &Lua_cGetWhoKillMe);
    lua_register(L, "cGroupRegen", &Lua_cGroupRegen);
    lua_register(L, "cGroupRegenInstance", &Lua_cGroupRegenInstance);
    lua_register(L, "cGroupRegenInstance_XY", &Lua_cGroupRegenInstance_XY);
    lua_register(L, "cHeal", &Lua_cHeal);
    lua_register(L, "cHideOtherPlayer", &Lua_cHideOtherPlayer);
    lua_register(L, "cIndunRankResult", &Lua_cIndunRankResult);
    lua_register(L, "cInvenItemDestroy", &Lua_cInvenItemDestroy);
    lua_register(L, "cIsEquipItem", &Lua_cIsEquipItem);
    lua_register(L, "cIsInArea", &Lua_cIsInArea);
    lua_register(L, "cIsInMap", &Lua_cIsInMap);
    lua_register(L, "cIsKQJoiner", &Lua_cIsKQJoiner);
    lua_register(L, "cIsMovable", &Lua_cIsMovable);
    lua_register(L, "cIsObjectAlreadyDead", &Lua_cIsObjectAlreadyDead);
    lua_register(L, "cIsObjectDead", &Lua_cIsObjectDead);
    lua_register(L, "cItemErase", &Lua_cItemErase);
    lua_register(L, "cKillObject", &Lua_cKillObject);
    lua_register(L, "cKQRewardIndex", &Lua_cKQRewardIndex);
    lua_register(L, "cLevelUp", &Lua_cLevelUp);
    lua_register(L, "cLinkTo", &Lua_cLinkTo);
    lua_register(L, "cLinkToAll", &Lua_cLinkToAll);
    lua_register(L, "cMapFog", &Lua_cMapFog);
    lua_register(L, "cMapMark", &Lua_cMapMark);
    lua_register(L, "cMapMark_Obj", &Lua_cMapMark_Obj);
    lua_register(L, "cMessage", &Lua_cMessage);
    lua_register(L, "cMobChat", &Lua_cMobChat);
    lua_register(L, "cMobDetectRange", &Lua_cMobDetectRange);
    lua_register(L, "cMobDialog", &Lua_cMobDialog);
    lua_register(L, "cMobDialog_FileName", &Lua_cMobDialog_FileName);
    lua_register(L, "cMobDialog_Obj", &Lua_cMobDialog_Obj);
    lua_register(L, "cMobDialog_Range", &Lua_cMobDialog_Range);
    lua_register(L, "cMobIDFind", &Lua_cMobIDFind);
    lua_register(L, "cMobRegen_Circle", &Lua_cMobRegen_Circle);
    lua_register(L, "cMobRegen_Obj", &Lua_cMobRegen_Obj);
    lua_register(L, "cMobRegen_Rectangle", &Lua_cMobRegen_Rectangle);
    lua_register(L, "cMobRegen_XY", &Lua_cMobRegen_XY);
    lua_register(L, "cMobShout", &Lua_cMobShout);
    lua_register(L, "cMobSuicide", &Lua_cMobSuicide);
    lua_register(L, "cMoveStop", &Lua_cMoveStop);
    lua_register(L, "cNearObjectList", &Lua_cNearObjectList);
    lua_register(L, "cNotice", &Lua_cNotice);
    lua_register(L, "cNotice_Obj", &Lua_cNotice_Obj);
    lua_register(L, "cNoticeRedWarningCode", &Lua_cNoticeRedWarningCode);
    lua_register(L, "cNoticeString", &Lua_cNoticeString);
    lua_register(L, "cNPCChat", &Lua_cNPCChat);
    lua_register(L, "cNPCChatTest", &Lua_cNPCChatTest);
    lua_register(L, "cNPCMenuOpen", &Lua_cNPCMenuOpen);
    lua_register(L, "cNPCRegen", &Lua_cNPCRegen);
    lua_register(L, "cNPCSkillUse", &Lua_cNPCSkillUse);
    lua_register(L, "cNPCVanish", &Lua_cNPCVanish);
    lua_register(L, "cObjectCount", &Lua_cObjectCount);
    lua_register(L, "cObjectFind", &Lua_cObjectFind);
    lua_register(L, "cObjectHP", &Lua_cObjectHP);
    lua_register(L, "cObjectLocate", &Lua_cObjectLocate);
    lua_register(L, "cObjectLocateDirection", &Lua_cObjectLocateDirection);
    lua_register(L, "cObjectSound", &Lua_cObjectSound);
    lua_register(L, "cObjectType", &Lua_cObjectType);
    lua_register(L, "cPartyJoin", &Lua_cPartyJoin);
    lua_register(L, "cPartyLeave", &Lua_cPartyLeave);
    lua_register(L, "cPermileRate", &Lua_cPermileRate);
    lua_register(L, "cPlayerExist", &Lua_cPlayerExist);
    lua_register(L, "cPlaySound", &Lua_cPlaySound);
    lua_register(L, "cProgressTutorial", &Lua_cProgressTutorial);
    lua_register(L, "cQuestMobKill", &Lua_cQuestMobKill);
    lua_register(L, "cQuestMobKill_AllInMap", &Lua_cQuestMobKill_AllInMap);
    lua_register(L, "cQuestResult", &Lua_cQuestResult);
    lua_register(L, "cQuestResult_Individual", &Lua_cQuestResult_Individual);
    lua_register(L, "cRandom", &Lua_cRandom);
    lua_register(L, "cRandomInt", &Lua_cRandomInt);
    lua_register(L, "cResetAbstate", &Lua_cResetAbstate);
    lua_register(L, "cRevivalAll", &Lua_cRevivalAll);
    lua_register(L, "cReward", &Lua_cReward);
    lua_register(L, "cRewardItem", &Lua_cRewardItem);
    lua_register(L, "cRewardItem_AllInMap", &Lua_cRewardItem_AllInMap);
    lua_register(L, "cRewardItem_CharInven", &Lua_cRewardItem_CharInven);
    lua_register(L, "cRunTo", &Lua_cRunTo);
    lua_register(L, "cRunToUntilBlock", &Lua_cRunToUntilBlock);
    lua_register(L, "cScriptMessage", &Lua_cScriptMessage);
    lua_register(L, "cScriptMessage_Obj", &Lua_cScriptMessage_Obj);
    lua_register(L, "cScriptMessage_Range", &Lua_cScriptMessage_Range);
    lua_register(L, "cScriptMsg", &Lua_cScriptMsg);
    lua_register(L, "cScriptMsg_World", &Lua_cScriptMsg_World);
    lua_register(L, "cServerMenu", &Lua_cServerMenu);
    lua_register(L, "cSetAbstate", &Lua_cSetAbstate);
    lua_register(L, "cSetAbstate_Range", &Lua_cSetAbstate_Range);
    lua_register(L, "cSetAbstateInArea", &Lua_cSetAbstateInArea);
    lua_register(L, "cSetAIScript", &Lua_cSetAIScript);
    lua_register(L, "cSetCanUseReviveItem", &Lua_cSetCanUseReviveItem);
    lua_register(L, "cSetCanUseReviveSkill", &Lua_cSetCanUseReviveSkill);
    lua_register(L, "cSetDeadDelayTime", &Lua_cSetDeadDelayTime);
    lua_register(L, "cSetFieldScript", &Lua_cSetFieldScript);
    lua_register(L, "cSetFreeBattle", &Lua_cSetFreeBattle);
    lua_register(L, "cSetMobAttr", &Lua_cSetMobAttr);
    lua_register(L, "cSetNPCIsItemDrop", &Lua_cSetNPCIsItemDrop);
    lua_register(L, "cSetNPCParam", &Lua_cSetNPCParam);
    lua_register(L, "cSetNPCResist", &Lua_cSetNPCResist);
    lua_register(L, "cSetObjectDirect", &Lua_cSetObjectDirect);
    lua_register(L, "cSetReviveDelayTime", &Lua_cSetReviveDelayTime);
    lua_register(L, "cSetServantFlag", &Lua_cSetServantFlag);
    lua_register(L, "cSetTeamBattle", &Lua_cSetTeamBattle);
    lua_register(L, "cShowCinematicText", &Lua_cShowCinematicText);
    lua_register(L, "cShowKQTimerWithLife", &Lua_cShowKQTimerWithLife);
    lua_register(L, "cShowKQTimerWithLife_Obj", &Lua_cShowKQTimerWithLife_Obj);
    lua_register(L, "cSimpleChatScriptMsg", &Lua_cSimpleChatScriptMsg);
    lua_register(L, "cSkillBlast", &Lua_cSkillBlast);
    lua_register(L, "cStartMsg_AllInMap", &Lua_cStartMsg_AllInMap);
    lua_register(L, "cStaticDamage", &Lua_cStaticDamage);
    lua_register(L, "cStaticDamage_smo", &Lua_cStaticDamage_smo);
    lua_register(L, "cStaticMoverSpeed", &Lua_cStaticMoverSpeed);
    lua_register(L, "cStaticRunSpeed", &Lua_cStaticRunSpeed);
    lua_register(L, "cStaticSpeed", &Lua_cStaticSpeed);
    lua_register(L, "cStaticWalkSpeed", &Lua_cStaticWalkSpeed);
    lua_register(L, "cSystemMessage_Obj", &Lua_cSystemMessage_Obj);
    lua_register(L, "cTargetChangeNull", &Lua_cTargetChangeNull);
    lua_register(L, "cTargetHandle", &Lua_cTargetHandle);
    lua_register(L, "cTimer", &Lua_cTimer);
    lua_register(L, "cTimer_Obj", &Lua_cTimer_Obj);
    lua_register(L, "cTimerEnd", &Lua_cTimerEnd);
    lua_register(L, "cTimerStart", &Lua_cTimerStart);
    lua_register(L, "cUseMoney", &Lua_cUseMoney);
    lua_register(L, "cVanishAll", &Lua_cVanishAll);
    lua_register(L, "cVanishReserv", &Lua_cVanishReserv);
    lua_register(L, "cViewEquip", &Lua_cViewEquip);
    lua_register(L, "cViewSlotUnEquipAll", &Lua_cViewSlotUnEquipAll);
    lua_register(L, "cWalkTo", &Lua_cWalkTo);
    lua_register(L, "cWinter_Event_ScoreBoard_AllInMap", &Lua_cWinter_Event_ScoreBoard_AllInMap);
    lua_register(L, "cGetExactTime", &Lua_cGetExactTime);
    lua_register(L, "cGainMoney", &Lua_cGainMoney);
    lua_register(L, "cAddMoney", &Lua_cAddMoney);
    lua_register(L, "cGetExactClass", &Lua_cGetExactClass);
    lua_register(L, "cGetClass", &Lua_cGetClass);
    lua_register(L, "cObjectSP", &Lua_cObjectSP);
    lua_register(L, "cGainFame", &Lua_cGainFame);
    lua_register(L, "cAddFame", &Lua_cAddFame);
    lua_register(L, "cSendShout", &Lua_cSendShout);
    lua_register(L, "cShout", &Lua_cShout);
    lua_register(L, "cSendRoar", &Lua_cSendRoar);
    lua_register(L, "cRoar", &Lua_cRoar);
    lua_register(L, "cPostDiscord", &Lua_cPostDiscord);
    lua_register(L, "cKickout", &Lua_cKickout);
    lua_register(L, "cGetItemPower", &Lua_cGetItemPower);
    lua_register(L, "cHasSubAbstate", &Lua_cHasSubAbstate);
    lua_register(L, "cPartyMembers", &Lua_cPartyMembers);
    lua_register(L, "cPartyMembersZone", &Lua_cPartyMembersZone);
    lua_register(L, "cPlayerStatTotal", &Lua_cPlayerStatTotal);
    lua_register(L, "cPlayerStatSkilled", &Lua_cPlayerStatSkilled);
    lua_register(L, "cGetTarget", &Lua_cGetTarget);
    lua_register(L, "cGetExp", &Lua_cGetExp);
    lua_register(L, "cRevive", &Lua_cRevive);
    lua_register(L, "cFinishKey", &Lua_cFinishKey);
    lua_register(L, "cStartKey", &Lua_cStartKey);
    lua_register(L, "cPet_IsMasterCalling", &Lua_cPet_IsMasterCalling);
    lua_register(L, "cPet_GetMind", &Lua_cPet_GetMind);
    lua_register(L, "cPet_GetStress", &Lua_cPet_GetStress);
    lua_register(L, "cPet_GetWalkSpeed", &Lua_cPet_GetWalkSpeed);
    lua_register(L, "cPet_GetRunSpeed", &Lua_cPet_GetRunSpeed);
    lua_register(L, "cPet_Unsummon", &Lua_cPet_Unsummon);
    lua_register(L, "cPet_GetActionRecord", &Lua_cPet_GetActionRecord);
    lua_register(L, "cPet_ChangeMind", &Lua_cPet_ChangeMind);
    lua_register(L, "cPet_SaveTendency", &Lua_cPet_SaveTendency);
    lua_register(L, "cEffectTimer", &Lua_cEffectTimer);
    lua_register(L, "cFindAttackBlockLocate", &Lua_cFindAttackBlockLocate);
    lua_register(L, "cFreeStatInit", &Lua_cFreeStatInit);
    lua_register(L, "cGetMaster", &Lua_cGetMaster);
    lua_register(L, "cGetQuestHero_ItemUse", &Lua_cGetQuestHero_ItemUse);
    lua_register(L, "cGetQuestHero_NPC", &Lua_cGetQuestHero_NPC);
    lua_register(L, "cGetTutorialInfo", &Lua_cGetTutorialInfo);
    lua_register(L, "cIsNoAttacOrNoMove", &Lua_cIsNoAttacOrNoMove);
    lua_register(L, "cLocationRotate", &Lua_cLocationRotate);
    lua_register(L, "cMagicFieldSpread", &Lua_cMagicFieldSpread);
    lua_register(L, "cMapMark_FieldSight", &Lua_cMapMark_FieldSight);
    lua_register(L, "cMapObjectControl", &Lua_cMapObjectControl);
    lua_register(L, "cMobDialog_String", &Lua_cMobDialog_String);
    lua_register(L, "cMobRegen_XY_Layer", &Lua_cMobRegen_XY_Layer);
    lua_register(L, "cMobWeaponRate", &Lua_cMobWeaponRate);
    lua_register(L, "cMove2Where", &Lua_cMove2Where);
    lua_register(L, "cMoveLayer", &Lua_cMoveLayer);
    lua_register(L, "cObjectEffect", &Lua_cObjectEffect);
    lua_register(L, "cRegenGroupActiv", &Lua_cRegenGroupActiv);
    lua_register(L, "cSaveTutorialInfo", &Lua_cSaveTutorialInfo);
    lua_register(L, "cScoreBoard", &Lua_cScoreBoard);
    lua_register(L, "cScoreBoard_AllInMap", &Lua_cScoreBoard_AllInMap);
    lua_register(L, "cScoreInfo", &Lua_cScoreInfo);
    lua_register(L, "cScoreInfo_AllInMap", &Lua_cScoreInfo_AllInMap);
    lua_register(L, "cScoreResultList", &Lua_cScoreResultList);
    lua_register(L, "cScoreTopList", &Lua_cScoreTopList);
    lua_register(L, "cSendGameLogDataType_4", &Lua_cSendGameLogDataType_4);
    lua_register(L, "cSendGameLogDataType_5", &Lua_cSendGameLogDataType_5);
    lua_register(L, "cSendRankingList", &Lua_cSendRankingList);
    lua_register(L, "cSetItemDropMobID", &Lua_cSetItemDropMobID);
    lua_register(L, "cSetSightState", &Lua_cSetSightState);
    lua_register(L, "cSkillDamageCalculate", &Lua_cSkillDamageCalculate);
    lua_register(L, "cSomebodyShout", &Lua_cSomebodyShout);
    lua_register(L, "cTopView", &Lua_cTopView);
    lua_register(L, "cTriggerRegist", &Lua_cTriggerRegist);
    lua_register(L, "cVanishTimer", &Lua_cVanishTimer);
    lua_register(L, "cWaitIdle", &Lua_cWaitIdle);
    lua_register(L, "cWillMovement", &Lua_cWillMovement);
}
} // namespace fiesta