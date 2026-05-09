// Server/Zone/MapField.h
// map / field / layer / collision / portal management.
//                                  BlockImage, MapNavigator, FindWay, Link, TownPortal
#ifndef FIESTA_ZONE_MAPFIELD_H
#define FIESTA_ZONE_MAPFIELD_H
#include "ShineObject.h"
#include <map>
#include <vector>

namespace fiesta {

// World <-> cell scale. Live SHBD grids store one byte per 16-world-unit
// cell (verified against Adl.shbd: file size - 8 == width*height where
// width/height are the cell dimensions, and the in-game map width is
// roughly 16x the cell count). If a future map ships with a different
// scale we'll plumb it through MapInfo.shn; the constant here is the
// universal default.
const int32 kMapBlockCellSize = 16;

// Per-Field collision grid. One byte per cell; 0 = walkable, non-zero
// (typically 0xFF) = blocked. Origin is the south-west corner of the
// map at world (0, 0). x grows east, y grows north.
struct MapBlockInformation {
    uint16             uiW, uiH;
    std::vector<uint8> kBlocked;
    bool   IsBlockedCell(uint16 cx, uint16 cy) const {
        if (cx >= uiW || cy >= uiH) return true;
        size_t i = (size_t)cy * uiW + cx;
        return i < kBlocked.size() && kBlocked[i] != 0;
    }
    // World-coordinate test. Negatives are treated as out-of-bounds
    // (= blocked) so the path sampler returns failure rather than
    // wrapping around.
    bool   IsBlockedWorld(float x, float y) const {
        if (x < 0.0f || y < 0.0f) return true;
        uint16 cx = (uint16)((int32)(x / (float)kMapBlockCellSize));
        uint16 cy = (uint16)((int32)(y / (float)kMapBlockCellSize));
        return IsBlockedCell(cx, cy);
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
    // Straight-line waypoint sampler. Steps the segment in cell-sized
    // increments and consults MapBlockInformation::IsBlockedWorld at each
    // step. Returns false on the first blocked sample so callers can
    // fall back to a stepped re-route.
    static bool FindWay(Field& rField, const Vec3& a, const Vec3& b, std::vector<Vec3>& rPath);
};

void TownPortal(ShinePlayer* pk, MapID dest, const Vec3& kSpawn);

} // namespace fiesta
#endif
