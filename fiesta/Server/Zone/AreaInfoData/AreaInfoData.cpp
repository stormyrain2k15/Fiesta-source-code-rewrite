// Server/Zone/AreaInfoData/AreaInfoData.cpp
// .aid file area definitions -- per-map list of named rectangles (safe
// zones, PvP zones, no-mount, no-dismount, gather areas). Used by
// RuleOfEngagement and the gather/craft system.
#include "../../Shared/ShineTypes.h"
#include <vector>
namespace fiesta {
struct AreaRect { uint32 uiAreaId; uint16 uiMap; float x1, y1, x2, y2; uint8 uiKind; };
class AreaInfoData {
public:
    static AreaInfoData& Get() { static AreaInfoData s; return s; }
    bool Load(uint16 /*uiMap*/, const std::string& /*rPath*/) { return true; }
    const AreaRect* AreaAt(uint16 uiMap, float x, float y) const;
private:
    std::vector<AreaRect> m_kAreas;
};
const AreaRect* AreaInfoData::AreaAt(uint16 uiMap, float x, float y) const {
    for (size_t i = 0; i < m_kAreas.size(); ++i) {
        const AreaRect& a = m_kAreas[i];
        if (a.uiMap == uiMap && x >= a.x1 && x <= a.x2 && y >= a.y1 && y <= a.y2)
            return &m_kAreas[i];
    }
    return NULL;
}
} // namespace fiesta
