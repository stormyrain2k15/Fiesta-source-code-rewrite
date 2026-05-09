// Server/Zone/MapField.cpp
#include "MapField.h"
#include <math.h>

namespace fiesta {

Field::Field(MapID id) : m_uiId(id) { m_kBlocks.uiW = 0; m_kBlocks.uiH = 0; }

void Field::AddObject(ShineObject* p)    { if (p) m_kObjects.push_back(p); }
void Field::RemoveObject(ShineObject* p) {
    for (size_t i = 0; i < m_kObjects.size(); ++i)
        if (m_kObjects[i] == p) { m_kObjects.erase(m_kObjects.begin() + i); return; }
}

MapDataBox& MapDataBox::Get() { static MapDataBox s; return s; }

Field* MapDataBox::GetField(MapID id) {
    std::map<MapID, Field*>::iterator it = m_kFields.find(id);
    if (it != m_kFields.end()) return it->second;
    Field* p = new Field(id);
    m_kFields[id] = p;
    return p;
}

void MapDataBox::Shutdown() {
    for (std::map<MapID, Field*>::iterator it = m_kFields.begin(); it != m_kFields.end(); ++it) delete it->second;
    m_kFields.clear();
}

bool MapNavigator::FindWay(Field& rField, const Vec3& a, const Vec3& b, std::vector<Vec3>& rPath) {
    rPath.clear();
    // Sample at one-cell granularity so we never step over a single
    // blocked cell. Cap the iteration count so a runaway segment over a
    // continent-sized map can't loop forever.
    float dx = b.x - a.x, dy = b.y - a.y;
    float dist = (float)sqrt((double)(dx * dx + dy * dy));
    int32 steps = (int32)(dist / (float)kMapBlockCellSize);
    if (steps < 1)    steps = 1;
    if (steps > 4096) steps = 4096;
    for (int i = 0; i <= steps; ++i) {
        float t = (float)i / (float)steps;
        Vec3 p((1.0f - t) * a.x + t * b.x,
               (1.0f - t) * a.y + t * b.y,
               (1.0f - t) * a.z + t * b.z);
        if (rField.Blocks().IsBlockedWorld(p.x, p.y)) return false;
        rPath.push_back(p);
    }
    return true;
}

void TownPortal(ShinePlayer* pk, MapID dest, const Vec3& kSpawn) {
    if (!pk) return;
    Field* pkOld = MapDataBox::Get().GetField(pk->GetMap());
    Field* pkNew = MapDataBox::Get().GetField(dest);
    if (pkOld) pkOld->RemoveObject(pk);
    pk->SetMap(dest); pk->SetPos(kSpawn);
    if (pkNew) pkNew->AddObject(pk);
}

} // namespace fiesta
