// Client/ResSystem/CharacterLoader.h
// Resolves a character class ID → InxName → NIF path + action dat.
// Called by ShineApp::OnZoneReady to load the player model into the scene.
//
// Path convention (matching reschar layout):
//   reschar\<InxName>-<Gender>\body.nif -> renamed .sga
//   ressystem\action\<InxName>-<Gender>.dat
//
// Gender suffix: "M" for male classes, "F" for female classes.
// Some classes have no gender variant (NPCs, mobs) -- just InxName.dat
#ifndef SHINE_CLIENT_RESSYSTEM_CHARACTERLOADER_H
#define SHINE_CLIENT_RESSYSTEM_CHARACTERLOADER_H

#include "../../Server/Shared/ShineTypes.h"
#include "../Engine/ShineScene.h"
#include "ActionDat.h"
#include <string>

namespace shine {

enum CharGender { GENDER_MALE = 0, GENDER_FEMALE = 1 };

struct CharacterAssets {
    std::string      kInxName;       // e.g. "Archer"
    std::string      kGenderedInx;   // e.g. "Archer-M"
    std::string      kBodySGAPath;   // full path to body .sga
    std::string      kActionDatPath; // full path to action .dat
    const ActionDat* pkActionDat;    // loaded action dat (from cache)
    NiAVObject*      pkNode;         // scene node once loaded
    bool             bLoaded;

    CharacterAssets() : pkActionDat(NULL), pkNode(NULL), bLoaded(false) {}
};

class CharacterLoader {
public:
    static CharacterLoader& Get();

    // Resolve and load everything for a player class + gender.
    // uiClass = class ID from NC_CHAR_INFO_CMD
    // eGender = GENDER_MALE or GENDER_FEMALE
    // pkScene = scene to attach the loaded node to
    CharacterAssets LoadPlayer(uint16 uiClass, CharGender eGender, ShineScene* pkScene);

    // Load a mob/NPC model by InxName (no gender variant)
    CharacterAssets LoadActor(const std::string& rInxName, ShineScene* pkScene);

    // Resolve class ID → InxName from ClassName.shn
    // Returns empty string if class not found
    std::string ResolveClassInx(uint16 uiClass) const;

private:
    CharacterLoader() {}

    CharacterAssets LoadInternal(const std::string& rGenderedInx,
                                  const std::string& rBaseInx,
                                  ShineScene* pkScene);

    // Class ID → gender mapping (populated from ClassName.shn at first call)
    void EnsureClassMap();
    bool m_bClassMapReady = false;
};

} // namespace shine
#endif // SHINE_CLIENT_RESSYSTEM_CHARACTERLOADER_H
