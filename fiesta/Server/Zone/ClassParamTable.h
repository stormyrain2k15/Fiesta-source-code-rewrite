// Server/Zone/ClassParamTable.h
// one loader for each per-class progression file:
//   World/ParamFighterServer.txt
//   World/ParamCleverFighterServer.txt
//   World/ParamWarriorServer.txt           ... (22 classes total)
//   ... etc
// Each file ships exactly one "Param" table with these 35 columns:
//   Level Strength Constitution Intelligence Wisdom Dexterity MentalPower
//   SoulHP MAXSoulHP PriceHPStone
//   SoulSP MAXSoulSP PriceSPStone
//   AtkPerAP DmgPerAP
//   MaxPwrStone NumPwrStone PricePwrStone PwrStoneWC PwrStoneMA
//   MaxGrdStone NumGrdStone PriceGrdStone GrdStoneAC GrdStoneMR
//   PainRes RestraintRes CurseRes ShockRes
//   MaxHP MaxSP
//   CharTitlePt SkillPwrPt JobChangeDmgUp
// Semantics (verified from ParamFighterServer.txt rows):
//   * MAXSoulHP / MAXSoulSP = how many HP / SP soul stones the character
//     may hold at this level (true per-level cap; replaces my old fake
//     `kSoulStoneMaxCount`).
//   * SoulHP / SoulSP        = the heal amount per stone at this level
//     (replaces my old fake `kHpSoulHealByTier[]`).
//   * PriceHPStone / PriceSPStone = vendor price per stone at this level.
//   * MaxPwrStone / NumPwrStone / PricePwrStone / PwrStoneWC / PwrStoneMA
//                            = full Power Stone (offense buff stone) ladder.
//   * MaxGrdStone / NumGrdStone / PriceGrdStone / GrdStoneAC / GrdStoneMR
//                            = full Guard Stone (defense buff stone) ladder.
//   * PainRes / RestraintRes / CurseRes / ShockRes = base abstate resists
//     at this level (added to any equipment / buff modifiers).
//   * MaxHP / MaxSP          = base ceiling at this (class, level) -- the
//     "BaseHP/BaseSP" my MoverMain loader looked for.
//   * CharTitlePt / SkillPwrPt = points granted on level-up.
//   * JobChangeDmgUp         = damage bonus after job change (1000 = 1.0x).
//                                   World/Param*Server.txt files.
#ifndef SHINE_ZONE_CLASSPARAMTABLE_H
#define SHINE_ZONE_CLASSPARAMTABLE_H
#include "../Shared/ShineTypes.h"
#include <map>
#include <string>
#include <vector>

namespace shine {

// Class IDs (verbatim from QuestParser.txt).
enum eShineClass {
    SC_NONE          = 0,
    SC_FIGHTER       = 1,
    SC_CLEVERFIGHTER = 2,
    SC_WARRIOR       = 3,
    SC_GLADIATOR     = 4,
    SC_KNIGHT        = 5,
    SC_CLERIC        = 6,
    SC_HIGHCLERIC    = 7,
    SC_PALADIN       = 8,
    SC_HOLYKNIGHT    = 9,
    SC_GUARDIAN      = 10,
    SC_ARCHER        = 11,
    SC_HAWKARCHER    = 12,
    SC_SCOUT         = 13,
    SC_SHARPSHOOTER  = 14,
    SC_RANGER        = 15,
    SC_MAGE          = 16,
    SC_WIZMAGE       = 17,
    SC_ENCHANTER     = 18,
    SC_WARROCK       = 19,
    SC_WIZARD        = 20,
    SC_JOKER         = 21,
    SC_CHASER        = 22,
    SC_CRUEL         = 23,
    SC_CLOSER        = 24,
    SC_ASSASSIN      = 25,
    SC_SAVIOR        = 26,           // 4th-job lines (Sentinel/Savior)
    SC_SENTINEL      = 27,
    SC_MAX           = 32
};

struct ClassParamRow {
    uint16 uiLevel;
    int32  nStr, nCon, nInt, nWis, nDex, nMen;
    int32  nSoulHP, nMaxSoulHP, nPriceHPStone;
    int32  nSoulSP, nMaxSoulSP, nPriceSPStone;
    int32  nAtkPerAP, nDmgPerAP;
    int32  nMaxPwrStone, nNumPwrStone, nPricePwrStone, nPwrStoneWC, nPwrStoneMA;
    int32  nMaxGrdStone, nNumGrdStone, nPriceGrdStone, nGrdStoneAC, nGrdStoneMR;
    int32  nPainRes, nRestraintRes, nCurseRes, nShockRes;
    int32  nMaxHP, nMaxSP;
    int32  nCharTitlePt, nSkillPwrPt;
    int32  nJobChangeDmgUp;
};

class ClassParamTable {
public:
    static ClassParamTable& Get();

    // Loads every Param<Class>Server.txt that exists under `rRoot/World/`.
    bool Load(const std::string& rRoot);

    // Lookup at (class, level).
    const ClassParamRow* Find(eShineClass eClass, uint16 uiLevel) const;

    // Convenience accessors for the soul-stone system.
    int32 SoulHpHealForLevel  (eShineClass eClass, uint16 uiLevel) const;
    int32 SoulSpHealForLevel  (eShineClass eClass, uint16 uiLevel) const;
    int32 SoulHpCapForLevel   (eShineClass eClass, uint16 uiLevel) const;
    int32 SoulSpCapForLevel   (eShineClass eClass, uint16 uiLevel) const;
    int32 SoulHpPriceForLevel (eShineClass eClass, uint16 uiLevel) const;
    int32 SoulSpPriceForLevel (eShineClass eClass, uint16 uiLevel) const;

    // Power Stone / Guard Stone vendor accessors (Pwr boosts WC/MA, Grd boosts AC/MR).
    int32 PwrStoneCapForLevel (eShineClass eClass, uint16 uiLevel) const;
    int32 PwrStonePerUseForLevel(eShineClass eClass, uint16 uiLevel) const;
    int32 PwrStonePriceForLevel(eShineClass eClass, uint16 uiLevel) const;
    int32 PwrStoneWcForLevel  (eShineClass eClass, uint16 uiLevel) const;
    int32 PwrStoneMaForLevel  (eShineClass eClass, uint16 uiLevel) const;
    int32 GrdStoneCapForLevel (eShineClass eClass, uint16 uiLevel) const;
    int32 GrdStonePerUseForLevel(eShineClass eClass, uint16 uiLevel) const;
    int32 GrdStonePriceForLevel(eShineClass eClass, uint16 uiLevel) const;
    int32 GrdStoneAcForLevel  (eShineClass eClass, uint16 uiLevel) const;
    int32 GrdStoneMrForLevel  (eShineClass eClass, uint16 uiLevel) const;

    // String <-> enum helpers.
    static eShineClass NameToClass(const std::string& rName);
    static const char* ClassToName(eShineClass e);
    static const char* ClassToFile(eShineClass e); // returns "ParamFighterServer.txt" etc.

private:
    ClassParamTable() {}
    bool LoadOneFile(const std::string& rPath, eShineClass eClass);

    // key = ((uint32)class << 16) | level  ->  row
    std::map<uint32, ClassParamRow> m_kRows;
};

} // namespace shine
#endif
