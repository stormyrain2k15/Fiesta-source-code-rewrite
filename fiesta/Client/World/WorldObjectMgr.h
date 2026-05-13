// Client/World/WorldObjectMgr.h
// Manages world objects visible to the local player:
// other players, mobs, NPCs. Handles spawn/despawn/move packets.
//
// NC packets handled:
//   NC_CHAR_BRIEFINFO_LOGINCHARACTER_CMD -- another player entered range
//   NC_CHAR_CLIENT_BASE_CMD              -- mob/NPC spawn data
//   NC_CHAR_DISAPPEAR_CMD                -- object left range or died
//   NC_ACT_MOVERUN_CMD / MOVEWALK_CMD    -- object movement update
//   NC_CHAR_STATUS_CHANGE_CMD            -- object HP change
#ifndef SHINE_CLIENT_WORLD_WORLDOBJECTMGR_H
#define SHINE_CLIENT_WORLD_WORLDOBJECTMGR_H

#include <NiMain.h>
#include "../../Server/Shared/ShineTypes.h"
#include "../../Server/Shared/GPacket.h"
#include "../Engine/ShineScene.h"
#include "../ResSystem/CharacterLoader.h"
#include <map>
#include <string>

namespace shine {

enum WorldObjType { WOT_PLAYER, WOT_MOB, WOT_NPC };

struct WorldObject {
    uint32          uiHandle;
    WorldObjType    eType;
    std::string     kName;
    std::string     kInxName;   // for mob/NPC action dat lookup
    uint16          uiClass;
    uint16          uiLevel;
    int32           iHP;
    int32           iMaxHP;
    float           fX, fY, fZ;
    float           fFacing;
    NiAVObject*     pkNode;     // scene node (NULL until loaded)
    CharacterAssets kAssets;

    WorldObject()
        : uiHandle(0), eType(WOT_MOB), uiClass(0), uiLevel(1),
          iHP(100), iMaxHP(100), fX(0), fY(0), fZ(0), fFacing(0),
          pkNode(NULL) {}
};

class WorldObjectMgr {
public:
    WorldObjectMgr();

    void Init(ShineScene* pkScene);
    void Shutdown();
    void Tick(float fDt);

    // Packet handlers -- called by GameFrameWork::OnGamePacket
    void OnBriefInfoLogin   (const GPacket& rPkt); // other player enters range
    void OnCharDisappear    (const GPacket& rPkt); // object leaves range
    void OnObjMoveRun       (const GPacket& rPkt);
    void OnObjMoveWalk      (const GPacket& rPkt);
    void OnObjStop          (const GPacket& rPkt);
    void OnObjStatusChange  (const GPacket& rPkt);

    const WorldObject* FindByHandle(uint32 uiHandle) const;
    uint32 Count() const { return (uint32)m_kObjects.size(); }

private:
    ShineScene* m_pkScene;
    std::map<uint32, WorldObject> m_kObjects;

    void SpawnObject(WorldObject& rObj);
    void DespawnObject(uint32 uiHandle);
    void UpdateObjectPosition(WorldObject& rObj, float fX, float fY, float fZ);
};

} // namespace shine
#endif // SHINE_CLIENT_WORLD_WORLDOBJECTMGR_H
