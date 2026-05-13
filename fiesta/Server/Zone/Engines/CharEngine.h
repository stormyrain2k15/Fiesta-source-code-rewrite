// Server/Zone/Engines/CharEngine.h
// Owns all character-definition data: class/race/appearance options,
// starting loadout, and the GenerateCharacter() template builder.
// SHN inputs:  ClassName, RaceNameInfo, UseClassTypeInfo, HairInfo,
//              HairColorInfo, FaceInfo, ColorInfo, JobEquipInfo,
//              WeaponTitleData, CharacterTitleData
// TXT inputs:  DefaultCharacterData.txt, TutorialCharacterData.txt
#ifndef SHINE_ZONE_ENGINES_CHARENGINE_H
#define SHINE_ZONE_ENGINES_CHARENGINE_H
#include "../../DataReader/SHN/ClassName.h"
#include "../../DataReader/SHN/RaceNameInfo.h"
#include "../../DataReader/SHN/UseClassTypeInfo.h"
#include "../../DataReader/SHN/HairInfo.h"
#include "../../DataReader/SHN/HairColorInfo.h"
#include "../../DataReader/SHN/FaceInfo.h"
#include "../../DataReader/SHN/ColorInfo.h"
#include "../../DataReader/SHN/JobEquipInfo.h"
#include "../../DataReader/SHN/WeaponTitleData.h"
#include "../../DataReader/SHN/CharacterTitleData.h"
#include "../World/DefaultCharacterData.h"
#include "../World/TutorialCharacterData.h"
#include "../../Shared/ShineTypes.h"
#include <string>
#include <vector>

namespace shine {

// ----------------------------------------------------------------------------
// NewCharacterTemplate
// Assembled by GenerateCharacter(); handed to the character-creation packet
// handler to build the DB insert and the initial world-entry state.
// ----------------------------------------------------------------------------
struct NewCharacterTemplate {
    // Identity
    uint32      uiClass;
    std::string kClassName;

    // Starting world position (from DefaultCharacterData)
    std::string kStartMapName;
    int32       iStartPX;
    int32       iStartPY;

    // Starting vitals
    int32       iHP;
    int32       iSP;
    int32       iHPSoulStoneCount;
    int32       iSPSoulStoneCount;
    int32       iMoney;
    int32       iInitLevel;

    // Appearance (validated against SHN tables)
    uint32      uiHairType;
    uint32      uiHairColor;
    uint32      uiFaceType;

    // Starting equipment item indices (from JobEquipInfo)
    std::vector<std::string> kStartItemInxList;

    // True if template was built successfully; false if any lookup failed
    bool        bValid;
};

// ----------------------------------------------------------------------------
// CharEngine
// ----------------------------------------------------------------------------
class CharEngine {
public:
    static CharEngine& Get();

    // Load all SHN tables + DefaultCharacterData / TutorialCharacterData.
    // Call once at boot after ShnRegistry::LoadAll().
    // rShineRoot is the path to the Shine/ directory (for the .txt files).
    void Bind(const std::string& rShineRoot);

    // Build a NewCharacterTemplate for a new character creation request.
    // Returns template with bValid=false if the class/appearance combo is
    // invalid (unknown class, hair/face index out of range, etc.).
    NewCharacterTemplate GenerateCharacter(
        uint32             uiClass,
        uint32             uiHairType,
        uint32             uiHairColor,
        uint32             uiFaceType,
        bool               bTutorial = false) const;

    // Validation helpers used by the packet handler before calling Generate.
    bool IsValidClass   (uint32 uiClass)     const;
    bool IsValidHair    (uint32 uiHairType)  const;
    bool IsValidHairColor(uint32 uiColor)    const;
    bool IsValidFace    (uint32 uiFaceType)  const;

    // Direct SHN accessors for systems that need raw table data.
    ClassNameShn&            classNames()    { return ClassNameShn::Get(); }
    RaceNameInfoShn&         raceNames()     { return RaceNameInfoShn::Get(); }
    UseClassTypeInfoShn&     classTypeInfo() { return UseClassTypeInfoShn::Get(); }
    HairInfoShn&             hairInfo()      { return HairInfoShn::Get(); }
    HairColorInfoShn&        hairColorInfo() { return HairColorInfoShn::Get(); }
    FaceInfoShn&             faceInfo()      { return FaceInfoShn::Get(); }
    ColorInfoShn&            colorInfo()     { return ColorInfoShn::Get(); }
    JobEquipInfoShn&         jobEquipInfo()  { return JobEquipInfoShn::Get(); }
    WeaponTitleDataShn&      weaponTitles()  { return WeaponTitleDataShn::Get(); }
    CharacterTitleDataShn&   charTitles()    { return CharacterTitleDataShn::Get(); }
    DefaultCharacterDataTable&  defaultData()   { return DefaultCharacterDataTable::Get(); }
    TutorialCharacterDataTable& tutorialData()  { return TutorialCharacterDataTable::Get(); }

private:
    CharEngine() {}
};

} // namespace shine
#endif // SHINE_ZONE_ENGINES_CHARENGINE_H
