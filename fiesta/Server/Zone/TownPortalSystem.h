// Server/Zone/TownPortalSystem.h
// FEATURE: portals -- town-portal scroll flow (item-use teleport UI).
//
// Ingest: TownPortal.shn columns Index, MinLevel, TP_GroupNo, MapName, X, Y.
// One row per (group, index) destination. Multiple rows share the same
// TP_GroupNo to form one scroll's destination list.
//
// Flow:
//   1) Player uses a town-portal scroll item.
//   2) Server resolves item -> TP_GroupNo (mapping table here, not data:
//      NA2016 hard-codes the scroll-tier -> group association in client
//      strings; the server mirrors the canonical 0/1/2 tier mapping).
//   3) Server filters TownPortal rows for that group AND MinLevel <=
//      player level, and sends NC_MAP_TOWNPORTAL_LIST_ACK with the rows.
//   4) Client opens picker UI; player chooses Index N.
//   5) Client sends NC_MAP_TOWNPORTAL_REQ with N.
//   6) Server validates (still in inventory, still on same map, still
//      under same group+level), consumes the scroll, teleports.
#ifndef FIESTA_ZONE_TOWNPORTALSYSTEM_H
#define FIESTA_ZONE_TOWNPORTALSYSTEM_H
#include "../Shared/ShineTypes.h"
#include <string>
#include <vector>

namespace fiesta {
class ShinePlayer;

struct TownPortalRow {
    uint8       uiIndex;        // unique within group, 0..N
    uint8       uiMinLevel;
    uint8       uiGroupNo;
    std::string kMapName;       // MapInxName (e.g. "RouN", "Eld")
    uint32      uiX;
    uint32      uiY;
};

class TownPortalSystem {
public:
    static TownPortalSystem& Get();
    bool   Load();              // pulls TownPortal.shn from ShnRegistry

    // Resolve an item (by inx-name or item-id) to the scroll group
    // tier. Returns -1 if the item isn't a known town-portal scroll.
    int32  ResolveItemToGroup(uint32 uiItemId,
                              const std::string& rInxName) const;

    // Build the destination list a player should see when they use a
    // group-`uiGroup` scroll. Filtered by MinLevel.
    void   BuildList(uint8 uiGroup, uint8 uiPlayerLv,
                     std::vector<TownPortalRow>& rOut) const;

    // Player has picked uiIndex inside uiGroup. Validates and teleports.
    // Returns true on success. The scroll consumption is the caller's
    // responsibility (see ZoneHandlers.cpp).
    bool   Teleport(ShinePlayer* pk, uint8 uiGroup, uint8 uiIndex) const;

    size_t Count() const { return m_kRows.size(); }

private:
    TownPortalSystem() {}
    std::vector<TownPortalRow> m_kRows;
};

} // namespace fiesta
#endif
