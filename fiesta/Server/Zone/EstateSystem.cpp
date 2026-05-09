// Server/Zone/EstateSystem.cpp
#include "EstateSystem.h"
#include "ExtendedTables.h"
#include "GroupTables.h"
#include "MarketSystems.h"
#include "MapField.h"
#include "ZoneServer.h"
#include "AbState.h"
#include "CharDBClient.h"
#include "../Shared/ShineLogSystem.h"
#include <string.h>

namespace fiesta {

// Furniture aura is refreshed every minute (the UI updates buff icons no
// faster than that and the AbState has its own keep-time, so we don't need
// per-frame refresh). Keep-time is 90s -> tolerates one missed tick.
static const uint32 ESTATE_AURA_KEEP_MS    = 90000;
static const uint32 ESTATE_AURA_TICK_MS    = 60000;
static const uint64 ESTATE_HOUR_MS         = 3600000ULL;


EstateServer& EstateServer::Get() { static EstateServer s; return s; }

EstateRec* EstateServer::Create(CharID uiOwner, uint32 uiHouseID, uint32 uiTier) {
    if (m_kOwnerIdx.find(uiOwner) != m_kOwnerIdx.end()) {
        SHINELOG_WARN("Estate: owner %u already has an estate", uiOwner);
        return NULL;
    }
    if (!MiniHouseTables::Get().Find(uiHouseID)) {
        SHINELOG_WARN("Estate: HouseID %u not in MiniHouse.shn", uiHouseID);
        return NULL;
    }
    EstateRec rec;
    rec.uiEstateID    = m_uiNext++;
    rec.uiOwner       = uiOwner;
    rec.uiHouseID     = uiHouseID;
    rec.uiTier        = uiTier;
    const EstateExtraTables::EndureRow* end =
        EstateExtraTables::Get().FindEndure(uiTier);
    rec.uiEndure      = (uint16)(end ? end->uiMaxEndure : 1000);
    rec.uiNextDecayMs = 0;
    rec.bVendorOpen   = false;
    m_kAll[rec.uiEstateID]   = rec;
    m_kOwnerIdx[uiOwner]     = rec.uiEstateID;
    SHINELOG_INFO("Estate %u created for char %u (house %u tier %u, endure %u)",
                  rec.uiEstateID, uiOwner, uiHouseID, uiTier, rec.uiEndure);
    CharDBClient::Get().EstateCreate(uiOwner, uiHouseID, uiTier);
    return &m_kAll[rec.uiEstateID];
}

EstateRec* EstateServer::Find(uint32 uiEstateID) {
    std::map<uint32, EstateRec>::iterator it = m_kAll.find(uiEstateID);
    return (it == m_kAll.end()) ? NULL : &it->second;
}

EstateRec* EstateServer::FindByOwner(CharID uiOwner) {
    std::map<CharID, uint32>::iterator it = m_kOwnerIdx.find(uiOwner);
    if (it == m_kOwnerIdx.end()) return NULL;
    return Find(it->second);
}

void EstateServer::Demolish(uint32 uiEstateID, CharID uiRequester) {
    EstateRec* pkE = Find(uiEstateID);
    if (!pkE || pkE->uiOwner != uiRequester) return;
    CharDBClient::Get().EstateDemolish(pkE->uiOwner);
    m_kOwnerIdx.erase(pkE->uiOwner);
    m_kAll.erase(uiEstateID);
    SHINELOG_INFO("Estate %u demolished by char %u", uiEstateID, uiRequester);
}

namespace {
// Serialize a placement vector into a flat blob for persistence. Layout:
//   uint32 count
//   { uint32 slot, uint32 furn, int32 X,Y,Z, uint16 yaw, uint16 endure } * count
void SerializePlacements(const std::vector<EstatePlacement>& rIn,
                         std::vector<uint8>& rOut) {
    rOut.clear();
    uint32 cnt = (uint32)rIn.size();
    rOut.resize(4 + cnt * 24);
    uint8* p = rOut.empty() ? NULL : &rOut[0];
    if (!p) return;
    memcpy(p, &cnt, 4); p += 4;
    for (size_t i = 0; i < rIn.size(); ++i) {
        const EstatePlacement& e = rIn[i];
        memcpy(p, &e.uiSlotID, 4);  p += 4;
        memcpy(p, &e.uiFurnID, 4);  p += 4;
        memcpy(p, &e.iX,       4);  p += 4;
        memcpy(p, &e.iY,       4);  p += 4;
        memcpy(p, &e.iZ,       4);  p += 4;
        memcpy(p, &e.uiYawDeg, 2);  p += 2;
        memcpy(p, &e.uiEndure, 2);  p += 2;
    }
}
} // namespace

bool EstateServer::Place(CharID uiOwner, uint32 uiFurnID, uint32 uiSlotID,
                         int32 iX, int32 iY, int32 iZ, uint16 uiYawDeg) {
    EstateRec* pkE = FindByOwner(uiOwner); if (!pkE) return false;
    if (!MiniHouseTables::Get().FindFurn(uiFurnID)) return false;
    // Validate slot exists per MiniHouseDummy.
    std::vector<const EstateExtraTables::DummyRow*> dummies;
    EstateExtraTables::Get().DummiesFor(pkE->uiHouseID, dummies);
    bool slotOk = dummies.empty();   // legacy houses have no dummy rows
    for (size_t i = 0; i < dummies.size(); ++i)
        if (dummies[i]->uiSlotID == uiSlotID) { slotOk = true; break; }
    if (!slotOk) return false;
    // Reject if slot already occupied.
    for (size_t i = 0; i < pkE->kPlacements.size(); ++i)
        if (pkE->kPlacements[i].uiSlotID == uiSlotID) return false;

    EstatePlacement p;
    p.uiSlotID  = uiSlotID;
    p.uiFurnID  = uiFurnID;
    p.iX = iX; p.iY = iY; p.iZ = iZ;
    p.uiYawDeg  = uiYawDeg;
    p.uiEndure  = 1000;
    p.uiPlacedMs= 0;
    pkE->kPlacements.push_back(p);
    // Persist the new layout.
    std::vector<uint8> blob;
    SerializePlacements(pkE->kPlacements, blob);
    CharDBClient::Get().EstateSave(uiOwner, blob.empty() ? NULL : &blob[0], blob.size());
    return true;
}

bool EstateServer::PickUp(CharID uiOwner, uint32 uiSlotID) {
    EstateRec* pkE = FindByOwner(uiOwner); if (!pkE) return false;
    for (size_t i = 0; i < pkE->kPlacements.size(); ++i) {
        if (pkE->kPlacements[i].uiSlotID == uiSlotID) {
            pkE->kPlacements.erase(pkE->kPlacements.begin() + i);
            std::vector<uint8> blob;
            SerializePlacements(pkE->kPlacements, blob);
            CharDBClient::Get().EstateSave(uiOwner,
                blob.empty() ? NULL : &blob[0], blob.size());
            return true;
        }
    }
    return false;
}

bool EstateServer::PayMaintenance(CharID uiOwner, int64 iGoldPaid) {
    EstateRec* pkE = FindByOwner(uiOwner); if (!pkE) return false;
    const EstateExtraTables::EndureRow* end =
        EstateExtraTables::Get().FindEndure(pkE->uiTier);
    uint16 cap = (uint16)(end ? end->uiMaxEndure : 1000);
    // Each gold restores 1 endure point up to the tier max. (Caller is
    // responsible for actually deducting the gold from inventory.)
    int64 add = iGoldPaid; if (add < 0) add = 0;
    if (add > (int64)(cap - pkE->uiEndure)) add = cap - pkE->uiEndure;
    pkE->uiEndure = (uint16)(pkE->uiEndure + add);
    return true;
}

bool EstateServer::VendorOpen(CharID uiOwner, const std::string& rTitle) {
    EstateRec* pkE = FindByOwner(uiOwner); if (!pkE) return false;
    pkE->bVendorOpen  = true;
    pkE->kVendorTitle = rTitle;
    StreetBooth::Open(uiOwner, rTitle);
    return true;
}
bool EstateServer::VendorClose(CharID uiOwner) {
    EstateRec* pkE = FindByOwner(uiOwner); if (!pkE) return false;
    pkE->bVendorOpen  = false;
    StreetBooth::Close(uiOwner);
    return true;
}

void EstateServer::Tick(uint64 uiNowMs) {
    // Two cadences:
    //   * Hourly endure decay (per MiniHouseEndure[Tier]).
    //   * Per-minute furniture aura refresh: every placed furniture row
    //     with a MiniHouseFurnitureObjEffect entry pushes its abstate to
    //     the owner and to every ShinePlayer within Range on the same map
    //     (Range is in cells; resolved against MiniHouseDummy slot bone).
    std::map<uint32, EstateRec>::iterator it;
    for (it = m_kAll.begin(); it != m_kAll.end(); ++it) {
        EstateRec& e = it->second;

        // ---- hourly endure decay ----
        if (e.uiNextDecayMs == 0) e.uiNextDecayMs = uiNowMs + ESTATE_HOUR_MS;
        if (uiNowMs >= e.uiNextDecayMs) {
            const EstateExtraTables::EndureRow* end =
                EstateExtraTables::Get().FindEndure(e.uiTier);
            uint32 dec = end ? end->uiHourlyDecay : 0;
            if (e.uiEndure <= dec) e.uiEndure = 0;
            else                   e.uiEndure = (uint16)(e.uiEndure - dec);
            e.uiNextDecayMs = uiNowMs + ESTATE_HOUR_MS;
            if (e.uiEndure == 0 && e.bVendorOpen) VendorClose(e.uiOwner);
        }

        // ---- per-minute furniture aura ----
        // The estate is "live" for buff purposes only when the owner is
        // online and the endure is non-zero.
        if (e.uiEndure == 0) continue;
        ShinePlayer* pkOwner = ZoneServer::Get().FindPlayerByCharID(e.uiOwner);
        if (!pkOwner) continue;

        for (size_t i = 0; i < e.kPlacements.size(); ++i) {
            const EstatePlacement& p = e.kPlacements[i];
            const EstateExtraTables::FurnEffRow* fe =
                EstateExtraTables::Get().FindFurnEff(p.uiFurnID);
            if (!fe || !fe->uiAbStateID) continue;

            // Owner always receives the aura (they're inside their own
            // estate by definition while logged in via the estate portal).
            pkOwner->AbState().Apply(fe->uiAbStateID, ESTATE_AURA_KEEP_MS);

            // Visitor scan: same map, within Range cells of (X,Y,Z).
            Field* pkF = MapDataBox::Get().GetField(pkOwner->GetMap());
            if (!pkF || fe->uiRange == 0) continue;
            const std::vector<ShineObject*>& objs = pkF->Objects();
            float dx, dy, dz, r2 = (float)fe->uiRange * (float)fe->uiRange;
            for (size_t j = 0; j < objs.size(); ++j) {
                if (objs[j] == pkOwner) continue;
                if (objs[j]->GetType() != OT_PLAYER) continue;
                ShinePlayer* pkV = (ShinePlayer*)objs[j];
                dx = pkV->GetPos().x - (float)p.iX;
                dy = pkV->GetPos().y - (float)p.iY;
                dz = pkV->GetPos().z - (float)p.iZ;
                if (dx*dx + dy*dy + dz*dz <= r2)
                    pkV->AbState().Apply(fe->uiAbStateID, ESTATE_AURA_KEEP_MS);
            }
        }
    }
}

} // namespace fiesta
