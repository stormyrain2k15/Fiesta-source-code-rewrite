#include "MarketSystems.h"
#include "../Shared/GTimer.h"
#include <windows.h>
#include <string.h>
namespace shine {

AuctionSystem& AuctionSystem::Get() { static AuctionSystem s; return s; }

uint32 AuctionSystem::List(CharID s, const ShineItem& it, int64 minBid, int64 buyout, uint64 dur) {
    AuctionListing l; l.uiId = m_uiNext++; l.uiSeller = s; l.kItem = it; l.iMinBid = minBid;
    l.iBuyout = buyout; l.uiEndMs = GTimer::NowMillis() + dur; l.uiBidder = INVALID_CHARID; l.iCurBid = 0;
    m_kAll[l.uiId] = l; return l.uiId;
}
bool AuctionSystem::Bid(uint32 id, CharID b, int64 a) {
    std::map<uint32, AuctionListing>::iterator it = m_kAll.find(id);
    if (it == m_kAll.end()) return false;
    if (a <= it->second.iCurBid || a < it->second.iMinBid) return false;
    it->second.iCurBid = a; it->second.uiBidder = b; return true;
}
bool AuctionSystem::Buyout(uint32 id, CharID buyer) {
    std::map<uint32, AuctionListing>::iterator it = m_kAll.find(id);
    if (it == m_kAll.end() || it->second.iBuyout <= 0) return false;
    it->second.uiBidder = buyer; it->second.iCurBid = it->second.iBuyout;
    m_kAll.erase(it); return true;
}
void AuctionSystem::Tick() {
    uint64 now = GTimer::NowMillis();
    for (std::map<uint32, AuctionListing>::iterator it = m_kAll.begin(); it != m_kAll.end(); )
        if (now >= it->second.uiEndMs) { std::map<uint32, AuctionListing>::iterator k = it++; m_kAll.erase(k); }
        else ++it;
}
void AuctionSystem::Search(const std::string&, std::vector<AuctionListing>& r) {
    r.clear();
    for (std::map<uint32, AuctionListing>::iterator it = m_kAll.begin(); it != m_kAll.end(); ++it) r.push_back(it->second);
}

void TradeBoard   ::Post(CharID, const std::string&) {}
void TradeOnBoard ::Browse(std::vector<std::string>& r) { r.clear(); }
void TradeOffBoard::Pull (CharID) {}

bool Trade::Begin (CharID,CharID)        { return true; }
bool Trade::Put   (CharID,const ShineItem&){ return true; }
bool Trade::Remove(CharID,uint32)        { return true; }
bool Trade::Accept(CharID)               { return true; }
bool Trade::Cancel(CharID)               { return true; }
int64 TradeMisc::GoldCapPerTrade() { return 2000000000LL; }

// ----- StreetBooth (persistent listings) -----------------------------------
// Each open booth holds a vector of (item, price) listings. The booth
// persists in-memory until Close() or its owning player disconnects (the
// disconnect path lives in ZoneServer::DetachPlayer in pass 2). The Tick
// handler reaps booths whose owner has been gone for more than 15 minutes
// so other players don't see ghost vendors.
namespace {
    struct BoothEntry {
        ShineItem kItem;
        int64     iPrice;
    };
    struct BoothRec {
        CharID                  uiOwner;
        std::string             kTitle;
        std::vector<BoothEntry> kListings;
        uint64                  uiCreatedMs;
        uint64                  uiOwnerLastSeenMs;
    };
    static std::map<CharID, BoothRec> s_kBooths;
}

bool StreetBooth::Open(CharID owner, const std::string& rTitle) {
    if (owner == 0) return false;
    BoothRec& r = s_kBooths[owner];
    r.uiOwner            = owner;
    r.kTitle             = rTitle;
    r.uiCreatedMs        = GTimer::NowMillis();
    r.uiOwnerLastSeenMs  = r.uiCreatedMs;
    return true;
}
void StreetBooth::Close(CharID owner) { s_kBooths.erase(owner); }

bool StreetBoothSell::ListInBooth(CharID self, const ShineItem& kIt, int64 iPrice) {
    std::map<CharID, BoothRec>::iterator it = s_kBooths.find(self);
    if (it == s_kBooths.end()) return false;
    BoothEntry e; e.kItem = kIt; e.iPrice = iPrice;
    it->second.kListings.push_back(e);
    return true;
}

bool StreetBoothBuy::BuyFromBooth(CharID self, CharID booth, uint32 uiItemId) {
    std::map<CharID, BoothRec>::iterator it = s_kBooths.find(booth);
    if (it == s_kBooths.end()) return false;
    for (size_t i = 0; i < it->second.kListings.size(); ++i) {
        if (it->second.kListings[i].kItem.uiItemId == uiItemId) {
            it->second.kListings.erase(it->second.kListings.begin() + i);
            (void)self;  // gold deduction + grant lives in pass 2 against the
                         // CharDB SQLP_Item facade.
            return true;
        }
    }
    return false;
}

void BoothManager::Tick() {
    const uint64 kStaleMs = 15ULL * 60ULL * 1000ULL;
    uint64 now = GTimer::NowMillis();
    for (std::map<CharID, BoothRec>::iterator it = s_kBooths.begin(); it != s_kBooths.end();) {
        if (now - it->second.uiOwnerLastSeenMs > kStaleMs) {
            std::map<CharID, BoothRec>::iterator dead = it++;
            s_kBooths.erase(dead);
        } else {
            ++it;
        }
    }
}

} // namespace shine
