// Server/Zone/Link.cpp
// FEATURE: portals
// Three portal types in NA2016, all routed through this module:
//   1) Map portal       -- physical gate object placed at a MapWayPoint;
//                          contact-trigger teleports to the linked map.
//                          MapLinkPoint.shn is the (from, to) graph and
//                          MapWayPoint.shn is the placement table.
//   2) Karen portal     -- mechanically identical to (1); the visual
//                          model is owned by the client. Same path.
//   3) Town portal      -- item-use scroll. Server sends the list rows
//                          from TownPortal.shn that match the scroll's
//                          group, then waits for NC_MAP_TOWNPORTAL_REQ
//                          carrying the chosen row index. See
//                          TownPortalSystem.cpp.
//
// SHN columns consumed (all NA2016, see docs/spec_pack/data_dictionary):
//   MapLinkPoint  -- MLP_FromID, MLP_ToID, MLP_Weight, MLP_OneWay_Street
//   MapWayPoint   -- MapID, X, Y, MWP_Gate
//
// MWP_Gate is the gate's placement key. The runtime joins MapWayPoint
// rows on (MapID, MWP_Gate) -> the LinkPoint that owns the destination
// pulled out of MapLinkPoint via the matching MLP_FromID/MLP_ToID pair.
#include "Link.h"
#include "MapField.h"
#include "GroupTables.h"
#include "../DataReader/ShnRegistry.h"
#include "../Shared/ShineLogSystem.h"

namespace fiesta {

Link& Link::Get() { static Link s; return s; }

bool Link::Load() {
    m_kLinks.clear();
    m_kEdges.clear();

    // 1) Build the from->to graph from MapLinkPoint.
    // FEATURE: portals -- column read: MLP_FromID, MLP_ToID,
    // MLP_Weight, MLP_OneWay_Street
    const ShnFile* tEdge = ShnRegistry::Get().GetTable("MapLinkPoint");
    if (tEdge) {
        for (size_t i = 0; i < tEdge->Rows().size(); ++i) {
            Edge e;
            e.uiFromID = (uint16)ShnGetU32(*tEdge, i, "MLP_FromID");
            e.uiToID   = (uint16)ShnGetU32(*tEdge, i, "MLP_ToID");
            e.uiWeight = (uint16)ShnGetU32(*tEdge, i, "MLP_Weight");
            e.bOneWay  = (uint8) ShnGetU32(*tEdge, i, "MLP_OneWay_Street");
            if (e.uiFromID == 0 && e.uiToID == 0) continue;
            m_kEdges.push_back(e);
        }
    }

    // 2) Build the placement table from MapWayPoint. Each row is a
    //    physical gate on a map. The MWP_Gate value joins back into
    //    MapLinkPoint via the from/to pair: a gate on map A with
    //    MWP_Gate=N teleports to the matching MWP_Gate=N entry on the
    //    destination map.
    // FEATURE: portals -- column read: MapID, X, Y, MWP_Gate
    const ShnFile* tWay = ShnRegistry::Get().GetTable("MapWayPoint");
    if (tWay) {
        for (size_t i = 0; i < tWay->Rows().size(); ++i) {
            Gate g;
            g.uiMapID  = (uint16)ShnGetU32(*tWay, i, "MapID");
            g.fX       = (float) ShnGetU32(*tWay, i, "X");
            g.fY       = (float) ShnGetU32(*tWay, i, "Y");
            g.uiGateID = (uint8) ShnGetU32(*tWay, i, "MWP_Gate");
            // Default contact radius. NA2016 ships no per-gate radius
            // so we use a fixed cell cluster (~3 cells = ~48 world units).
            // Authoritative value should be tuned per-map later.
            g.fRadius  = 48.0f;
            m_kGates.push_back(g);
        }
    }

    // 3) Resolve each gate to its destination by joining (MapID, GateID)
    //    pairs across the gate list. The convention: a gate on map A
    //    with gate-id N teleports to the gate on the linked map whose
    //    gate-id is also N. Edge direction is honored (one-way streets
    //    only join in one direction).
    for (size_t i = 0; i < m_kGates.size(); ++i) {
        const Gate& g = m_kGates[i];
        // Find an edge that originates from g's map.
        for (size_t e = 0; e < m_kEdges.size(); ++e) {
            const Edge& edge = m_kEdges[e];
            if (edge.uiFromID != g.uiMapID) continue;
            // Find a matching gate on the destination side.
            for (size_t j = 0; j < m_kGates.size(); ++j) {
                if (i == j) continue;
                const Gate& d = m_kGates[j];
                if (d.uiMapID  != edge.uiToID)  continue;
                if (d.uiGateID != g.uiGateID)   continue;
                LinkPoint p;
                p.uiSrcMap = g.uiMapID;
                p.fSx      = g.fX;
                p.fSy      = g.fY;
                p.fR       = g.fRadius;
                p.uiDstMap = d.uiMapID;
                p.fDx      = d.fX;
                p.fDy      = d.fY;
                p.bOneWay  = edge.bOneWay;
                m_kLinks.push_back(p);
            }
        }
    }

    SHINELOG_INFO("Link/portal: %u edges, %u gates, %u resolved links",
                  (uint32)m_kEdges.size(),
                  (uint32)m_kGates.size(),
                  (uint32)m_kLinks.size());
    return !m_kLinks.empty();
}

const LinkPoint* Link::Lookup(uint16 uiMap, float x, float y) const {
    for (size_t i = 0; i < m_kLinks.size(); ++i) {
        const LinkPoint& l = m_kLinks[i];
        if (l.uiSrcMap != uiMap) continue;
        float dx = x - l.fSx, dy = y - l.fSy;
        if (dx*dx + dy*dy <= l.fR*l.fR) return &m_kLinks[i];
    }
    return NULL;
}

bool Link::OnPlayerStep(ShinePlayer* pk, uint16 uiMap, float x, float y) const {
    const LinkPoint* p = Lookup(uiMap, x, y);
    if (!p) return false;
    if (!pk) return false;
    Vec3 dst((float)p->fDx, (float)p->fDy, 0.0f);
    TownPortal(pk, (MapID)p->uiDstMap, dst);
    return true;
}

} // namespace fiesta
