// Server/Zone/Link.h
// FEATURE: portals -- runtime portal data + walk-into trigger.
// See Link.cpp for the full flow description.
#ifndef SHINE_ZONE_LINK_H
#define SHINE_ZONE_LINK_H
#include "../Shared/ShineTypes.h"
#include <vector>

namespace shine {

class ShinePlayer;

// One side of a resolved gate pair. Source position is on uiSrcMap;
// stepping inside fR teleports the player to (fDx, fDy) on uiDstMap.
struct LinkPoint {
    uint16 uiSrcMap;
    float  fSx, fSy, fR;
    uint16 uiDstMap;
    float  fDx, fDy;
    uint8  bOneWay;        // 1 = no return on this same edge
};

// Raw MapLinkPoint row -- the (from-map, to-map, weight) graph edge.
struct Edge {
    uint16 uiFromID;
    uint16 uiToID;
    uint16 uiWeight;       // pathing weight (used by NPC traversal)
    uint8  bOneWay;
};

// Raw MapWayPoint row -- placement of a single gate on a map.
struct Gate {
    uint16 uiMapID;
    float  fX, fY;
    uint8  uiGateID;
    float  fRadius;
};

class Link {
public:
    static Link& Get();
    bool Load();
    const LinkPoint* Lookup(uint16 uiMap, float x, float y) const;

    // Called from MoveManager after every successful step. Teleports
    // the player and returns true when the step entered a gate trigger.
    bool OnPlayerStep(ShinePlayer* pk, uint16 uiMap, float x, float y) const;

    // Diagnostics.
    size_t EdgeCount() const  { return m_kEdges.size(); }
    size_t GateCount() const  { return m_kGates.size(); }
    size_t LinkCount() const  { return m_kLinks.size(); }

private:
    Link() {}
    std::vector<LinkPoint> m_kLinks;
    std::vector<Edge>      m_kEdges;
    std::vector<Gate>      m_kGates;
};

} // namespace shine
#endif
