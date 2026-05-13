// Client/Engine/ShineLighting.cpp
#include "ShineLighting.h"
#include "../../Server/Shared/ShineLogSystem.h"
#include <cmath>
#ifndef M_PI
#define M_PI 3.14159265358979f
#endif

namespace shine {

void ShineLighting::SetupWorldLighting(NiNode* pkScene, float fTimeOfDay) {
    if (!pkScene) return;

    // ── Directional sun light ─────────────────────────────────────────────
    NiDirectionalLight* pkSun = NiNew NiDirectionalLight();
    pkSun->SetName("SunLight");

    // Warm midday sun color
    pkSun->SetDiffuseColor(NiColor(1.0f, 0.95f, 0.85f));
    pkSun->SetSpecularColor(NiColor(1.0f, 1.0f, 0.9f));
    pkSun->SetAmbientColor(NiColor(0.0f, 0.0f, 0.0f)); // ambient handled separately

    UpdateTimeOfDay(pkSun, fTimeOfDay);

    pkSun->SetEffectType(NiDynamicEffect::AMBIENT_EFFECT);
    pkScene->AttachEffect(pkSun);

    // ── Ambient fill light -- sky hemisphere feel ─────────────────────────
    NiAmbientLight* pkAmbient = NiNew NiAmbientLight();
    pkAmbient->SetName("SkyAmbient");
    // Cool blue-grey sky fill -- prevents pure black shadows
    pkAmbient->SetAmbientColor(NiColor(0.35f, 0.40f, 0.50f));
    pkAmbient->SetDiffuseColor(NiColor(0.0f, 0.0f, 0.0f));
    pkScene->AttachEffect(pkAmbient);

    // Force scene update to propagate lighting
    pkScene->UpdateEffects();
    pkScene->Update(0.0f);

    SHINELOG_INFO("ShineLighting: attached sun + ambient to scene");
}

void ShineLighting::UpdateTimeOfDay(NiDirectionalLight* pkSun, float fTimeOfDay) {
    if (!pkSun) return;

    // Clamp to [0,1]
    while (fTimeOfDay > 1.0f) fTimeOfDay -= 1.0f;
    while (fTimeOfDay < 0.0f) fTimeOfDay += 1.0f;

    // Sun elevation: rises at 0.25 (6am), peaks at 0.5 (noon), sets at 0.75 (6pm)
    // Angle in radians: -PI/2 at midnight, 0 at noon
    float fAngle = (fTimeOfDay - 0.5f) * (float)M_PI;
    float fElevation = cosf(fAngle);  // 1.0 at noon, 0.0 at horizon

    // Direction vector (sun shines DOWN and from the south-east)
    NiPoint3 kDir;
    kDir.x = -0.5f;                          // slightly east
    kDir.y = -(fElevation > 0.0f ? fElevation : 0.05f); // downward
    kDir.z = -0.5f;                          // slightly south
    kDir.Unitize();

    pkSun->SetWorldDirection(kDir);

    // Color shifts: warm orange at dawn/dusk, white at noon, dim at night
    float fIntensity = (fTimeOfDay > 0.25f && fTimeOfDay < 0.75f)
        ? sinf((fTimeOfDay - 0.25f) / 0.5f * (float)M_PI)
        : 0.05f;

    float fR = 0.9f + (1.0f - fIntensity) * 0.3f; // warm at low angles
    float fG = 0.85f * fIntensity;
    float fB = 0.75f * fIntensity;

    pkSun->SetDiffuseColor(NiColor(fR * fIntensity, fG, fB));
}

NiDirectionalLight* ShineLighting::GetSunLight(NiNode* pkScene) {
    if (!pkScene) return NULL;
    NiDynEffectIter kIter = pkScene->GetEffectHeadPos();
    NiDynamicEffect* pkEffect = NULL;
    while ((pkEffect = pkScene->GetNextEffect(kIter)) != NULL) {
        NiDirectionalLight* pkDir = NiDynamicCast(NiDirectionalLight, pkEffect);
        if (pkDir) return pkDir;
    }
    return NULL;
}

} // namespace shine
