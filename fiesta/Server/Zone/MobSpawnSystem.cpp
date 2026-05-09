// Server/Zone/MobSpawnSystem.cpp
#include "MobSpawnSystem.h"
#include "MobSpawn.h"
#include "MapField.h"
#include "GroupTables.h"
#include "ZoneServer.h"
#include "../Shared/ShineLogSystem.h"
#include <stdlib.h>

namespace fiesta {

MobSpawnSystem& MobSpawnSystem::Get() { static MobSpawnSystem s; return s; }

size_t MobSpawnSystem::LoadAll() {
    m_kGroups.clear();
    m_kHandleToGroup.clear();
    // MobRegenBox is keyed by map name; we don't know all the keys without
    // re-walking the data folder, so we accept the union from MapTables and
    // attempt a Find against each.
    const std::vector<MapInfoRow>& maps = MapTables::Get().Maps();
    size_t built = 0;
    for (size_t mi = 0; mi < maps.size(); ++mi) {
        const MapInfoRow& m = maps[mi];
        const MobRegenTable* t = MobRegenBox::Get().Find(m.kMapName);
        if (!t) continue;
        // Build a quick group-index -> group descriptor map.
        std::map<std::string, const MobRegenGroupRow*> g;
        for (size_t i = 0; i < t->Groups().size(); ++i) {
            const MobRegenGroupRow& r = t->Groups()[i];
            g[r.kGroupIndex] = &r;
        }
        // Each MobRegen row contributes one SpawnGroup (a row references a
        // group center via kRegenIndex).
        for (size_t i = 0; i < t->Regens().size(); ++i) {
            const MobRegenRow& r = t->Regens()[i];
            std::map<std::string, const MobRegenGroupRow*>::const_iterator git
                = g.find(r.kRegenIndex);
            if (git == g.end())                     continue;
            const MobInfoRow* pkInfo =
                MobTables::Get().FindByInx(r.kMobIndex);
            if (!pkInfo)                            continue;
            MobSpawnGroup sg;
            sg.kGroupIndex = git->second->kGroupIndex;
            sg.kMobIndex   = r.kMobIndex;
            sg.uiSpecies   = (MobID)pkInfo->uiID;
            sg.uiMap       = (MapID)m.uiID;
            sg.iCenterX    = (int32)git->second->uiCenterX;
            sg.iCenterY    = (int32)git->second->uiCenterY;
            sg.iHalfW      = (int32)(git->second->uiWidth  / 2);
            sg.iHalfH      = (int32)(git->second->uiHeight / 2);
            sg.uiTarget    = r.uiMobNum;
            // Respawn base; aDelta/aSec curve is applied per-mob in OnMobDied.
            sg.uiRespawnMs = r.uiRegStandard * 1000;
            sg.uiNextRollMs= 0;
            m_kGroups.push_back(sg);
            ++built;
        }
    }
    SHINELOG_INFO("MobSpawnSystem: %u groups across %u maps",
                  (uint32)built, (uint32)maps.size());
    return built;
}

void MobSpawnSystem::Tick(uint64 uiNowMs) {
    // Per-group: ensure live count >= target. Roll once per second to keep
    // CPU manageable in the face of thousands of groups.
    for (size_t gi = 0; gi < m_kGroups.size(); ++gi) {
        MobSpawnGroup& g = m_kGroups[gi];
        if (g.uiNextRollMs > uiNowMs) continue;
        g.uiNextRollMs = uiNowMs + 1000;

        Field* pkF = MapDataBox::Get().GetField(g.uiMap);
        if (!pkF) continue;
        while (g.kAlive.size() < (size_t)g.uiTarget) {
            // Random offset inside (Width,Height) about (CenterX,CenterY).
            int32 dx = g.iHalfW > 0 ? (rand() % (2 * g.iHalfW + 1)) - g.iHalfW : 0;
            int32 dy = g.iHalfH > 0 ? (rand() % (2 * g.iHalfH + 1)) - g.iHalfH : 0;
            Vec3 pos((float)(g.iCenterX + dx), (float)(g.iCenterY + dy), 0.0f);
            ShineMob* pkMob = SpawnMob(g.uiSpecies, g.uiMap, pos);
            if (!pkMob) break;
            pkMob->SetHandle(ZoneServer::Get().NewObjectHandle());
            pkF->AddObject(pkMob);
            ZoneServer::Get().RegisterObject(pkMob);   // unified handle map
            g.kAlive.push_back(pkMob->GetHandle());
            m_kHandleToGroup[pkMob->GetHandle()] = gi;
        }
    }
}

void MobSpawnSystem::OnMobDied(Handle uiMob) {
    std::map<Handle, size_t>::iterator it = m_kHandleToGroup.find(uiMob);
    if (it == m_kHandleToGroup.end()) return;
    MobSpawnGroup& g = m_kGroups[it->second];
    for (size_t i = 0; i < g.kAlive.size(); ++i) {
        if (g.kAlive[i] == uiMob) {
            g.kAlive.erase(g.kAlive.begin() + i);
            break;
        }
    }
    m_kHandleToGroup.erase(it);
    // Drop the dead mob from the unified registry; the field-side
    // remove + dtor are handled by Battle::Kill -> SpawnMob inverse,
    // which is the canonical owner of the ShineMob* allocation.
    ShineObject* pk = ZoneServer::Get().FindObject(uiMob);
    if (pk) ZoneServer::Get().UnregisterObject(pk);
    // Top-up will pick up the slot on the next Tick once
    // `uiNowMs >= uiNextRollMs + uiRespawnMs`. For simplicity we enforce
    // the gate by pushing the group's next-roll to "now + respawn".
    g.uiNextRollMs += g.uiRespawnMs;
}

} // namespace fiesta
