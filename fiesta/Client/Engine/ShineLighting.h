// Client/Engine/ShineLighting.h
// Scene lighting setup.
// Attaches a directional sun light + ambient fill to the scene root.
// Light parameters match typical outdoor MMO lighting (warm sun, cool sky fill).
// Call SetupWorldLighting() once after LoadSGA() for the map.
#ifndef SHINE_CLIENT_ENGINE_SHINELIGHTING_H
#define SHINE_CLIENT_ENGINE_SHINELIGHTING_H
#include <NiMain.h>
#include "../../Server/Shared/ShineTypes.h"

namespace shine {

class ShineLighting {
public:
    // Attach sun + ambient lights to pkScene.
    // fTimeOfDay: 0.0=midnight 0.5=noon 1.0=midnight (wraps)
    static void SetupWorldLighting(NiNode* pkScene, float fTimeOfDay = 0.5f);

    // Update sun angle for time-of-day (call each frame if you want day cycle)
    static void UpdateTimeOfDay(NiDirectionalLight* pkSun, float fTimeOfDay);

    // Get the directional light from a scene (returns NULL if none attached)
    static NiDirectionalLight* GetSunLight(NiNode* pkScene);

private:
    ShineLighting() {}
};

} // namespace shine
#endif // SHINE_CLIENT_ENGINE_SHINELIGHTING_H
