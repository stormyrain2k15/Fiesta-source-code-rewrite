// Client/Engine/AnimationLink.cpp
#include "AnimationLink.h"
#include "../../Server/Shared/ShineLogSystem.h"

namespace fiesta {

std::string AnimationLink::ResolveClip(const DatFile& rDat, uint16 uiActionId) {
    const std::vector<DatEntry>& v = rDat.Entries();
    if (uiActionId < v.size()) return v[uiActionId].kClipName;
    // Some .dat files use uiKind as a direct mapping rather than positional
    // index -- fall back to scanning by uiKind == actionId.
    for (size_t i = 0; i < v.size(); ++i)
        if (v[i].uiKind == uiActionId) return v[i].kClipName;
    return std::string();
}

bool AnimationLink::Play(NiAVObject* pkRoot, const std::string& rClip, float fBlend) {
    (void)pkRoot; (void)fBlend;
    // Wired against Gamebryo in pass 2:
    //   NiControllerManager* pkMgr = (NiControllerManager*)pkRoot->GetControllerManager();
    //   NiControllerSequence* pkSeq = pkMgr ? pkMgr->FindSequence(rClip.c_str()) : 0;
    //   if (pkSeq) pkMgr->ActivateSequence(pkSeq, 0, true, 1.0f, fBlend);
    SHINELOG_DEBUG("AnimationLink::Play '%s' (Gamebryo wiring deferred)", rClip.c_str());
    return !rClip.empty();
}

} // namespace fiesta
