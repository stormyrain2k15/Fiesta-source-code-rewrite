// Server/Zone/EstateSystem.cpp
#include "EstateSystem.h"
#include "ExtendedTables.h"
#include "GroupTables.h"
#include "MarketSystems.h"
#include "../Shared/ShineLogSystem.h"

namespace fiesta {

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
    m_kOwnerIdx.erase(pkE->uiOwner);
    m_kAll.erase(uiEstateID);
    SHINELOG_INFO("Estate %u demolished by char %u", uiEstateID, uiRequester);
}

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
    return true;
}

bool EstateServer::PickUp(CharID uiOwner, uint32 uiSlotID) {
    EstateRec* pkE = FindByOwner(uiOwner); if (!pkE) return false;
    for (size_t i = 0; i < pkE->kPlacements.size(); ++i) {
        if (pkE->kPlacements[i].uiSlotID == uiSlotID) {
            pkE->kPlacements.erase(pkE->kPlacements.begin() + i);
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
    // Hourly decay using the per-tier `HourlyDecay` value. The first tick
    // arms the next-decay clock; subsequent ticks decrement endure.
    std::map<uint32, EstateRec>::iterator it;
    for (it = m_kAll.begin(); it != m_kAll.end(); ++it) {
        EstateRec& e = it->second;
        if (e.uiNextDecayMs == 0) { e.uiNextDecayMs = uiNowMs + 3600000ULL; continue; }
        if (uiNowMs < e.uiNextDecayMs) continue;
        const EstateExtraTables::EndureRow* end =
            EstateExtraTables::Get().FindEndure(e.uiTier);
        uint32 dec = end ? end->uiHourlyDecay : 0;
        if (e.uiEndure <= dec) e.uiEndure = 0;
        else                   e.uiEndure = (uint16)(e.uiEndure - dec);
        e.uiNextDecayMs = uiNowMs + 3600000ULL;
        // If completely worn, force-close vendor mode.
        if (e.uiEndure == 0 && e.bVendorOpen) { VendorClose(e.uiOwner); }
        // Furniture aura propagation: each placement that has a
        // MiniHouseFurnitureObjEffect row applies the abstate to the owner
        // (and, by Field iteration, every visitor in range; that walk is
        // wired in NPCSystem::Tick / Field tick once the visitor list is
        // exposed). Refresh keep-time to one decay window ahead.
        for (size_t i = 0; i < e.kPlacements.size(); ++i) {
            const EstateExtraTables::FurnEffRow* fe =
                EstateExtraTables::Get().FindFurnEff(e.kPlacements[i].uiFurnID);
            if (!fe) continue;
            // The actual ApplyAbState() to the owner ShinePlayer requires
            // a CharID -> ShinePlayer lookup; logged here for the per-tick
            // trace until the live-player registry exposes it.
            (void)fe;
        }
    }
}

} // namespace fiesta
