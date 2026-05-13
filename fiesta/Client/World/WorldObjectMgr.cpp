// Client/World/WorldObjectMgr.cpp
#include "WorldObjectMgr.h"
#include "../Engine/ShineConfig.h"
#include "../../Server/Shared/ShineLogSystem.h"
#include "../../Server/Common/NETCOMMAND.h"

namespace shine {

WorldObjectMgr::WorldObjectMgr() : m_pkScene(NULL) {}

void WorldObjectMgr::Init(ShineScene* pkScene) {
    m_pkScene = pkScene;
    SHINELOG_INFO("WorldObjectMgr: init");
}

void WorldObjectMgr::Shutdown() {
    // Detach all object nodes from scene
    for (std::map<uint32,WorldObject>::iterator it = m_kObjects.begin();
         it != m_kObjects.end(); ++it) {
        if (it->second.pkNode && m_pkScene && m_pkScene->GetRoot())
            m_pkScene->GetRoot()->DetachChild(it->second.pkNode);
    }
    m_kObjects.clear();
}

void WorldObjectMgr::Tick(float fDt) {
    // Update animations for each object (future: blend tree, attack anims)
    // For now just let NiTimeController handle it via scene tick in ShineScene::Tick
}

// ── NC packet handlers ────────────────────────────────────────────────────────

void WorldObjectMgr::OnBriefInfoLogin(const GPacket& rPkt) {
    // NC_CHAR_BRIEFINFO_LOGINCHARACTER_CMD
    // [handle(4)][name(str)][level(2)][class(2)][posX(4f)][posY(4f)][posZ(4f)][facing(2)]
    PacketBuffer body = rPkt.Body();
    WorldObject obj;
    obj.eType = WOT_PLAYER;
    body.ReadU32(obj.uiHandle);
    body.ReadString(obj.kName);
    body.ReadU16(obj.uiLevel);
    body.ReadU16(obj.uiClass);
    body.ReadF32(obj.fX);
    body.ReadF32(obj.fY);
    body.ReadF32(obj.fZ);
    uint16 uiFacing = 0; body.ReadU16(uiFacing);
    obj.fFacing = uiFacing * (360.0f / 65535.0f);

    // Derive InxName from class for action dat
    obj.kInxName = CharacterLoader::Get().ResolveClassInx(obj.uiClass);

    SHINELOG_DEBUG("WorldObjectMgr: player '%s' lv=%u class=%u handle=%u",
                   obj.kName.c_str(), obj.uiLevel, obj.uiClass, obj.uiHandle);

    m_kObjects[obj.uiHandle] = obj;
    SpawnObject(m_kObjects[obj.uiHandle]);
}

void WorldObjectMgr::OnCharDisappear(const GPacket& rPkt) {
    // NC_CHAR_DISAPPEAR_CMD: [handle(4)]
    PacketBuffer body = rPkt.Body();
    uint32 uiHandle = 0;
    body.ReadU32(uiHandle);
    DespawnObject(uiHandle);
    SHINELOG_DEBUG("WorldObjectMgr: despawn handle=%u", uiHandle);
}

void WorldObjectMgr::OnObjMoveRun(const GPacket& rPkt) {
    PacketBuffer body = rPkt.Body();
    uint32 uiHandle = 0; float fX=0,fY=0,fZ=0;
    body.ReadU32(uiHandle);
    body.ReadF32(fX); body.ReadF32(fY); body.ReadF32(fZ);
    std::map<uint32,WorldObject>::iterator it = m_kObjects.find(uiHandle);
    if (it != m_kObjects.end())
        UpdateObjectPosition(it->second, fX, fY, fZ);
}

void WorldObjectMgr::OnObjMoveWalk(const GPacket& rPkt) {
    OnObjMoveRun(rPkt); // same payload, different animation speed
}

void WorldObjectMgr::OnObjStop(const GPacket& rPkt) {
    PacketBuffer body = rPkt.Body();
    uint32 uiHandle = 0; float fX=0,fY=0,fZ=0;
    body.ReadU32(uiHandle);
    body.ReadF32(fX); body.ReadF32(fY); body.ReadF32(fZ);
    std::map<uint32,WorldObject>::iterator it = m_kObjects.find(uiHandle);
    if (it != m_kObjects.end()) {
        it->second.fX = fX;
        it->second.fY = fY;
        it->second.fZ = fZ;
        // TODO: trigger idle/stand action from dat when animation system is wired
    }
}

void WorldObjectMgr::OnObjStatusChange(const GPacket& rPkt) {
    // [handle(4)][hp(4)][maxhp(4)]
    PacketBuffer body = rPkt.Body();
    uint32 uiHandle=0; int32 iHP=0, iMaxHP=0;
    body.ReadU32(uiHandle);
    body.ReadI32(iHP);
    body.ReadI32(iMaxHP);
    std::map<uint32,WorldObject>::iterator it = m_kObjects.find(uiHandle);
    if (it != m_kObjects.end()) {
        it->second.iHP    = iHP;
        it->second.iMaxHP = iMaxHP;
    }
}

const WorldObject* WorldObjectMgr::FindByHandle(uint32 uiHandle) const {
    std::map<uint32,WorldObject>::const_iterator it = m_kObjects.find(uiHandle);
    return (it == m_kObjects.end()) ? NULL : &it->second;
}

// ── Private ───────────────────────────────────────────────────────────────────

void WorldObjectMgr::SpawnObject(WorldObject& rObj) {
    if (!m_pkScene) return;

    // Load body SGA -- player uses gender-M default, mob/NPC uses InxName
    CharacterAssets assets;
    if (rObj.eType == WOT_PLAYER) {
        assets = CharacterLoader::Get().LoadPlayer(
            rObj.uiClass, GENDER_MALE, m_pkScene);
    } else if (!rObj.kInxName.empty()) {
        assets = CharacterLoader::Get().LoadActor(rObj.kInxName, m_pkScene);
    }

    rObj.kAssets = assets;
    rObj.pkNode  = assets.pkNode;

    // Position the node
    if (rObj.pkNode) {
        rObj.pkNode->SetTranslate(NiPoint3(rObj.fX, rObj.fY, rObj.fZ));
        rObj.pkNode->Update(0.0f);
    }
}

void WorldObjectMgr::DespawnObject(uint32 uiHandle) {
    std::map<uint32,WorldObject>::iterator it = m_kObjects.find(uiHandle);
    if (it == m_kObjects.end()) return;
    if (it->second.pkNode && m_pkScene && m_pkScene->GetRoot())
        m_pkScene->GetRoot()->DetachChild(it->second.pkNode);
    m_kObjects.erase(it);
}

void WorldObjectMgr::UpdateObjectPosition(WorldObject& rObj, float fX, float fY, float fZ) {
    rObj.fX = fX; rObj.fY = fY; rObj.fZ = fZ;
    if (rObj.pkNode) {
        rObj.pkNode->SetTranslate(NiPoint3(fX, fY, fZ));
        rObj.pkNode->Update(0.0f);
    }
}

} // namespace shine
