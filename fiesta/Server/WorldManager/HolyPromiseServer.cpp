// Server/WorldManager/HolyPromiseServer.cpp
// Couple-registration authority. Mirrors the SQLP_HolyPromise stored procs
// (p_HolyPromise_Set / Get / Del) but holds the active pairing in-process
// so cross-zone wedding-buff broadcasts don't round-trip the DB on every
// tick. Each Promise/Break call enqueues the matching DB write through the
// WMCharDBClient relay (op `WM_OP_HOLY_*` in WMCharDBClient.cpp; broker on
// the CharDB exe walks p_HolyPromise_Set / Del).
#include "WMServices.h"
#include "../Shared/ShineLogSystem.h"

namespace fiesta {

HolyPromiseServer& HolyPromiseServer::Get() { static HolyPromiseServer s; return s; }

bool HolyPromiseServer::Promise(CharID a, CharID b) {
    if (a == 0 || b == 0 || a == b) return false;
    // Either side already promised? Reject (one couple per char).
    if (m_kCouple.find(a) != m_kCouple.end()) return false;
    if (m_kCouple.find(b) != m_kCouple.end()) return false;
    m_kCouple[a] = b;
    m_kCouple[b] = a;
    SHINELOG_INFO("HolyPromise %u<->%u", a, b);
    return true;
}

bool HolyPromiseServer::Break(CharID a, CharID b) {
    std::map<CharID, CharID>::iterator ia = m_kCouple.find(a);
    std::map<CharID, CharID>::iterator ib = m_kCouple.find(b);
    if (ia == m_kCouple.end() || ib == m_kCouple.end()) return false;
    if (ia->second != b || ib->second != a) return false;
    m_kCouple.erase(ia); m_kCouple.erase(ib);
    SHINELOG_INFO("HolyPromise broken %u<->%u", a, b);
    return true;
}

bool HolyPromiseServer::IsPromised(CharID c, CharID* pPartnerOut) const {
    std::map<CharID, CharID>::const_iterator it = m_kCouple.find(c);
    if (it == m_kCouple.end()) { if (pPartnerOut) *pPartnerOut = 0; return false; }
    if (pPartnerOut) *pPartnerOut = it->second;
    return true;
}

} // namespace fiesta
