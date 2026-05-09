#include "MarketSystems.h"
#include "../Shared/GTimer.h"
#include <string.h>
namespace fiesta {

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

bool StreetBooth::Open (CharID,const std::string&){ return true; }
void StreetBooth::Close(CharID) {}
bool StreetBoothBuy::BuyFromBooth(CharID,CharID,uint32) { return true; }
bool StreetBoothSell::ListInBooth(CharID,const ShineItem&,int64) { return true; }
void BoothManager::Tick() {}

} // namespace fiesta
