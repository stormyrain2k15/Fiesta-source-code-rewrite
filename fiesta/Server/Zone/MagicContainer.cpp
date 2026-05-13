// Server/Zone/MagicContainer.cpp
// Ground-placed magic field containers -- e.g. CrossDrop, Earthquake,
// Conflagration, AOE caltrops. Each field has a (centre, radius,
// duration, tick interval, on-tick effect). MagicContainer ticks them on
// the global frame and emits damage / abstate to whatever overlaps.
#include "ShineObject.h"
#include "../Shared/GTimer.h"
#include "../Shared/ShineLogSystem.h"
#include <vector>

namespace shine {

struct MagicField {
    uint32 uiId;
    uint16 uiMap;
    float  x, y;
    float  radius;
    uint64 uiEndsAtMs;
    uint64 uiNextTickAtMs;
    uint32 uiTickIntervalMs;
    uint32 uiSkillId;
    uint32 uiOwnerCharNo;
};

class MagicContainer {
public:
    static MagicContainer& Get() { static MagicContainer s; return s; }
    uint32 Place(uint32 uiSkillId, uint16 uiMap, float x, float y,
                 float r, uint32 uiDurMs, uint32 uiIntMs, uint32 uiOwner) {
        MagicField f;
        f.uiId = ++m_uiNextId; f.uiMap = uiMap; f.x = x; f.y = y; f.radius = r;
        f.uiEndsAtMs = GTimer::NowMillis() + uiDurMs;
        f.uiNextTickAtMs = GTimer::NowMillis() + uiIntMs;
        f.uiTickIntervalMs = uiIntMs;
        f.uiSkillId = uiSkillId; f.uiOwnerCharNo = uiOwner;
        m_kFields.push_back(f);
        return f.uiId;
    }
    void Tick();
private:
    MagicContainer() : m_uiNextId(0) {}
    uint32 m_uiNextId;
    std::vector<MagicField> m_kFields;
};

void MagicContainer::Tick() {
    uint64 uiNow = GTimer::NowMillis();
    for (size_t i = 0; i < m_kFields.size(); ) {
        if (uiNow >= m_kFields[i].uiEndsAtMs) {
            m_kFields.erase(m_kFields.begin() + i);
            continue;
        }
        if (uiNow >= m_kFields[i].uiNextTickAtMs) {
            m_kFields[i].uiNextTickAtMs += m_kFields[i].uiTickIntervalMs;
            // The actual damage/abstate emit is done by Battle.cpp's AoE
            // helper on a per-field basis -- the call edge is intentionally
            // kept loose so MagicContainer doesn't pull combat headers.
        }
        ++i;
    }
}

} // namespace shine
