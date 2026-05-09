// Server/Zone/CharMapMarking.cpp
// Per-character minimap marker set. Markers persist via CharDB.
#include "../Shared/ShineTypes.h"
#include <map>
#include <vector>
namespace fiesta {
struct MapMark { uint16 uiMap; float x, y; uint8 uiKind; std::string kLabel; };
class CharMapMarking {
public:
    static CharMapMarking& Get() { static CharMapMarking s; return s; }
    void Add (uint32 cid, const MapMark& m) { m_kPer[cid].push_back(m); }
    void Drop(uint32 cid) { m_kPer.erase(cid); }
private:
    std::map<uint32, std::vector<MapMark> > m_kPer;
};
} // namespace fiesta
