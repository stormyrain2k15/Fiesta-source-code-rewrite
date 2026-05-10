// Server/Zone/Engines/CharEngine.cpp
// Character definition engine: loads all char-creation data and
// assembles NewCharacterTemplate on demand.
#include "CharEngine.h"
#include "../../Shared/ShineLogSystem.h"

namespace fiesta {

CharEngine& CharEngine::Get() { static CharEngine s; return s; }

void CharEngine::Bind(const std::string& rShineRoot) {
    // -- SHN tables (already in ShnRegistry after LoadAll) --
    ClassNameShn::Get()          .Load();
    RaceNameInfoShn::Get()       .Load();
    UseClassTypeInfoShn::Get()   .Load();
    HairInfoShn::Get()           .Load();
    HairColorInfoShn::Get()      .Load();
    FaceInfoShn::Get()           .Load();
    ColorInfoShn::Get()          .Load();
    JobEquipInfoShn::Get()       .Load();
    WeaponTitleDataShn::Get()    .Load();
    CharacterTitleDataShn::Get() .Load();

    // -- Root TXT files (co-located with SHNs) --
    DefaultCharacterDataTable::Get() .Load(rShineRoot + "\\DefaultCharacterData.txt");
    TutorialCharacterDataTable::Get().Load(rShineRoot + "\\TutorialCharacterData.txt");

    SHINELOG_INFO("CharEngine::Bind done -- %u classes, %u hair, %u face, %u color",
        (uint32)ClassNameShn::Get().Count(),
        (uint32)HairInfoShn::Get().Count(),
        (uint32)FaceInfoShn::Get().Count(),
        (uint32)ColorInfoShn::Get().Count());
}

// ── validation ───────────────────────────────────────────────────────────────

bool CharEngine::IsValidClass(uint32 uiClass) const {
    return ClassNameShn::Get().FindById(uiClass) != NULL;
}

bool CharEngine::IsValidHair(uint32 uiHairType) const {
    return HairInfoShn::Get().FindById(uiHairType) != NULL;
}

bool CharEngine::IsValidHairColor(uint32 uiColor) const {
    return HairColorInfoShn::Get().FindById(uiColor) != NULL;
}

bool CharEngine::IsValidFace(uint32 uiFaceType) const {
    return FaceInfoShn::Get().FindById(uiFaceType) != NULL;
}

// ── GenerateCharacter ─────────────────────────────────────────────────────────

NewCharacterTemplate CharEngine::GenerateCharacter(
    uint32      uiClass,
    uint32      uiHairType,
    uint32      uiHairColor,
    uint32      uiFaceType,
    bool        bTutorial) const
{
    NewCharacterTemplate tmpl;
    tmpl.bValid      = false;
    tmpl.uiClass     = uiClass;
    tmpl.uiHairType  = uiHairType;
    tmpl.uiHairColor = uiHairColor;
    tmpl.uiFaceType  = uiFaceType;

    // -- 1. Validate class --
    const LegacyClassNameRow* pClass = ClassNameShn::Get().FindById(uiClass);
    if (!pClass) {
        SHINELOG_WARN("CharEngine::GenerateCharacter: unknown class %u", uiClass);
        return tmpl;
    }
    tmpl.kClassName = pClass->kAcEngName;

    // -- 2. Validate appearance --
    if (!IsValidHair(uiHairType)) {
        SHINELOG_WARN("CharEngine::GenerateCharacter: invalid hair type %u", uiHairType);
        return tmpl;
    }
    if (!IsValidHairColor(uiHairColor)) {
        SHINELOG_WARN("CharEngine::GenerateCharacter: invalid hair color %u", uiHairColor);
        return tmpl;
    }
    if (!IsValidFace(uiFaceType)) {
        SHINELOG_WARN("CharEngine::GenerateCharacter: invalid face type %u", uiFaceType);
        return tmpl;
    }

    // -- 3. Pull starting stats from DefaultCharacterData / TutorialCharacterData --
    const std::vector<DefaultCharacterDataRow>& kDefRows =
        DefaultCharacterDataTable::Get().Rows();
    const std::vector<TutorialCharacterDataRow>& kTutRows =
        TutorialCharacterDataTable::Get().Rows();

    const std::vector<DefaultCharacterDataRow>*  pSrc    = &kDefRows;
    bool bFoundStats = false;

    if (bTutorial) {
        // Tutorial path: match class in TutorialCharacterData
        for (size_t i = 0; i < kTutRows.size(); ++i) {
            if ((uint32)kTutRows[i].iClass == uiClass) {
                tmpl.kStartMapName      = kTutRows[i].kStartMapName;
                tmpl.iStartPX           = kTutRows[i].iStartPX;
                tmpl.iStartPY           = kTutRows[i].iStartPY;
                tmpl.iHP                = kTutRows[i].iHP;
                tmpl.iSP                = kTutRows[i].iSP;
                tmpl.iHPSoulStoneCount  = kTutRows[i].iHPSoulStoneCount;
                tmpl.iSPSoulStoneCount  = kTutRows[i].iSPSoulStoneCount;
                tmpl.iMoney             = kTutRows[i].iMoney;
                tmpl.iInitLevel         = 1;
                bFoundStats = true;
                break;
            }
        }
    }

    if (!bFoundStats) {
        // Normal path: match class in DefaultCharacterData
        for (size_t i = 0; i < kDefRows.size(); ++i) {
            if ((uint32)kDefRows[i].iClass == uiClass) {
                tmpl.kStartMapName      = kDefRows[i].kStartMapName;
                tmpl.iStartPX           = kDefRows[i].iStartPX;
                tmpl.iStartPY           = kDefRows[i].iStartPY;
                tmpl.iHP                = kDefRows[i].iHP;
                tmpl.iSP                = kDefRows[i].iSP;
                tmpl.iHPSoulStoneCount  = kDefRows[i].iHPSoulStoneCount;
                tmpl.iSPSoulStoneCount  = kDefRows[i].iSPSoulStoneCount;
                tmpl.iMoney             = kDefRows[i].iMoney;
                tmpl.iInitLevel         = kDefRows[i].iInitLV;
                bFoundStats = true;
                break;
            }
        }
    }

    if (!bFoundStats) {
        SHINELOG_WARN("CharEngine::GenerateCharacter: no DefaultCharacterData row for class %u",
                      uiClass);
        return tmpl;
    }

    // -- 4. Collect starting equipment from JobEquipInfo --
    // JobEquipInfo.shn has 7 fixed equip slots per class row (RightHand,
    // LeftHand, Shoes, Head, Leg, Body, ETC). Each non-empty slot produces
    // one starting-item index for the new character.
    const std::vector<JobEquipInfoRow>& kEquip = JobEquipInfoShn::Get().Rows();
    for (size_t i = 0; i < kEquip.size(); ++i) {
        if (kEquip[i].uiChrClass != uiClass) continue;
        if (!kEquip[i].kEqu_RightHand.empty()) tmpl.kStartItemInxList.push_back(kEquip[i].kEqu_RightHand);
        if (!kEquip[i].kEqu_LeftHand.empty())  tmpl.kStartItemInxList.push_back(kEquip[i].kEqu_LeftHand);
        if (!kEquip[i].kEqu_Shoes.empty())     tmpl.kStartItemInxList.push_back(kEquip[i].kEqu_Shoes);
        if (!kEquip[i].kEqu_Head.empty())      tmpl.kStartItemInxList.push_back(kEquip[i].kEqu_Head);
        if (!kEquip[i].kEqu_Leg.empty())       tmpl.kStartItemInxList.push_back(kEquip[i].kEqu_Leg);
        if (!kEquip[i].kEqu_Body.empty())      tmpl.kStartItemInxList.push_back(kEquip[i].kEqu_Body);
        if (!kEquip[i].kEqu_ETC.empty())       tmpl.kStartItemInxList.push_back(kEquip[i].kEqu_ETC);
    }

    tmpl.bValid = true;
    return tmpl;
}

} // namespace fiesta
