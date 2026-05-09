// Server/Zone/BoothManager.cpp
// Real implementation backed by a per-zone live record map.
#include "BoothManager.h"
#include "ShineObject.h"
#include "../Shared/GTimer.h"
#include "../Shared/ShineLogSystem.h"

namespace fiesta {

BoothManager& BoothManager::Get() { static BoothManager s; return s; }

bool BoothManager::Open(ShinePlayer* pkOwner, const std::string& rTitle) {
    if (!pkOwner) return false;
    CharID c = pkOwner->GetCharID();
    if (m_kOpen.find(c) != m_kOpen.end()) return false;       // already open
    BoothRecord r;
    r.uiOwner    = c;
    r.uiMap      = pkOwner->GetMap();
    r.fX         = pkOwner->GetPos().x;
    r.fY         = pkOwner->GetPos().z;
    r.kTitle     = rTitle;
    r.uiOpenedMs = GTimer::NowMillis();
    m_kOpen[c] = r;
    SHINELOG_INFO("Booth opened: cid=%u map=%u title='%s'",
                  c, (uint32)r.uiMap, rTitle.c_str());
    return true;
}

void BoothManager::Close(CharID c) {
    if (m_kOpen.erase(c) > 0)
        SHINELOG_INFO("Booth closed: cid=%u", c);
}

bool BoothManager::AddItem(CharID c, const BoothListing& rL) {
    std::map<CharID, BoothRecord>::iterator it = m_kOpen.find(c);
    if (it == m_kOpen.end()) return false;
    // Reject duplicate item-key listings (one item-key per booth).
    for (size_t i = 0; i < it->second.kListings.size(); ++i)
        if (it->second.kListings[i].uiItemKey == rL.uiItemKey) return false;
    it->second.kListings.push_back(rL);
    return true;
}

bool BoothManager::RemoveItem(CharID c, uint32 uiItemKey) {
    std::map<CharID, BoothRecord>::iterator it = m_kOpen.find(c);
    if (it == m_kOpen.end()) return false;
    for (size_t i = 0; i < it->second.kListings.size(); ++i) {
        if (it->second.kListings[i].uiItemKey == uiItemKey) {
            it->second.kListings.erase(it->second.kListings.begin() + i);
            return true;
        }
    }
    return false;
}

int64 BoothManager::Buy(CharID buyerCid, CharID sellerCid, uint32 uiItemKey,
                        uint32 uiQty, ShinePlayer* pkBuyer) {
    if (!pkBuyer || buyerCid == sellerCid) return 0;
    std::map<CharID, BoothRecord>::iterator it = m_kOpen.find(sellerCid);
    if (it == m_kOpen.end()) return 0;
    BoothRecord& r = it->second;
    for (size_t i = 0; i < r.kListings.size(); ++i) {
        if (r.kListings[i].uiItemKey != uiItemKey) continue;
        BoothListing& L = r.kListings[i];
        if (uiQty == 0 || uiQty > L.uiQty) return 0;
        int64 cost = L.iAskingGold * (int64)uiQty;
        if ((int64)pkBuyer->GetMoney() < cost) return 0;
        // Buyer debit; the cross-character gold transfer to the seller
        // is owned by the cross-zone money path (BoothPayoutQueue).
        pkBuyer->AddMoney(-cost);
        L.uiQty -= uiQty;
        if (L.uiQty == 0) r.kListings.erase(r.kListings.begin() + i);
        return cost;
    }
    return 0;
}

const BoothRecord* BoothManager::Find(CharID c) const {
    std::map<CharID, BoothRecord>::const_iterator it = m_kOpen.find(c);
    return (it == m_kOpen.end()) ? NULL : &it->second;
}

void BoothManager::ListNearby(MapID uiMap, float fX, float fY, float fRadius,
                              std::vector<const BoothRecord*>& rOut) const {
    rOut.clear();
    float r2 = fRadius * fRadius;
    for (std::map<CharID, BoothRecord>::const_iterator it = m_kOpen.begin();
         it != m_kOpen.end(); ++it) {
        if (it->second.uiMap != uiMap) continue;
        float dx = it->second.fX - fX, dy = it->second.fY - fY;
        if (dx*dx + dy*dy <= r2) rOut.push_back(&it->second);
    }
}

} // namespace fiesta
