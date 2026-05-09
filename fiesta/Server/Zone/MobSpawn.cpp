// Server/Zone/MobSpawn.cpp
#include "MobSpawn.h"
#include "GroupTables.h"
#include "../Shared/ShineLogSystem.h"

namespace fiesta {

ShineMob* SpawnMob(MobID uiSpecies, MapID uiMap, const Vec3& rPos) {
    const MobInfoRow* pkInfo = MobTables::Get().FindMob((uint32)uiSpecies);
    if (!pkInfo) {
        SHINELOG_WARN("SpawnMob: no MobInfo row for species=%u", (uint32)uiSpecies);
        return NULL;
    }
    ShineMob* pk = new ShineMob();
    pk->m_uiSpecies = uiSpecies;
    pk->m_uiLevel   = (uint16)pkInfo->uiLevel;
    pk->SetMap(uiMap);
    pk->SetPos(rPos);
    int32 hp = (int32)pkInfo->uiMaxHP;
    if (hp <= 0) hp = 1;
    pk->SetMaxHP(hp);
    pk->SetHP(hp);
    return pk;
}

} // namespace fiesta
