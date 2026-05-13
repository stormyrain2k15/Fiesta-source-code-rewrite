// Server/Zone/BoothManager.h
// Per-zone player-booth registry. Booths are player-run vending stalls;
// the manager tracks every open booth so:
//
//   * proximity NPC scans (used by talk-NPC-to-buy paths) can list
//     nearby booths
//   * the global booth-search UI can return paginated results
//   * a player who DCs while their booth is open has it auto-closed
//
// Booth pricing / item listing lives on the per-booth record; the
// manager is purely a directory.
#ifndef SHINE_ZONE_BOOTHMANAGER_H
#define SHINE_ZONE_BOOTHMANAGER_H
#include "Inventory.h"
#include "../Shared/ShineTypes.h"
#include <map>
#include <vector>
#include <string>

namespace shine {

class ShinePlayer;

struct BoothListing {
    uint32     uiItemKey;        // tCharItem.uiKey of the listed item
    uint32     uiQty;
    int64      iAskingGold;
};

struct BoothRecord {
    CharID                    uiOwner;
    MapID                     uiMap;
    float                     fX, fY;
    std::string               kTitle;
    std::vector<BoothListing> kListings;
    uint64                    uiOpenedMs;
};

class BoothManager {
public:
    static BoothManager& Get();
    bool   Open    (ShinePlayer* pkOwner, const std::string& rTitle);
    void   Close   (CharID cid);
    bool   AddItem (CharID cid, const BoothListing& rL);
    bool   RemoveItem(CharID cid, uint32 uiItemKey);
    // Buy uiQty of the listed (booth, itemKey) tuple. Returns the price
    // actually paid (0 means the buy didn't go through).
    int64  Buy     (CharID buyerCid, CharID sellerCid, uint32 uiItemKey,
                    uint32 uiQty, ShinePlayer* pkBuyer);
    const  BoothRecord* Find(CharID cid) const;
    void   ListNearby(MapID uiMap, float fX, float fY, float fRadius,
                      std::vector<const BoothRecord*>& rOut) const;
    size_t OpenCount() const { return m_kOpen.size(); }
private:
    BoothManager() {}
    std::map<CharID, BoothRecord> m_kOpen;
};

} // namespace shine
#endif
