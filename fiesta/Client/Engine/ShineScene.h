// Client/Engine/ShineScene.h
// Scene graph wrapper: owns the NiNode root, loads .sga assets (NIF under the hood),
// drives animation updates.
#ifndef SHINE_CLIENT_ENGINE_SHINESCENE_H
#define SHINE_CLIENT_ENGINE_SHINESCENE_H

#include <NiMain.h>
#include <NiAnimation.h>
#include "../../Server/Shared/ShineTypes.h"
#include <string>

namespace fiesta {

class ShineScene {
public:
    ShineScene();
    ~ShineScene();

    bool        Init(NiRenderer* pkRenderer);
    void        Tick(float fDt);
    void        Shutdown();

    // Load a .sga asset (NIF binary under the extension).
    // Returns the loaded NiAVObject attached to scene root, or NULL on failure.
    NiAVObject* LoadSGA(const std::string& rPath);

    // Clear all loaded objects from root
    void        ClearScene();

    NiNode*     GetRoot() { return m_spRoot; }

private:
    NiNodePtr   m_spRoot;
    NiRenderer* m_pkRenderer;
};

} // namespace fiesta
#endif // SHINE_CLIENT_ENGINE_SHINESCENE_H
