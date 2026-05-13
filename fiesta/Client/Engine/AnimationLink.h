// Client/Engine/AnimationLink.h
// runtime resolver for the SHN -> DAT -> NIF chain.
// Server emits an action opcode with (objectHandle, actionId). Client looks up
// the action set name from the entity's MobInfo/NPCInfo row, finds the matching
// Action/<Name>.dat, picks the clip name for actionId, and tells Gamebryo's
// NiControllerManager on the loaded NIF to play that NiControllerSequence.
#ifndef SHINE_CLIENT_ANIMATIONLINK_H
#define SHINE_CLIENT_ANIMATIONLINK_H
#include "../../Server/Shared/ShineTypes.h"
#include "../../Server/DataReader/DatFile.h"
#include <string>

class NiControllerManager;
class NiControllerSequence;
class NiAVObject;

namespace shine {

class AnimationLink {
public:
    // Resolve a server action id against a previously-loaded Action .dat.
    // Returns the clip name or empty string if not found.
    static std::string ResolveClip(const DatFile& rActionDat, uint16 uiActionId);

    // Once the NIF is loaded by Gamebryo, hand the resolved clip name to the
    // model's NiControllerManager. Real impl uses NiControllerManager::FindSequence
    // + ActivateSequence; pass-2 fills in once the Gamebryo headers are
    // wired into the client VS project.
    static bool Play(NiAVObject* pkModelRoot, const std::string& rClipName, float fBlend);
};

} // namespace shine
#endif
