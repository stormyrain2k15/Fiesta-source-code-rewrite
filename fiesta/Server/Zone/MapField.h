// Server/Zone/MapField.h
// 10 -- map / field / layer / collision / portal management.
// EVIDENCE: PDB_CONFIRMED  symbol: Field, Layer, MapDataBox, MapBlockInformation,
//                                  BlockImage, MapNavigator, FindWay, Link, TownPortal
#ifndef FIESTA_ZONE_MAPFIELD_H
#define FIESTA_ZONE_MAPFIELD_H
#include "ShineObject.h"
#include <map>
#include <vector>

namespace fiesta {

struct MapBlockInformation {
    uint16 uiW, uiH;
    std::vector<uint8> kBlocked; // 1 byte per cell, 1 = blocked
    bool IsBlocked(uint16 x, uint16 y) const {
        if (x >= uiW || y >= uiH) return true;
        size_t i = (size_t)y * uiW + x;
        return i < kBlocked.size() && kBlocked[i] != 0;
    }
};

class Field {
public:
    explicit Field(MapID id);
    MapID  Id() const { return m_uiId; }
    void   AddObject(ShineObject* p);
    void   RemoveObject(ShineObject* p);
    const std::vector<ShineObject*>& Objects() const { return m_kObjects; }
    MapBlockInformation& Blocks() { return m_kBlocks; }
private:
    MapID  m_uiId;
    std::vector<ShineObject*> m_kObjects;
    MapBlockInformation       m_kBlocks;
};

class Layer { public: uint32 uiId; }; // viewing layer id (multi-channel maps)

class MapDataBox {
public:
    static MapDataBox& Get();
    Field* GetField(MapID id);
    void   Shutdown();
private:
    std::map<MapID, Field*> m_kFields;
};

class MapNavigator {
public:
    // Provisional path approximation: straight-line sampling + block test.
    // EV_VERIFY -- replace with FindWay() when MapBlockInformation/BlockImage are pinned.
    static bool FindWay(Field& rField, const Vec3& a, const Vec3& b, std::vector<Vec3>& rPath);
};

void TownPortal(ShinePlayer* pk, MapID dest, const Vec3& kSpawn);

} // namespace fiesta
#endif
