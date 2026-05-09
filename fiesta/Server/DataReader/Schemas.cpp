// Server/DataReader/Schemas.cpp  -- GENERATED row decoders
#include "Schemas.h"
#include "../Shared/ShineLogSystem.h"
#include <stdlib.h>
namespace fiesta {
static uint32 ColU32(const std::vector<std::string>& r, size_t i){return (i<r.size()&&!r[i].empty())?(uint32)strtoul(r[i].c_str(),NULL,10):0u;}
static int32  ColI32(const std::vector<std::string>& r, size_t i){return (i<r.size()&&!r[i].empty())?(int32)atoi(r[i].c_str()):0;}
static float  ColF32(const std::vector<std::string>& r, size_t i){return (i<r.size()&&!r[i].empty())?(float)atof(r[i].c_str()):0.0f;}
static std::string ColStr(const std::vector<std::string>& r, size_t i){return (i<r.size())?r[i]:std::string();}
#include <string.h>
ItemInfoTab g_ItemInfoTab;
bool ItemInfoTab::Load(DataReader& reader) {
    std::vector<std::vector<std::string> > kRows;
    if (!reader.LoadAsRows("ItemInfo", kRows)) return false;
    for (size_t i = 1; i < kRows.size(); ++i) {
        const std::vector<std::string>& r = kRows[i];
        ItemInfoRow rec; memset(&rec, 0, sizeof(rec));
        rec.ID = (uint32)ColU32(r, 0);
        { std::string s = ColStr(r, 1); size_t n = s.size(); if (n > sizeof(rec.InxName)-1) n = sizeof(rec.InxName)-1; memcpy(rec.InxName, s.data(), n); rec.InxName[n] = 0; }
        { std::string s = ColStr(r, 2); size_t n = s.size(); if (n > sizeof(rec.Name)-1) n = sizeof(rec.Name)-1; memcpy(rec.Name, s.data(), n); rec.Name[n] = 0; }
        rec.Type = (uint32)ColU32(r, 3);
        rec.Class = (uint32)ColU32(r, 4);
        rec.MaxLot = (uint32)ColU32(r, 5);
        rec.Equip = (uint32)ColU32(r, 6);
        rec.ItemAuctionSubGroup = (uint32)ColU32(r, 7);
        rec.TwoHand = (uint8)ColU32(r, 8);
        rec.AtkSpeed = (uint32)ColU32(r, 9);
        rec.DemandLv = (uint32)ColU32(r, 10);
        rec.Grade = (uint32)ColU32(r, 11);
        rec.MinWC = (uint32)ColU32(r, 12);
        rec.MaxWC = (uint32)ColU32(r, 13);
        rec.AC = (uint32)ColU32(r, 14);
        rec.MinMA = (uint32)ColU32(r, 15);
        rec.MaxMA = (uint32)ColU32(r, 16);
        rec.MR = (uint32)ColU32(r, 17);
        rec.WCRate = (uint32)ColU32(r, 18);
        rec.MARate = (uint32)ColU32(r, 19);
        rec.ACRate = (uint32)ColU32(r, 20);
        rec.MRRate = (uint32)ColU32(r, 21);
        rec.CriRate = (uint32)ColU32(r, 22);
        rec.CriMinWc = (uint32)ColU32(r, 23);
        rec.CriMaxWc = (uint32)ColU32(r, 24);
        rec.CriMinMa = (uint32)ColU32(r, 25);
        rec.CriMaxMa = (uint32)ColU32(r, 26);
        rec.CrlTB = (uint32)ColU32(r, 27);
        rec.MaxHP = (uint32)ColU32(r, 28);
        rec.MaxSP = (uint32)ColU32(r, 29);
        rec.MaxAP = (uint32)ColU32(r, 30);
        rec.dummy = (uint32)ColU32(r, 31);
        rec.Fig = (uint32)ColU32(r, 32);
        rec.Cfig = (uint32)ColU32(r, 33);
        rec.War = (uint32)ColU32(r, 34);
        rec.Gla = (uint32)ColU32(r, 35);
        rec.Kni = (uint32)ColU32(r, 36);
        rec.Cle = (uint32)ColU32(r, 37);
        rec.Hcle = (uint32)ColU32(r, 38);
        rec.Pal = (uint32)ColU32(r, 39);
        rec.Hol = (uint32)ColU32(r, 40);
        rec.Gua = (uint32)ColU32(r, 41);
        rec.Arc = (uint32)ColU32(r, 42);
        rec.Harc = (uint32)ColU32(r, 43);
        rec.Sco = (uint32)ColU32(r, 44);
        rec.Sha = (uint32)ColU32(r, 45);
        rec.Ran = (uint32)ColU32(r, 46);
        rec.Mag = (uint32)ColU32(r, 47);
        rec.Wmag = (uint32)ColU32(r, 48);
        rec.Enc = (uint32)ColU32(r, 49);
        rec.Warl = (uint32)ColU32(r, 50);
        rec.Wiz = (uint32)ColU32(r, 51);
        rec.Jok = (uint32)ColU32(r, 52);
        rec.Chs = (uint32)ColU32(r, 53);
        rec.Cru = (uint32)ColU32(r, 54);
        rec.Cls = (uint32)ColU32(r, 55);
        rec.Ass = (uint32)ColU32(r, 56);
        rec.BuyPrice = (uint32)ColU32(r, 57);
        rec.SellPrice = (uint32)ColU32(r, 58);
        rec.BuyFame = (uint32)ColU32(r, 59);
        rec.BuyGToken = (uint32)ColU32(r, 60);
        rec.BuyGBCoin = (uint32)ColU32(r, 61);
        rec.WeaponType = (uint32)ColU32(r, 62);
        rec.ArmorType = (uint32)ColU32(r, 63);
        rec.UpLimit = (uint8)ColU32(r, 64);
        rec.UpSucRatio = (uint16)ColU32(r, 65);
        rec.UpLuckRatio = (uint16)ColU32(r, 66);
        rec.UpResource = (uint8)ColU32(r, 67);
        rec.BasicUpInx = (uint16)ColU32(r, 68);
        rec.AddUpInx = (uint16)ColU32(r, 69);
        rec.TH = (uint32)ColU32(r, 70);
        rec.TB = (uint32)ColU32(r, 71);
        rec.ShieldAC = (uint32)ColU32(r, 72);
        rec.HitRatePlus = (uint32)ColU32(r, 73);
        rec.EvaRatePlus = (uint32)ColU32(r, 74);
        rec.MACriPlus = (uint32)ColU32(r, 75);
        rec.CriDamPlus = (uint32)ColU32(r, 76);
        rec.MagCriDamPlus = (uint32)ColU32(r, 77);
        rec.PutOnBelonged = (uint8)ColU32(r, 78);
        rec.Belonged = (uint8)ColU32(r, 79);
        rec.NoDrop = (uint8)ColU32(r, 80);
        rec.NoSell = (uint8)ColU32(r, 81);
        rec.NoStorage = (uint8)ColU32(r, 82);
        rec.NoTrade = (uint8)ColU32(r, 83);
        rec.NoDelete = (uint8)ColU32(r, 84);
        { std::string s = ColStr(r, 85); size_t n = s.size(); if (n > sizeof(rec.TitleName)-1) n = sizeof(rec.TitleName)-1; memcpy(rec.TitleName, s.data(), n); rec.TitleName[n] = 0; }
        rec.ItemGradeType = (uint32)ColU32(r, 86);
        { std::string s = ColStr(r, 87); size_t n = s.size(); if (n > sizeof(rec.ItemUseSkill)-1) n = sizeof(rec.ItemUseSkill)-1; memcpy(rec.ItemUseSkill, s.data(), n); rec.ItemUseSkill[n] = 0; }
        { std::string s = ColStr(r, 88); size_t n = s.size(); if (n > sizeof(rec.SetItemIndex)-1) n = sizeof(rec.SetItemIndex)-1; memcpy(rec.SetItemIndex, s.data(), n); rec.SetItemIndex[n] = 0; }
        rec.ItemFunc = (uint32)ColU32(r, 89);
        rec.AutoMon = (uint8)ColU32(r, 90);
        Push((uint32)i, rec);
    }
    SHINELOG_INFO("ItemInfo: loaded %u rows", (uint32)m_uiTotal);
    return true;
}
AbStateTab g_AbStateTab;
bool AbStateTab::Load(DataReader& reader) {
    std::vector<std::vector<std::string> > kRows;
    if (!reader.LoadAsRows("AbState", kRows)) return false;
    for (size_t i = 1; i < kRows.size(); ++i) {
        const std::vector<std::string>& r = kRows[i];
        AbStateRow rec; memset(&rec, 0, sizeof(rec));
        rec.ID = (uint32)ColU32(r, 0);
        { std::string s = ColStr(r, 1); size_t n = s.size(); if (n > sizeof(rec.InxName)-1) n = sizeof(rec.InxName)-1; memcpy(rec.InxName, s.data(), n); rec.InxName[n] = 0; }
        rec.AbStataIndex = (uint32)ColU32(r, 2);
        rec.KeepTimePower = (uint8)ColU32(r, 3);
        rec.StateGrade = (uint8)ColU32(r, 4);
        { std::string s = ColStr(r, 5); size_t n = s.size(); if (n > sizeof(rec.PartyState1)-1) n = sizeof(rec.PartyState1)-1; memcpy(rec.PartyState1, s.data(), n); rec.PartyState1[n] = 0; }
        { std::string s = ColStr(r, 6); size_t n = s.size(); if (n > sizeof(rec.PartyState2)-1) n = sizeof(rec.PartyState2)-1; memcpy(rec.PartyState2, s.data(), n); rec.PartyState2[n] = 0; }
        { std::string s = ColStr(r, 7); size_t n = s.size(); if (n > sizeof(rec.PartyState3)-1) n = sizeof(rec.PartyState3)-1; memcpy(rec.PartyState3, s.data(), n); rec.PartyState3[n] = 0; }
        { std::string s = ColStr(r, 8); size_t n = s.size(); if (n > sizeof(rec.PartyState4)-1) n = sizeof(rec.PartyState4)-1; memcpy(rec.PartyState4, s.data(), n); rec.PartyState4[n] = 0; }
        { std::string s = ColStr(r, 9); size_t n = s.size(); if (n > sizeof(rec.PartyState5)-1) n = sizeof(rec.PartyState5)-1; memcpy(rec.PartyState5, s.data(), n); rec.PartyState5[n] = 0; }
        rec.PartyRange = (uint32)ColU32(r, 10);
        rec.PartyEnchantNumber = (uint32)ColU32(r, 11);
        { std::string s = ColStr(r, 12); size_t n = s.size(); if (n > sizeof(rec.SubAbState)-1) n = sizeof(rec.SubAbState)-1; memcpy(rec.SubAbState, s.data(), n); rec.SubAbState[n] = 0; }
        rec.DispelIndex = (uint32)ColU32(r, 13);
        rec.SubDispelIndex = (uint32)ColU32(r, 14);
        rec.IsSave = (uint8)ColU32(r, 15);
        { std::string s = ColStr(r, 16); size_t n = s.size(); if (n > sizeof(rec.MainStateInx)-1) n = sizeof(rec.MainStateInx)-1; memcpy(rec.MainStateInx, s.data(), n); rec.MainStateInx[n] = 0; }
        rec.Duplicate = (uint8)ColU32(r, 17);
        Push((uint32)i, rec);
    }
    SHINELOG_INFO("AbState: loaded %u rows", (uint32)m_uiTotal);
    return true;
}
SubAbStateTab g_SubAbStateTab;
bool SubAbStateTab::Load(DataReader& reader) {
    std::vector<std::vector<std::string> > kRows;
    if (!reader.LoadAsRows("SubAbState", kRows)) return false;
    for (size_t i = 1; i < kRows.size(); ++i) {
        const std::vector<std::string>& r = kRows[i];
        SubAbStateRow rec; memset(&rec, 0, sizeof(rec));
        rec.ID = (uint32)ColU32(r, 0);
        { std::string s = ColStr(r, 1); size_t n = s.size(); if (n > sizeof(rec.InxName)-1) n = sizeof(rec.InxName)-1; memcpy(rec.InxName, s.data(), n); rec.InxName[n] = 0; }
        rec.Strength = (uint32)ColU32(r, 2);
        rec.Type = (uint32)ColU32(r, 3);
        rec.SubType = (uint8)ColU32(r, 4);
        rec.KeepTime = (uint32)ColU32(r, 5);
        rec.ActionIndexA = (uint32)ColU32(r, 6);
        rec.ActionArgA = (uint32)ColU32(r, 7);
        rec.ActionIndexB = (uint32)ColU32(r, 8);
        rec.ActionArgB = (uint32)ColU32(r, 9);
        rec.ActionIndexC = (uint32)ColU32(r, 10);
        rec.ActionArgC = (uint32)ColU32(r, 11);
        rec.ActionIndexD = (uint32)ColU32(r, 12);
        rec.ActionArgD = (uint32)ColU32(r, 13);
        Push((uint32)i, rec);
    }
    SHINELOG_INFO("SubAbState: loaded %u rows", (uint32)m_uiTotal);
    return true;
}
ActiveSkillTab g_ActiveSkillTab;
bool ActiveSkillTab::Load(DataReader& reader) {
    std::vector<std::vector<std::string> > kRows;
    if (!reader.LoadAsRows("ActiveSkill", kRows)) return false;
    for (size_t i = 1; i < kRows.size(); ++i) {
        const std::vector<std::string>& r = kRows[i];
        ActiveSkillRow rec; memset(&rec, 0, sizeof(rec));
        rec.ID = (uint32)ColU32(r, 0);
        { std::string s = ColStr(r, 1); size_t n = s.size(); if (n > sizeof(rec.InxName)-1) n = sizeof(rec.InxName)-1; memcpy(rec.InxName, s.data(), n); rec.InxName[n] = 0; }
        { std::string s = ColStr(r, 2); size_t n = s.size(); if (n > sizeof(rec.Name)-1) n = sizeof(rec.Name)-1; memcpy(rec.Name, s.data(), n); rec.Name[n] = 0; }
        rec.Grade = (uint32)ColU32(r, 3);
        rec.Step = (uint32)ColU32(r, 4);
        rec.MaxStep = (uint32)ColU32(r, 5);
        rec.DemandType = (uint32)ColU32(r, 6);
        { std::string s = ColStr(r, 7); size_t n = s.size(); if (n > sizeof(rec.DemandSk)-1) n = sizeof(rec.DemandSk)-1; memcpy(rec.DemandSk, s.data(), n); rec.DemandSk[n] = 0; }
        rec.UseItem = (uint32)ColU32(r, 8);
        rec.ItemNumber = (uint32)ColU32(r, 9);
        rec.ItemOption = (uint32)ColU32(r, 10);
        rec.DemandItem1 = (uint32)ColU32(r, 11);
        rec.DemandItem2 = (uint32)ColU32(r, 12);
        rec.SP = (uint32)ColU32(r, 13);
        rec.HP = (uint32)ColU32(r, 14);
        rec.Range = (uint32)ColU32(r, 15);
        rec.First = (uint32)ColU32(r, 16);
        rec.Last = (uint32)ColU32(r, 17);
        rec.IsMovingSkill = (uint8)ColU32(r, 18);
        rec.UsableDegree = (uint16)ColU32(r, 19);
        rec.DirectionRotate = (uint16)ColU32(r, 20);
        rec.SkillDegree = (uint16)ColU32(r, 21);
        rec.SkillTargetState = (uint32)ColU32(r, 22);
        rec.CastTime = (uint16)ColU32(r, 23);
        rec.DlyTime = (uint32)ColU32(r, 24);
        rec.DlyGroupNum = (uint32)ColU32(r, 25);
        rec.DlyTimeGroup = (uint32)ColU32(r, 26);
        rec.MinWC = (uint32)ColU32(r, 27);
        rec.MaxWC = (uint32)ColU32(r, 28);
        rec.MinMA = (uint32)ColU32(r, 29);
        rec.MaxMA = (uint32)ColU32(r, 30);
        rec.AC = (uint32)ColU32(r, 31);
        rec.MR = (uint32)ColU32(r, 32);
        rec.Area = (uint32)ColU32(r, 33);
        rec.TargetNumber = (uint32)ColU32(r, 34);
        rec.dummy = (uint32)ColU32(r, 35);
        rec.Fig = (uint32)ColU32(r, 36);
        rec.Cfig = (uint32)ColU32(r, 37);
        rec.War = (uint32)ColU32(r, 38);
        rec.Gla = (uint32)ColU32(r, 39);
        rec.Kni = (uint32)ColU32(r, 40);
        rec.Cle = (uint32)ColU32(r, 41);
        rec.Hcle = (uint32)ColU32(r, 42);
        rec.Pal = (uint32)ColU32(r, 43);
        rec.Hol = (uint32)ColU32(r, 44);
        rec.Gua = (uint32)ColU32(r, 45);
        rec.Arc = (uint32)ColU32(r, 46);
        rec.Harc = (uint32)ColU32(r, 47);
        rec.Sco = (uint32)ColU32(r, 48);
        rec.Sha = (uint32)ColU32(r, 49);
        rec.Ran = (uint32)ColU32(r, 50);
        rec.Mag = (uint32)ColU32(r, 51);
        rec.Wmag = (uint32)ColU32(r, 52);
        rec.Enc = (uint32)ColU32(r, 53);
        rec.Warl = (uint32)ColU32(r, 54);
        rec.Wiz = (uint32)ColU32(r, 55);
        rec.Jok = (uint32)ColU32(r, 56);
        rec.Chs = (uint32)ColU32(r, 57);
        rec.Cru = (uint32)ColU32(r, 58);
        rec.Cls = (uint32)ColU32(r, 59);
        rec.Ass = (uint32)ColU32(r, 60);
        { std::string s = ColStr(r, 61); size_t n = s.size(); if (n > sizeof(rec.StaNameA)-1) n = sizeof(rec.StaNameA)-1; memcpy(rec.StaNameA, s.data(), n); rec.StaNameA[n] = 0; }
        rec.StaStrengthA = (uint32)ColU32(r, 62);
        rec.StaSucRateA = (uint32)ColU32(r, 63);
        { std::string s = ColStr(r, 64); size_t n = s.size(); if (n > sizeof(rec.StaNameB)-1) n = sizeof(rec.StaNameB)-1; memcpy(rec.StaNameB, s.data(), n); rec.StaNameB[n] = 0; }
        rec.StaStrengthB = (uint32)ColU32(r, 65);
        rec.StaSucRateB = (uint32)ColU32(r, 66);
        { std::string s = ColStr(r, 67); size_t n = s.size(); if (n > sizeof(rec.StaNameC)-1) n = sizeof(rec.StaNameC)-1; memcpy(rec.StaNameC, s.data(), n); rec.StaNameC[n] = 0; }
        rec.StaStrengthC = (uint32)ColU32(r, 68);
        rec.StaSucRateC = (uint32)ColU32(r, 69);
        { std::string s = ColStr(r, 70); size_t n = s.size(); if (n > sizeof(rec.StaNameD)-1) n = sizeof(rec.StaNameD)-1; memcpy(rec.StaNameD, s.data(), n); rec.StaNameD[n] = 0; }
        rec.StaStrengthD = (uint32)ColU32(r, 71);
        rec.StaSucRateD = (uint32)ColU32(r, 72);
        rec.IMPT0 = (uint32)ColU32(r, 73);
        rec.IMPT1 = (uint32)ColU32(r, 74);
        rec.IMPT2 = (uint32)ColU32(r, 75);
        rec.IMPT3 = (uint32)ColU32(r, 76);
        rec.T00 = (uint32)ColU32(r, 77);
        rec.T01 = (uint32)ColU32(r, 78);
        rec.T02 = (uint32)ColU32(r, 79);
        rec.T03 = (uint32)ColU32(r, 80);
        rec.T04 = (uint32)ColU32(r, 81);
        rec.T10 = (uint32)ColU32(r, 82);
        rec.T11 = (uint32)ColU32(r, 83);
        rec.T12 = (uint32)ColU32(r, 84);
        rec.T13 = (uint32)ColU32(r, 85);
        rec.T14 = (uint32)ColU32(r, 86);
        rec.T20 = (uint32)ColU32(r, 87);
        rec.T21 = (uint32)ColU32(r, 88);
        rec.T22 = (uint32)ColU32(r, 89);
        rec.T23 = (uint32)ColU32(r, 90);
        rec.T24 = (uint32)ColU32(r, 91);
        rec.T30 = (uint32)ColU32(r, 92);
        rec.T31 = (uint32)ColU32(r, 93);
        rec.T32 = (uint32)ColU32(r, 94);
        rec.T33 = (uint32)ColU32(r, 95);
        rec.T34 = (uint32)ColU32(r, 96);
        rec.EffectType = (uint32)ColU32(r, 97);
        rec.SpecialIndexA = (uint32)ColU32(r, 98);
        rec.SpecialValueA = (uint32)ColU32(r, 99);
        rec.SpecialIndexB = (uint32)ColU32(r, 100);
        rec.SpecialValueB = (uint32)ColU32(r, 101);
        rec.SpecialIndexC = (uint32)ColU32(r, 102);
        rec.SpecialValueC = (uint32)ColU32(r, 103);
        rec.SpecialIndexD = (uint32)ColU32(r, 104);
        rec.SpecialValueD = (uint32)ColU32(r, 105);
        rec.SpecialIndexE = (uint32)ColU32(r, 106);
        rec.SpecialValueE = (uint32)ColU32(r, 107);
        { std::string s = ColStr(r, 108); size_t n = s.size(); if (n > sizeof(rec.SkillClassifierA)-1) n = sizeof(rec.SkillClassifierA)-1; memcpy(rec.SkillClassifierA, s.data(), n); rec.SkillClassifierA[n] = 0; }
        { std::string s = ColStr(r, 109); size_t n = s.size(); if (n > sizeof(rec.SkillClassifierB)-1) n = sizeof(rec.SkillClassifierB)-1; memcpy(rec.SkillClassifierB, s.data(), n); rec.SkillClassifierB[n] = 0; }
        { std::string s = ColStr(r, 110); size_t n = s.size(); if (n > sizeof(rec.SkillClassifierC)-1) n = sizeof(rec.SkillClassifierC)-1; memcpy(rec.SkillClassifierC, s.data(), n); rec.SkillClassifierC[n] = 0; }
        rec.CannotInside = (uint8)ColU32(r, 111);
        rec.DemandSoul = (uint8)ColU32(r, 112);
        rec.HitID = (uint16)ColU32(r, 113);
        Push((uint32)i, rec);
    }
    SHINELOG_INFO("ActiveSkill: loaded %u rows", (uint32)m_uiTotal);
    return true;
}
MobInfoTab g_MobInfoTab;
bool MobInfoTab::Load(DataReader& reader) {
    std::vector<std::vector<std::string> > kRows;
    if (!reader.LoadAsRows("MobInfo", kRows)) return false;
    for (size_t i = 1; i < kRows.size(); ++i) {
        const std::vector<std::string>& r = kRows[i];
        MobInfoRow rec; memset(&rec, 0, sizeof(rec));
        rec.Field_Name = (uint32)ColU32(r, 0);
        { std::string s = ColStr(r, 1); size_t n = s.size(); if (n > sizeof(rec.InxName)-1) n = sizeof(rec.InxName)-1; memcpy(rec.InxName, s.data(), n); rec.InxName[n] = 0; }
        { std::string s = ColStr(r, 2); size_t n = s.size(); if (n > sizeof(rec.Name)-1) n = sizeof(rec.Name)-1; memcpy(rec.Name, s.data(), n); rec.Name[n] = 0; }
        rec.Level = (uint32)ColU32(r, 3);
        rec.MaxHP = (uint32)ColU32(r, 4);
        rec.WalkSpeed = (uint32)ColU32(r, 5);
        rec.RunSpeed = (uint32)ColU32(r, 6);
        rec.IsNPC = (uint8)ColU32(r, 7);
        rec.Size = (uint32)ColU32(r, 8);
        rec.WeaponType = (uint32)ColU32(r, 9);
        rec.ArmorType = (uint32)ColU32(r, 10);
        rec.GradeType = (uint32)ColU32(r, 11);
        rec.Type = (uint32)ColU32(r, 12);
        rec.IsPlayerSide = (uint8)ColU32(r, 13);
        rec.AbsoluteSize = (uint32)ColU32(r, 14);
        Push((uint32)i, rec);
    }
    SHINELOG_INFO("MobInfo: loaded %u rows", (uint32)m_uiTotal);
    return true;
}
ItemActionEffectTab g_ItemActionEffectTab;
bool ItemActionEffectTab::Load(DataReader& reader) {
    std::vector<std::vector<std::string> > kRows;
    if (!reader.LoadAsRows("ItemActionEffect", kRows)) return false;
    for (size_t i = 1; i < kRows.size(); ++i) {
        const std::vector<std::string>& r = kRows[i];
        ItemActionEffectRow rec; memset(&rec, 0, sizeof(rec));
        rec.EffectID = (uint16)ColU32(r, 0);
        rec.EffectTarget = (uint32)ColU32(r, 1);
        rec.EffectActivity = (uint32)ColU32(r, 2);
        rec.Value = (uint16)ColU32(r, 3);
        rec.Area = (uint16)ColU32(r, 4);
        Push((uint32)i, rec);
    }
    SHINELOG_INFO("ItemActionEffect: loaded %u rows", (uint32)m_uiTotal);
    return true;
}
ItemActionConditionTab g_ItemActionConditionTab;
bool ItemActionConditionTab::Load(DataReader& reader) {
    std::vector<std::vector<std::string> > kRows;
    if (!reader.LoadAsRows("ItemActionCondition", kRows)) return false;
    for (size_t i = 1; i < kRows.size(); ++i) {
        const std::vector<std::string>& r = kRows[i];
        ItemActionConditionRow rec; memset(&rec, 0, sizeof(rec));
        rec.ConditionID = (uint16)ColU32(r, 0);
        rec.SubjectTarget = (uint32)ColU32(r, 1);
        rec.ObjectTarget = (uint32)ColU32(r, 2);
        rec.ConditionActivity = (uint32)ColU32(r, 3);
        rec.ActivityRate = (uint16)ColU32(r, 4);
        rec.Range = (uint16)ColU32(r, 5);
        Push((uint32)i, rec);
    }
    SHINELOG_INFO("ItemActionCondition: loaded %u rows", (uint32)m_uiTotal);
    return true;
}
ItemInfoServerTab g_ItemInfoServerTab;
bool ItemInfoServerTab::Load(DataReader& reader) {
    std::vector<std::vector<std::string> > kRows;
    if (!reader.LoadAsRows("ItemInfoServer", kRows)) return false;
    for (size_t i = 1; i < kRows.size(); ++i) {
        const std::vector<std::string>& r = kRows[i];
        ItemInfoServerRow rec; memset(&rec, 0, sizeof(rec));
        rec.ID = (uint32)ColU32(r, 0);
        { std::string s = ColStr(r, 1); size_t n = s.size(); if (n > sizeof(rec.InxName)-1) n = sizeof(rec.InxName)-1; memcpy(rec.InxName, s.data(), n); rec.InxName[n] = 0; }
        { std::string s = ColStr(r, 2); size_t n = s.size(); if (n > sizeof(rec.MarketIndex)-1) n = sizeof(rec.MarketIndex)-1; memcpy(rec.MarketIndex, s.data(), n); rec.MarketIndex[n] = 0; }
        rec.Rou = (uint32)ColU32(r, 3);
        rec.Eld = (uint32)ColU32(r, 4);
        rec.Urg = (uint32)ColU32(r, 5);
        rec.All = (uint32)ColU32(r, 6);
        rec.Fer = (uint32)ColU32(r, 7);
        rec.Kas = (uint32)ColU32(r, 8);
        rec.Sad = (uint32)ColU32(r, 9);
        rec.Kor = (uint32)ColU32(r, 10);
        rec.Ver = (uint32)ColU32(r, 11);
        rec.Mys = (uint32)ColU32(r, 12);
        { std::string s = ColStr(r, 13); size_t n = s.size(); if (n > sizeof(rec.DropGroupA)-1) n = sizeof(rec.DropGroupA)-1; memcpy(rec.DropGroupA, s.data(), n); rec.DropGroupA[n] = 0; }
        { std::string s = ColStr(r, 14); size_t n = s.size(); if (n > sizeof(rec.DropGroupB)-1) n = sizeof(rec.DropGroupB)-1; memcpy(rec.DropGroupB, s.data(), n); rec.DropGroupB[n] = 0; }
        { std::string s = ColStr(r, 15); size_t n = s.size(); if (n > sizeof(rec.RandomOptionDropGroup)-1) n = sizeof(rec.RandomOptionDropGroup)-1; memcpy(rec.RandomOptionDropGroup, s.data(), n); rec.RandomOptionDropGroup[n] = 0; }
        rec.Vanish = (uint32)ColU32(r, 16);
        rec.looting = (uint32)ColU32(r, 17);
        rec.DropRateKilledByMob = (uint16)ColU32(r, 18);
        rec.DropRateKilledByPlayer = (uint16)ColU32(r, 19);
        rec.ISET_Index = (uint32)ColU32(r, 20);
        rec.KQItem = (uint8)ColU32(r, 21);
        rec.PK_KQ_USE = (uint8)ColU32(r, 22);
        rec.KQ_Item_Drop = (uint8)ColU32(r, 23);
        rec.PreventAttack = (uint8)ColU32(r, 24);
        Push((uint32)i, rec);
    }
    SHINELOG_INFO("ItemInfoServer: loaded %u rows", (uint32)m_uiTotal);
    return true;
}
CharacterTitleDataTab g_CharacterTitleDataTab;
bool CharacterTitleDataTab::Load(DataReader& reader) {
    std::vector<std::vector<std::string> > kRows;
    if (!reader.LoadAsRows("CharacterTitleData", kRows)) return false;
    for (size_t i = 1; i < kRows.size(); ++i) {
        const std::vector<std::string>& r = kRows[i];
        CharacterTitleDataRow rec; memset(&rec, 0, sizeof(rec));
        rec.Type = (uint32)ColU32(r, 0);
        rec.REMARK = (uint32)ColU32(r, 1);
        { std::string s = ColStr(r, 2); size_t n = s.size(); if (n > sizeof(rec.Title0)-1) n = sizeof(rec.Title0)-1; memcpy(rec.Title0, s.data(), n); rec.Title0[n] = 0; }
        rec.Permit = (uint32)ColU32(r, 3);
        rec.Refresh = (uint32)ColU32(r, 4);
        rec.Value0 = (uint32)ColU32(r, 5);
        rec.Fame0 = (uint32)ColU32(r, 6);
        { std::string s = ColStr(r, 7); size_t n = s.size(); if (n > sizeof(rec.Title1)-1) n = sizeof(rec.Title1)-1; memcpy(rec.Title1, s.data(), n); rec.Title1[n] = 0; }
        rec.Value1 = (uint32)ColU32(r, 8);
        rec.Fame1 = (uint32)ColU32(r, 9);
        { std::string s = ColStr(r, 10); size_t n = s.size(); if (n > sizeof(rec.Title2)-1) n = sizeof(rec.Title2)-1; memcpy(rec.Title2, s.data(), n); rec.Title2[n] = 0; }
        rec.Value2 = (uint32)ColU32(r, 11);
        rec.Fame2 = (uint32)ColU32(r, 12);
        { std::string s = ColStr(r, 13); size_t n = s.size(); if (n > sizeof(rec.Title3)-1) n = sizeof(rec.Title3)-1; memcpy(rec.Title3, s.data(), n); rec.Title3[n] = 0; }
        rec.Value3 = (uint32)ColU32(r, 14);
        rec.Fame3 = (uint32)ColU32(r, 15);
        Push((uint32)i, rec);
    }
    SHINELOG_INFO("CharacterTitleData: loaded %u rows", (uint32)m_uiTotal);
    return true;
}
CollectCardTab g_CollectCardTab;
bool CollectCardTab::Load(DataReader& reader) {
    std::vector<std::vector<std::string> > kRows;
    if (!reader.LoadAsRows("CollectCard", kRows)) return false;
    for (size_t i = 1; i < kRows.size(); ++i) {
        const std::vector<std::string>& r = kRows[i];
        CollectCardRow rec; memset(&rec, 0, sizeof(rec));
        rec.CC_CardID = (uint16)ColU32(r, 0);
        { std::string s = ColStr(r, 1); size_t n = s.size(); if (n > sizeof(rec.CC_ItemInx)-1) n = sizeof(rec.CC_ItemInx)-1; memcpy(rec.CC_ItemInx, s.data(), n); rec.CC_ItemInx[n] = 0; }
        rec.CC_CardGradeType = (uint32)ColU32(r, 2);
        rec.CC_CardMobGroup = (uint32)ColU32(r, 3);
        Push((uint32)i, rec);
    }
    SHINELOG_INFO("CollectCard: loaded %u rows", (uint32)m_uiTotal);
    return true;
}
ActionRangeFactorTab g_ActionRangeFactorTab;
bool ActionRangeFactorTab::Load(DataReader& reader) {
    std::vector<std::vector<std::string> > kRows;
    if (!reader.LoadAsRows("ActionRangeFactor", kRows)) return false;
    for (size_t i = 1; i < kRows.size(); ++i) {
        const std::vector<std::string>& r = kRows[i];
        ActionRangeFactorRow rec; memset(&rec, 0, sizeof(rec));
        rec.ActionRangeIndex = (uint32)ColU32(r, 0);
        rec.RangeType = (uint32)ColU32(r, 1);
        rec.RangeStart = (uint16)ColU32(r, 2);
        rec.RangeEnd = (uint16)ColU32(r, 3);
        Push((uint32)i, rec);
    }
    SHINELOG_INFO("ActionRangeFactor: loaded %u rows", (uint32)m_uiTotal);
    return true;
}
GTIServerTab g_GTIServerTab;
bool GTIServerTab::Load(DataReader& reader) {
    std::vector<std::vector<std::string> > kRows;
    if (!reader.LoadAsRows("GTIServer", kRows)) return false;
    for (size_t i = 1; i < kRows.size(); ++i) {
        const std::vector<std::string>& r = kRows[i];
        GTIServerRow rec; memset(&rec, 0, sizeof(rec));
        rec.ID = (uint8)ColU32(r, 0);
        { std::string s = ColStr(r, 1); size_t n = s.size(); if (n > sizeof(rec.InxName)-1) n = sizeof(rec.InxName)-1; memcpy(rec.InxName, s.data(), n); rec.InxName[n] = 0; }
        rec.SubjectTarget = (uint32)ColU32(r, 2);
        rec.EnemyNumber = (uint8)ColU32(r, 3);
        rec.GTIActionType = (uint32)ColU32(r, 4);
        { std::string s = ColStr(r, 5); size_t n = s.size(); if (n > sizeof(rec.Index)-1) n = sizeof(rec.Index)-1; memcpy(rec.Index, s.data(), n); rec.Index[n] = 0; }
        rec.Value = (uint8)ColU32(r, 6);
        Push((uint32)i, rec);
    }
    SHINELOG_INFO("GTIServer: loaded %u rows", (uint32)m_uiTotal);
    return true;
}
GuildTournamentTab g_GuildTournamentTab;
bool GuildTournamentTab::Load(DataReader& reader) {
    std::vector<std::vector<std::string> > kRows;
    if (!reader.LoadAsRows("GuildTournament", kRows)) return false;
    for (size_t i = 1; i < kRows.size(); ++i) {
        const std::vector<std::string>& r = kRows[i];
        GuildTournamentRow rec; memset(&rec, 0, sizeof(rec));
        rec.GuildTournament = (uint32)ColU32(r, 0);
        rec.GuildTournamentReward = (uint32)ColU32(r, 1);
        rec.GuildTournamentRequire = (uint32)ColU32(r, 2);
        rec.GuildTournamentSkill = (uint32)ColU32(r, 3);
        rec.GuildTournamentSkillDesc = (uint32)ColU32(r, 4);
        rec.GuildTournamentLvGap = (uint32)ColU32(r, 5);
        rec.GuildTournamentOccupy = (uint32)ColU32(r, 6);
        rec.GuildTournamentMasterBuff = (uint32)ColU32(r, 7);
        rec.GuildTournamentScore = (uint32)ColU32(r, 8);
        rec.GTWinScore = (uint32)ColU32(r, 9);
        rec.isActive = (uint8)ColU32(r, 10);
        rec.Weeks = (uint8)ColU32(r, 11);
        rec.Week = (uint8)ColU32(r, 12);
        rec.Hour = (uint8)ColU32(r, 13);
        rec.Minute = (uint8)ColU32(r, 14);
        rec.TermHour = (uint8)ColU32(r, 15);
        rec.TermMinute = (uint8)ColU32(r, 16);
        rec.MatchCycleMin = (uint16)ColU32(r, 17);
        rec.ExploerTimeMin = (uint16)ColU32(r, 18);
        rec.WaitPlayTimeSec = (uint16)ColU32(r, 19);
        rec.PlayTime = (uint16)ColU32(r, 20);
        rec.Deadline = (uint16)ColU32(r, 21);
        rec.MatchDelay = (uint16)ColU32(r, 22);
        rec.Match_161 = (uint16)ColU32(r, 23);
        rec.Match_162 = (uint16)ColU32(r, 24);
        rec.Match_8 = (uint16)ColU32(r, 25);
        rec.Match_4 = (uint16)ColU32(r, 26);
        rec.Match_2 = (uint16)ColU32(r, 27);
        rec.Totlal = (uint32)ColU32(r, 28);
        rec.Rank = (uint8)ColU32(r, 29);
        rec.RewardGroup = (uint8)ColU32(r, 30);
        rec.RewardType = (uint32)ColU32(r, 31);
        rec.Value1 = (uint32)ColU32(r, 32);
        rec.Value2 = (uint32)ColU32(r, 33);
        rec.Value3 = (uint32)ColU32(r, 34);
        rec.IO_Str = (uint32)ColU32(r, 35);
        rec.IO_Con = (uint32)ColU32(r, 36);
        rec.IO_Dex = (uint32)ColU32(r, 37);
        rec.IO_Int = (uint32)ColU32(r, 38);
        rec.IO_Men = (uint32)ColU32(r, 39);
        rec.MinLv = (uint32)ColU32(r, 40);
        rec.MinMem = (uint32)ColU32(r, 41);
        rec.JoinMoney = (uint32)ColU32(r, 42);
        rec.MAP_TYPE = (uint16)ColU32(r, 43);
        rec.Index = (uint16)ColU32(r, 44);
        rec.DeathPoint = (uint16)ColU32(r, 45);
        { std::string s = ColStr(r, 46); size_t n = s.size(); if (n > sizeof(rec.StaName)-1) n = sizeof(rec.StaName)-1; memcpy(rec.StaName, s.data(), n); rec.StaName[n] = 0; }
        rec.TargetType = (uint32)ColU32(r, 47);
        rec.DlyTime = (uint32)ColU32(r, 48);
        rec.MAP_TYPE_1 = (uint16)ColU32(r, 49);
        rec.Index_1 = (uint16)ColU32(r, 50);
        rec.IconIndex = (uint32)ColU32(r, 51);
        { std::string s = ColStr(r, 52); size_t n = s.size(); if (n > sizeof(rec.IconFile)-1) n = sizeof(rec.IconFile)-1; memcpy(rec.IconFile, s.data(), n); rec.IconFile[n] = 0; }
        { std::string s = ColStr(r, 53); size_t n = s.size(); if (n > sizeof(rec.Name)-1) n = sizeof(rec.Name)-1; memcpy(rec.Name, s.data(), n); rec.Name[n] = 0; }
        { std::string s = ColStr(r, 54); size_t n = s.size(); if (n > sizeof(rec.Description)-1) n = sizeof(rec.Description)-1; memcpy(rec.Description, s.data(), n); rec.Description[n] = 0; }
        rec.MAP_TYPE_2 = (uint16)ColU32(r, 55);
        rec.LvGap = ColF32(r, 56);
        rec.PointRate = (uint16)ColU32(r, 57);
        rec.MAP_TYPE_3 = (uint16)ColU32(r, 58);
        rec.MyGrade = (uint32)ColU32(r, 59);
        rec.EnemyGrade0 = (uint32)ColU32(r, 60);
        rec.EnemyGrade1 = (uint32)ColU32(r, 61);
        rec.EnemyGrade2 = (uint32)ColU32(r, 62);
        rec.EnemyGrade3 = (uint32)ColU32(r, 63);
        rec.EnemyGrade4 = (uint32)ColU32(r, 64);
        rec.EnemyGrade5 = (uint32)ColU32(r, 65);
        rec.EnemyGrade6 = (uint32)ColU32(r, 66);
        rec.MAP_TYPE_4 = (uint16)ColU32(r, 67);
        rec.OccupyTime = (uint16)ColU32(r, 68);
        rec.Score = (uint16)ColU32(r, 69);
        rec.MAP_TYPE_5 = (uint16)ColU32(r, 70);
        { std::string s = ColStr(r, 71); size_t n = s.size(); if (n > sizeof(rec.StateName)-1) n = sizeof(rec.StateName)-1; memcpy(rec.StateName, s.data(), n); rec.StateName[n] = 0; }
        rec.WinScore = (uint32)ColU32(r, 72);
        Push((uint32)i, rec);
    }
    SHINELOG_INFO("GuildTournament: loaded %u rows", (uint32)m_uiTotal);
    return true;
}
StateFieldTab g_StateFieldTab;
bool StateFieldTab::Load(DataReader& reader) {
    std::vector<std::vector<std::string> > kRows;
    if (!reader.LoadAsRows("StateField", kRows)) return false;
    for (size_t i = 1; i < kRows.size(); ++i) {
        const std::vector<std::string>& r = kRows[i];
        StateFieldRow rec; memset(&rec, 0, sizeof(rec));
        { std::string s = ColStr(r, 0); size_t n = s.size(); if (n > sizeof(rec.AbStateInx)-1) n = sizeof(rec.AbStateInx)-1; memcpy(rec.AbStateInx, s.data(), n); rec.AbStateInx[n] = 0; }
        rec.MapName = (uint32)ColU32(r, 1);
        rec.StateSet = (uint32)ColU32(r, 2);
        Push((uint32)i, rec);
    }
    SHINELOG_INFO("StateField: loaded %u rows", (uint32)m_uiTotal);
    return true;
}
MIDungeonTab g_MIDungeonTab;
bool MIDungeonTab::Load(DataReader& reader) {
    std::vector<std::vector<std::string> > kRows;
    if (!reader.LoadAsRows("MIDungeon", kRows)) return false;
    for (size_t i = 1; i < kRows.size(); ++i) {
        const std::vector<std::string>& r = kRows[i];
        MIDungeonRow rec; memset(&rec, 0, sizeof(rec));
        rec.MID_MapInx = (uint32)ColU32(r, 0);
        rec.MID_MinLv = (uint8)ColU32(r, 1);
        rec.MID_MaxLv = (uint8)ColU32(r, 2);
        rec.MID_TankNum = (uint8)ColU32(r, 3);
        rec.MID_HealNum = (uint8)ColU32(r, 4);
        rec.MID_DealNum = (uint8)ColU32(r, 5);
        rec.MID_MinUser = (uint8)ColU32(r, 6);
        rec.MID_MaxUser = (uint8)ColU32(r, 7);
        rec.MID_MakeParty = (uint8)ColU32(r, 8);
        rec.MID_CompleteGroup = (uint8)ColU32(r, 9);
        rec.MID_StartTimeM = (uint16)ColU32(r, 10);
        Push((uint32)i, rec);
    }
    SHINELOG_INFO("MIDungeon: loaded %u rows", (uint32)m_uiTotal);
    return true;
}
MIDServerTab g_MIDServerTab;
bool MIDServerTab::Load(DataReader& reader) {
    std::vector<std::vector<std::string> > kRows;
    if (!reader.LoadAsRows("MIDServer", kRows)) return false;
    for (size_t i = 1; i < kRows.size(); ++i) {
        const std::vector<std::string>& r = kRows[i];
        MIDServerRow rec; memset(&rec, 0, sizeof(rec));
        rec.MID_MapInx = (uint32)ColU32(r, 0);
        { std::string s = ColStr(r, 1); size_t n = s.size(); if (n > sizeof(rec.MID_ScriptLanguage)-1) n = sizeof(rec.MID_ScriptLanguage)-1; memcpy(rec.MID_ScriptLanguage, s.data(), n); rec.MID_ScriptLanguage[n] = 0; }
        { std::string s = ColStr(r, 2); size_t n = s.size(); if (n > sizeof(rec.MID_PR_Inx)-1) n = sizeof(rec.MID_PR_Inx)-1; memcpy(rec.MID_PR_Inx, s.data(), n); rec.MID_PR_Inx[n] = 0; }
        Push((uint32)i, rec);
    }
    SHINELOG_INFO("MIDServer: loaded %u rows", (uint32)m_uiTotal);
    return true;
}
ChargedEffectTab g_ChargedEffectTab;
bool ChargedEffectTab::Load(DataReader& reader) {
    std::vector<std::vector<std::string> > kRows;
    if (!reader.LoadAsRows("ChargedEffect", kRows)) return false;
    for (size_t i = 1; i < kRows.size(); ++i) {
        const std::vector<std::string>& r = kRows[i];
        ChargedEffectRow rec; memset(&rec, 0, sizeof(rec));
        rec.Handle = (uint16)ColU32(r, 0);
        { std::string s = ColStr(r, 1); size_t n = s.size(); if (n > sizeof(rec.ItemID)-1) n = sizeof(rec.ItemID)-1; memcpy(rec.ItemID, s.data(), n); rec.ItemID[n] = 0; }
        rec.KeepTime_Hour = (uint16)ColU32(r, 2);
        rec.EffectEnum = (uint32)ColU32(r, 3);
        rec.EffectValue = (uint16)ColU32(r, 4);
        rec.StaStrength = (uint8)ColU32(r, 5);
        Push((uint32)i, rec);
    }
    SHINELOG_INFO("ChargedEffect: loaded %u rows", (uint32)m_uiTotal);
    return true;
}
AbStateViewTab g_AbStateViewTab;
bool AbStateViewTab::Load(DataReader& reader) {
    std::vector<std::vector<std::string> > kRows;
    if (!reader.LoadAsRows("AbStateView", kRows)) return false;
    for (size_t i = 1; i < kRows.size(); ++i) {
        const std::vector<std::string>& r = kRows[i];
        AbStateViewRow rec; memset(&rec, 0, sizeof(rec));
        rec.ID = (uint32)ColU32(r, 0);
        { std::string s = ColStr(r, 1); size_t n = s.size(); if (n > sizeof(rec.inxName)-1) n = sizeof(rec.inxName)-1; memcpy(rec.inxName, s.data(), n); rec.inxName[n] = 0; }
        rec.icon = (uint32)ColU32(r, 2);
        { std::string s = ColStr(r, 3); size_t n = s.size(); if (n > sizeof(rec.iconFile)-1) n = sizeof(rec.iconFile)-1; memcpy(rec.iconFile, s.data(), n); rec.iconFile[n] = 0; }
        { std::string s = ColStr(r, 4); size_t n = s.size(); if (n > sizeof(rec.Descript)-1) n = sizeof(rec.Descript)-1; memcpy(rec.Descript, s.data(), n); rec.Descript[n] = 0; }
        rec.R = (uint8)ColU32(r, 5);
        rec.G = (uint8)ColU32(r, 6);
        rec.B = (uint8)ColU32(r, 7);
        { std::string s = ColStr(r, 8); size_t n = s.size(); if (n > sizeof(rec.AniIndex)-1) n = sizeof(rec.AniIndex)-1; memcpy(rec.AniIndex, s.data(), n); rec.AniIndex[n] = 0; }
        { std::string s = ColStr(r, 9); size_t n = s.size(); if (n > sizeof(rec.effName)-1) n = sizeof(rec.effName)-1; memcpy(rec.effName, s.data(), n); rec.effName[n] = 0; }
        rec.EffNamePos = (uint32)ColU32(r, 10);
        rec.EffRefresh = (uint8)ColU32(r, 11);
        { std::string s = ColStr(r, 12); size_t n = s.size(); if (n > sizeof(rec.LoopEffect)-1) n = sizeof(rec.LoopEffect)-1; memcpy(rec.LoopEffect, s.data(), n); rec.LoopEffect[n] = 0; }
        rec.LoopEffPos = (uint32)ColU32(r, 13);
        { std::string s = ColStr(r, 14); size_t n = s.size(); if (n > sizeof(rec.LastEffect)-1) n = sizeof(rec.LastEffect)-1; memcpy(rec.LastEffect, s.data(), n); rec.LastEffect[n] = 0; }
        rec.LastEffectPos = (uint32)ColU32(r, 15);
        { std::string s = ColStr(r, 16); size_t n = s.size(); if (n > sizeof(rec.DOTEffect)-1) n = sizeof(rec.DOTEffect)-1; memcpy(rec.DOTEffect, s.data(), n); rec.DOTEffect[n] = 0; }
        rec.DOTEffectPos = (uint32)ColU32(r, 17);
        { std::string s = ColStr(r, 18); size_t n = s.size(); if (n > sizeof(rec.IconSort)-1) n = sizeof(rec.IconSort)-1; memcpy(rec.IconSort, s.data(), n); rec.IconSort[n] = 0; }
        rec.TypeSort = (uint32)ColU32(r, 19);
        rec.View = (uint8)ColU32(r, 20);
        Push((uint32)i, rec);
    }
    SHINELOG_INFO("AbStateView: loaded %u rows", (uint32)m_uiTotal);
    return true;
}
SetEffectTab g_SetEffectTab;
bool SetEffectTab::Load(DataReader& reader) {
    std::vector<std::vector<std::string> > kRows;
    if (!reader.LoadAsRows("SetEffect", kRows)) return false;
    for (size_t i = 1; i < kRows.size(); ++i) {
        const std::vector<std::string>& r = kRows[i];
        SetEffectRow rec; memset(&rec, 0, sizeof(rec));
        { std::string s = ColStr(r, 0); size_t n = s.size(); if (n > sizeof(rec.SetItemIndex)-1) n = sizeof(rec.SetItemIndex)-1; memcpy(rec.SetItemIndex, s.data(), n); rec.SetItemIndex[n] = 0; }
        rec.Count = (uint8)ColU32(r, 1);
        rec.ItemActionID = (uint16)ColU32(r, 2);
        Push((uint32)i, rec);
    }
    SHINELOG_INFO("SetEffect: loaded %u rows", (uint32)m_uiTotal);
    return true;
}
GradeItemOptionTab g_GradeItemOptionTab;
bool GradeItemOptionTab::Load(DataReader& reader) {
    std::vector<std::vector<std::string> > kRows;
    if (!reader.LoadAsRows("GradeItemOption", kRows)) return false;
    for (size_t i = 1; i < kRows.size(); ++i) {
        const std::vector<std::string>& r = kRows[i];
        GradeItemOptionRow rec; memset(&rec, 0, sizeof(rec));
        rec.ItemIndex = (uint32)ColU32(r, 0);
        rec.STR = (uint16)ColU32(r, 1);
        rec.CON = (uint16)ColU32(r, 2);
        rec.DEX = (uint16)ColU32(r, 3);
        rec.INT = (uint16)ColU32(r, 4);
        rec.MEN = (uint16)ColU32(r, 5);
        rec.ResistPoison = (uint16)ColU32(r, 6);
        rec.ResistDeaseas = (uint16)ColU32(r, 7);
        rec.ResistCurse = (uint16)ColU32(r, 8);
        rec.ResistMoveSpdDown = (uint16)ColU32(r, 9);
        rec.Critical = (uint16)ColU32(r, 10);
        rec.ToHitRate = (uint16)ColU32(r, 11);
        rec.ToHitPlus = (uint16)ColU32(r, 12);
        rec.ToBlockRate = (uint16)ColU32(r, 13);
        rec.ToBlockPlus = (uint16)ColU32(r, 14);
        rec.MaxHP = (uint16)ColU32(r, 15);
        rec.MaxSP = (uint16)ColU32(r, 16);
        rec.MoveSpdRate = (uint16)ColU32(r, 17);
        rec.AbsoluteAttack = (uint16)ColU32(r, 18);
        rec.PickupLimit = (uint16)ColU32(r, 19);
        Push((uint32)i, rec);
    }
    SHINELOG_INFO("GradeItemOption: loaded %u rows", (uint32)m_uiTotal);
    return true;
}
void RegisterAllSchemaTabs() {
    DataBox::Get().Register(&g_ItemInfoTab);
    DataBox::Get().Register(&g_AbStateTab);
    DataBox::Get().Register(&g_SubAbStateTab);
    DataBox::Get().Register(&g_ActiveSkillTab);
    DataBox::Get().Register(&g_MobInfoTab);
    DataBox::Get().Register(&g_ItemActionEffectTab);
    DataBox::Get().Register(&g_ItemActionConditionTab);
    DataBox::Get().Register(&g_ItemInfoServerTab);
    DataBox::Get().Register(&g_CharacterTitleDataTab);
    DataBox::Get().Register(&g_CollectCardTab);
    DataBox::Get().Register(&g_ActionRangeFactorTab);
    DataBox::Get().Register(&g_GTIServerTab);
    DataBox::Get().Register(&g_GuildTournamentTab);
    DataBox::Get().Register(&g_StateFieldTab);
    DataBox::Get().Register(&g_MIDungeonTab);
    DataBox::Get().Register(&g_MIDServerTab);
    DataBox::Get().Register(&g_ChargedEffectTab);
    DataBox::Get().Register(&g_AbStateViewTab);
    DataBox::Get().Register(&g_SetEffectTab);
    DataBox::Get().Register(&g_GradeItemOptionTab);
}
} // namespace fiesta