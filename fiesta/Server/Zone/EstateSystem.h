// Server/Zone/EstateSystem.h
// Estate / MiniHouse / personal vendor (Booth).
//                                  MiniHouseEndure, MiniHouseObjAni
//                                  MiniHouseDummy.shn, MiniHouseEndure.shn,
//                                  MiniHouseFurnitureObjEffect.shn,
//                                  MiniHouseObjAni.shn
//                                  mini house/vendor system. U could set
//                                  up ur own shop and sell stuff. Or go
//                                  into ur estate and decorate ur house
//                                  with furniture..." (2026-02 user note).
// What lives here:
//   1. *Estate* -- a per-character instanced room. The character spawns
//      one when they place the housing-deed item; the engine allocates
//      a room handle (auto-incrementing) and the player can re-enter via
//      a portal NPC. Furniture placed in the room is persisted to
//      `tEstateFurniture` (CharDB).
//   2. *Furniture* -- ShineItem rows tagged with FurnID. When placed they
//      apply a `MiniHouseFurnitureObjEffect` aura and can broadcast a
//      bound `MiniHouseObjAni` animation slot.
//   3. *Durability* -- `MiniHouseEndure` table. Each estate has an Endure
//      value that decays hourly while the house is "live" and is restored
//      when the player pays a maintenance fee (via the same NPC that
//      spawned the deed).
//   4. *Vendor (Booth)* -- The estate doubles as a vendor stall. When the
//      owner toggles vendor mode, items priced via `StreetBoothSell` are
//      visible to other players who can browse the estate; this funnels
//      into the existing `StreetBooth` flow in MarketSystems.h.
#ifndef SHINE_ZONE_ESTATESYSTEM_H
#define SHINE_ZONE_ESTATESYSTEM_H
#include "Inventory.h"
#include <map>
#include <vector>
#include <string>

namespace shine {

struct EstatePlacement {
    uint32      uiSlotID;       // bone slot from MiniHouseDummy
    uint32      uiFurnID;       // -> MiniHouseFurniture row
    int32       iX, iY, iZ;
    uint16      uiYawDeg;
    uint16      uiEndure;       // local durability of *this* furniture
    uint64      uiPlacedMs;
};

struct EstateRec {
    uint32                          uiEstateID;
    CharID                          uiOwner;
    uint32                          uiHouseID;     // -> MiniHouse.shn row
    uint32                          uiTier;        // -> MiniHouseEndure tier
    uint16                          uiEndure;      // current global endure
    uint64                          uiNextDecayMs; // walltime, monotonic
    bool                            bVendorOpen;
    std::string                     kVendorTitle;
    std::vector<EstatePlacement>    kPlacements;
};

class EstateServer {
public:
    static EstateServer& Get();

    // Owner spawns / claims a house when the deed item is consumed in
    // their inventory. `uiHouseID` resolves to MiniHouse.shn -- caller
    // already validated the ItemInfo->HouseID mapping.
    EstateRec*  Create(CharID uiOwner, uint32 uiHouseID, uint32 uiTier);
    EstateRec*  Find  (uint32 uiEstateID);
    EstateRec*  FindByOwner(CharID uiOwner);
    void        Demolish(uint32 uiEstateID, CharID uiRequester);

    // Furniture place / pick up. Returns false if the slot is occupied,
    // the FurnID is not in MiniHouseFurniture, or the owner doesn't
    // match.
    bool        Place (CharID uiOwner, uint32 uiFurnID, uint32 uiSlotID,
                       int32 iX, int32 iY, int32 iZ, uint16 uiYawDeg);
    bool        PickUp(CharID uiOwner, uint32 uiSlotID);

    // Durability maintenance (player pays gold; resets endure to tier max).
    bool        PayMaintenance(CharID uiOwner, int64 iGoldPaid);

    // Vendor (booth) toggle. When opened, the estate appears in the
    // booth list under uiOwner's name and routes to StreetBoothBuy.
    bool        VendorOpen (CharID uiOwner, const std::string& rTitle);
    bool        VendorClose(CharID uiOwner);

    // Tick: decays endure for every estate whose hourly mark elapsed,
    // applies aura abstates from active furniture, ticks ObjAni loops.
    void        Tick(uint64 uiNowMs);

    size_t      Count() const { return m_kAll.size(); }
private:
    EstateServer() : m_uiNext(1) {}
    std::map<uint32, EstateRec> m_kAll;
    std::map<CharID, uint32>    m_kOwnerIdx;
    uint32                      m_uiNext;
};

} // namespace shine
#endif
