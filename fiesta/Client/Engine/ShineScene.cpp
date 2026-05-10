// Client/Engine/ShineScene.cpp
#include "ShineScene.h"
#include "../../Server/Shared/ShineLogSystem.h"
#include <NiStream.h>

namespace fiesta {

ShineScene::ShineScene() : m_pkRenderer(NULL) {}
ShineScene::~ShineScene() { Shutdown(); }

bool ShineScene::Init(NiRenderer* pkRenderer) {
    m_pkRenderer = pkRenderer;
    m_spRoot     = NiNew NiNode();
    m_spRoot->SetName("ShineRoot");
    SHINELOG_INFO("ShineScene: initialized");
    return true;
}

void ShineScene::Tick(float fDt) {
    if (!m_spRoot) return;
    m_spRoot->Update(fDt);
}

void ShineScene::Shutdown() {
    m_spRoot = NULL;
    m_pkRenderer = NULL;
}

NiAVObject* ShineScene::LoadSGA(const std::string& rPath) {
    if (!m_spRoot) return NULL;

    // .sga is NIF binary -- NiStream loads it transparently regardless of extension.
    NiStream kStream;
    if (!kStream.Load(rPath.c_str())) {
        SHINELOG_WARN("ShineScene::LoadSGA: NiStream::Load failed '%s'", rPath.c_str());
        return NULL;
    }

    if (kStream.GetObjectCount() == 0) {
        SHINELOG_WARN("ShineScene::LoadSGA: empty stream '%s'", rPath.c_str());
        return NULL;
    }

    // First root object is the scene node
    NiObject* pkObj = kStream.GetObjectAt(0);
    if (!pkObj || !NiIsKindOf(NiAVObject, pkObj)) {
        SHINELOG_WARN("ShineScene::LoadSGA: root is not NiAVObject '%s'", rPath.c_str());
        return NULL;
    }

    NiAVObject* pkAV = (NiAVObject*)pkObj;
    m_spRoot->AttachChild(pkAV);
    m_spRoot->Update(0.0f);
    m_spRoot->UpdateProperties();
    m_spRoot->UpdateEffects();

    SHINELOG_INFO("ShineScene::LoadSGA: loaded '%s'", rPath.c_str());
    return pkAV;
}

void ShineScene::ClearScene() {
    if (m_spRoot)
        m_spRoot->DetachAllChildren();
}

} // namespace fiesta
