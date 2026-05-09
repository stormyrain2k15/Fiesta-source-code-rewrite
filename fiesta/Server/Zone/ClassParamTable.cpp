// Server/Zone/ClassParamTable.cpp
#include "ClassParamTable.h"
#include "../DataReader/TableScriptFile.h"
#include "../Shared/ShineLogSystem.h"
#include <stdlib.h>
#include <string.h>

namespace fiesta {

ClassParamTable& ClassParamTable::Get() { static ClassParamTable s; return s; }

// ---- Class name <-> enum / file ---------------------------------------------
struct CMap { eShineClass e; const char* sz; const char* file; };
static const CMap kMap[] = {
    { SC_FIGHTER,       "Fighter",       "ParamFighterServer.txt"       },
    { SC_CLEVERFIGHTER, "CleverFighter", "ParamCleverFighterServer.txt" },
    { SC_WARRIOR,       "Warrior",       "ParamWarriorServer.txt"       },
    { SC_GLADIATOR,     "Gladiator",     "ParamGladiatorServer.txt"     },
    { SC_KNIGHT,        "Knight",        "ParamKnightServer.txt"        },
    { SC_CLERIC,        "Cleric",        "ParamClericServer.txt"        },
    { SC_HIGHCLERIC,    "HighCleric",    "ParamHighClericServer.txt"    },
    { SC_PALADIN,       "Paladin",       "ParamPaladinServer.txt"       },
    { SC_HOLYKNIGHT,    "HolyKnight",    "ParamHolyKnightServer.txt"    },
    { SC_GUARDIAN,      "Guardian",      "ParamGuardianServer.txt"      },
    { SC_ARCHER,        "Archer",        "ParamArcherServer.txt"        },
    { SC_HAWKARCHER,    "HawkArcher",    "ParamHawkArcherServer.txt"    },
    { SC_SCOUT,         "Scout",         "ParamScoutServer.txt"         },
    { SC_SHARPSHOOTER,  "SharpShooter",  "ParamSharpShooterServer.txt"  },
    { SC_RANGER,        "Ranger",        "ParamRangerServer.txt"        },
    { SC_MAGE,          "Mage",          "ParamMageServer.txt"          },
    { SC_WIZMAGE,       "WizMage",       "ParamWizMageServer.txt"       },
    { SC_ENCHANTER,     "Enchanter",     "ParamEnchanterServer.txt"     },
    { SC_WARROCK,       "Warrock",       "ParamWarrockServer.txt"       },
    { SC_WIZARD,        "Wizard",        "ParamWizardServer.txt"        },
    { SC_JOKER,         "Joker",         "ParamJokerServer.txt"         },
    { SC_CHASER,        "Chaser",        "ParamChaserServer.txt"        },
    { SC_CRUEL,         "Cruel",         "ParamCruelServer.txt"         },
    { SC_CLOSER,        "Closer",        "ParamCloserServer.txt"        },
    { SC_ASSASSIN,      "Assassin",      "ParamAssassinServer.txt"      },
    { SC_SAVIOR,        "Savior",        "ParamSaviorServer.txt"        },
    { SC_SENTINEL,      "Sentinel",      "ParamSentinelServer.txt"      }
};
static const size_t kMapN = sizeof(kMap) / sizeof(kMap[0]);

eShineClass ClassParamTable::NameToClass(const std::string& rName) {
    for (size_t i = 0; i < kMapN; ++i)
        if (_stricmp(kMap[i].sz, rName.c_str()) == 0) return kMap[i].e;
    return SC_NONE;
}
const char* ClassParamTable::ClassToName(eShineClass e) {
    for (size_t i = 0; i < kMapN; ++i)
        if (kMap[i].e == e) return kMap[i].sz;
    return "Unknown";
}
const char* ClassParamTable::ClassToFile(eShineClass e) {
    for (size_t i = 0; i < kMapN; ++i)
        if (kMap[i].e == e) return kMap[i].file;
    return NULL;
}

// ---- Loader ----------------------------------------------------------------
bool ClassParamTable::LoadOneFile(const std::string& rPath, eShineClass eClass) {
    TableScriptFile f;
    if (!f.Load(rPath)) return false;
    const TsTable* t = f.Find("Param");
    if (!t) { SHINELOG_WARN("ClassParam: '%s' has no Param table", rPath.c_str()); return false; }

    // Column index resolution by name (column ordinals are stable across all
    // 22 supplied files; we still resolve by name to remain robust).
    int iLv  = t->ColIndex("Level");
    int iStr = t->ColIndex("Strength");
    int iCon = t->ColIndex("Constitution");
    int iInt = t->ColIndex("Intelligence");
    int iWis = t->ColIndex("Wizdom");        // "Wizdom" -- intentional in source files
    int iDex = t->ColIndex("Dexterity");
    int iMen = t->ColIndex("MentalPower");
    int iSoulHP    = t->ColIndex("SoulHP");
    int iMaxSoulHP = t->ColIndex("MAXSoulHP");
    int iPriceHP   = t->ColIndex("PriceHPStone");
    int iSoulSP    = t->ColIndex("SoulSP");
    int iMaxSoulSP = t->ColIndex("MAXSoulSP");
    int iPriceSP   = t->ColIndex("PriceSPStone");
    int iAtkPerAP  = t->ColIndex("AtkPerAP");
    int iDmgPerAP  = t->ColIndex("DmgPerAP");
    int iMaxPwr   = t->ColIndex("MaxPwrStone");
    int iNumPwr   = t->ColIndex("NumPwrStone");
    int iPricePwr = t->ColIndex("PricePwrStone");
    int iPwrWC    = t->ColIndex("PwrStoneWC");
    int iPwrMA    = t->ColIndex("PwrStoneMA");
    int iMaxGrd   = t->ColIndex("MaxGrdStone");
    int iNumGrd   = t->ColIndex("NumGrdStone");
    int iPriceGrd = t->ColIndex("PriceGrdStone");
    int iGrdAC    = t->ColIndex("GrdStoneAC");
    int iGrdMR    = t->ColIndex("GrdStoneMR");
    int iPain     = t->ColIndex("PainRes");
    int iRestraint= t->ColIndex("RestraintRes");
    int iCurse    = t->ColIndex("CurseRes");
    int iShock    = t->ColIndex("ShockRes");
    int iMaxHP    = t->ColIndex("MaxHP");
    int iMaxSP    = t->ColIndex("MaxSP");
    int iCharTitle= t->ColIndex("CharTitlePt");
    int iSkillPwr = t->ColIndex("SkillPwrPt");
    int iJobUp    = t->ColIndex("JobChangeDmgUp");

    int n = 0;
    for (size_t r = 0; r < t->kRecords.size(); ++r) {
        ClassParamRow x;
        memset(&x, 0, sizeof(x));
        x.uiLevel        = (uint16)t->GetInt(r, "Level");
        if (x.uiLevel == 0) continue;
        x.nStr           = (int32)t->GetInt(r, "Strength");
        x.nCon           = (int32)t->GetInt(r, "Constitution");
        x.nInt           = (int32)t->GetInt(r, "Intelligence");
        x.nWis           = (int32)t->GetInt(r, "Wizdom");
        x.nDex           = (int32)t->GetInt(r, "Dexterity");
        x.nMen           = (int32)t->GetInt(r, "MentalPower");
        x.nSoulHP        = (int32)t->GetInt(r, "SoulHP");
        x.nMaxSoulHP     = (int32)t->GetInt(r, "MAXSoulHP");
        x.nPriceHPStone  = (int32)t->GetInt(r, "PriceHPStone");
        x.nSoulSP        = (int32)t->GetInt(r, "SoulSP");
        x.nMaxSoulSP     = (int32)t->GetInt(r, "MAXSoulSP");
        x.nPriceSPStone  = (int32)t->GetInt(r, "PriceSPStone");
        x.nAtkPerAP      = (int32)t->GetInt(r, "AtkPerAP");
        x.nDmgPerAP      = (int32)t->GetInt(r, "DmgPerAP");
        x.nMaxPwrStone   = (int32)t->GetInt(r, "MaxPwrStone");
        x.nNumPwrStone   = (int32)t->GetInt(r, "NumPwrStone");
        x.nPricePwrStone = (int32)t->GetInt(r, "PricePwrStone");
        x.nPwrStoneWC    = (int32)t->GetInt(r, "PwrStoneWC");
        x.nPwrStoneMA    = (int32)t->GetInt(r, "PwrStoneMA");
        x.nMaxGrdStone   = (int32)t->GetInt(r, "MaxGrdStone");
        x.nNumGrdStone   = (int32)t->GetInt(r, "NumGrdStone");
        x.nPriceGrdStone = (int32)t->GetInt(r, "PriceGrdStone");
        x.nGrdStoneAC    = (int32)t->GetInt(r, "GrdStoneAC");
        x.nGrdStoneMR    = (int32)t->GetInt(r, "GrdStoneMR");
        x.nPainRes       = (int32)t->GetInt(r, "PainRes");
        x.nRestraintRes  = (int32)t->GetInt(r, "RestraintRes");
        x.nCurseRes      = (int32)t->GetInt(r, "CurseRes");
        x.nShockRes      = (int32)t->GetInt(r, "ShockRes");
        x.nMaxHP         = (int32)t->GetInt(r, "MaxHP");
        x.nMaxSP         = (int32)t->GetInt(r, "MaxSP");
        x.nCharTitlePt   = (int32)t->GetInt(r, "CharTitlePt");
        x.nSkillPwrPt    = (int32)t->GetInt(r, "SkillPwrPt");
        x.nJobChangeDmgUp= (int32)t->GetInt(r, "JobChangeDmgUp");
        uint32 key = ((uint32)eClass << 16) | (uint32)x.uiLevel;
        m_kRows[key] = x;
        ++n;
    }
    SHINELOG_INFO("ClassParam[%s]: %d rows", ClassToName(eClass), n);
    (void)iLv;(void)iStr;(void)iCon;(void)iInt;(void)iWis;(void)iDex;(void)iMen;
    (void)iSoulHP;(void)iMaxSoulHP;(void)iPriceHP;(void)iSoulSP;(void)iMaxSoulSP;(void)iPriceSP;
    (void)iAtkPerAP;(void)iDmgPerAP;(void)iMaxPwr;(void)iNumPwr;(void)iPricePwr;(void)iPwrWC;(void)iPwrMA;
    (void)iMaxGrd;(void)iNumGrd;(void)iPriceGrd;(void)iGrdAC;(void)iGrdMR;
    (void)iPain;(void)iRestraint;(void)iCurse;(void)iShock;(void)iMaxHP;(void)iMaxSP;
    (void)iCharTitle;(void)iSkillPwr;(void)iJobUp;
    return n > 0;
}

bool ClassParamTable::Load(const std::string& rRoot) {
    m_kRows.clear();
    int loaded = 0;
    for (size_t i = 0; i < kMapN; ++i) {
        std::string p = rRoot + "\\World\\" + kMap[i].file;
        if (LoadOneFile(p, kMap[i].e)) ++loaded;
    }
    SHINELOG_INFO("ClassParamTable: %d / %u class files loaded", loaded, (uint32)kMapN);
    return loaded > 0;
}

const ClassParamRow* ClassParamTable::Find(eShineClass eClass, uint16 uiLevel) const {
    uint32 key = ((uint32)eClass << 16) | (uint32)uiLevel;
    std::map<uint32, ClassParamRow>::const_iterator it = m_kRows.find(key);
    return (it == m_kRows.end()) ? NULL : &it->second;
}

// ---- Convenience getters (fall through to 0 if (class, level) is absent) ---
#define DEF_GETTER(NAME, FIELD) \
    int32 ClassParamTable::NAME(eShineClass eC, uint16 uiL) const { \
        const ClassParamRow* p = Find(eC, uiL); return p ? p->FIELD : 0; }
DEF_GETTER(SoulHpHealForLevel,    nSoulHP)
DEF_GETTER(SoulSpHealForLevel,    nSoulSP)
DEF_GETTER(SoulHpCapForLevel,     nMaxSoulHP)
DEF_GETTER(SoulSpCapForLevel,     nMaxSoulSP)
DEF_GETTER(SoulHpPriceForLevel,   nPriceHPStone)
DEF_GETTER(SoulSpPriceForLevel,   nPriceSPStone)
DEF_GETTER(PwrStoneCapForLevel,   nMaxPwrStone)
DEF_GETTER(PwrStonePerUseForLevel,nNumPwrStone)
DEF_GETTER(PwrStonePriceForLevel, nPricePwrStone)
DEF_GETTER(PwrStoneWcForLevel,    nPwrStoneWC)
DEF_GETTER(PwrStoneMaForLevel,    nPwrStoneMA)
DEF_GETTER(GrdStoneCapForLevel,   nMaxGrdStone)
DEF_GETTER(GrdStonePerUseForLevel,nNumGrdStone)
DEF_GETTER(GrdStonePriceForLevel, nPriceGrdStone)
DEF_GETTER(GrdStoneAcForLevel,    nGrdStoneAC)
DEF_GETTER(GrdStoneMrForLevel,    nGrdStoneMR)
#undef DEF_GETTER

} // namespace fiesta
