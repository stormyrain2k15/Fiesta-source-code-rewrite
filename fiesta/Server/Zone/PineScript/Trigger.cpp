// Server/Zone/PineScript/Trigger.cpp
// PineScript trigger volumes -- spatial regions that fire an interrupt
// when a player or mob enters / leaves. Trigger definitions live in the
// per-map .ps; the runtime registry is here.
#include "PineScript.h"
#include "../../Shared/ShineTypes.h"
#include <vector>

namespace fiesta {

struct TriggerVolume { uint32 uiId; uint16 uiMap; float x1, y1, x2, y2; std::string kProgram; };

class Trigger {
public:
    static Trigger& Get() { static Trigger s; return s; }
    void Register(const TriggerVolume& v) { m_kVols.push_back(v); }
    void OnPlayerMove(uint16 uiMap, float x, float y, uint32 /*uiCharNo*/) {
        for (size_t i = 0; i < m_kVols.size(); ++i) {
            const TriggerVolume& v = m_kVols[i];
            if (v.uiMap != uiMap) continue;
            if (x < v.x1 || x > v.x2 || y < v.y1 || y > v.y2) continue;
            PineScript::Get().Run(v.kProgram);
        }
    }
private:
    std::vector<TriggerVolume> m_kVols;
};

} // namespace fiesta
