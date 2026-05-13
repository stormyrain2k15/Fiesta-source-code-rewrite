// Server/Zone/MarketSystems.h
// Auction / TradeBoard / Trade / StreetBooth.
//                                  Trade, TradeMisc, StreetBooth, StreetBoothBuy/Sell, BoothManager
#ifndef SHINE_ZONE_MARKETSYSTEMS_H
#define SHINE_ZONE_MARKETSYSTEMS_H
#include "Inventory.h"
#include <map>
#include <vector>
#include <string>

namespace shine {

struct AuctionListing { uint32 uiId; CharID uiSeller; ShineItem kItem; int64 iMinBid; int64 iBuyout; uint64 uiEndMs; CharID uiBidder; int64 iCurBid; };

class AuctionSystem {
public:
    static AuctionSystem& Get();
    uint32 List   (CharID seller, const ShineItem& it, int64 minBid, int64 buyout, uint64 uiDurMs);
    bool   Bid    (uint32 uiId, CharID bidder, int64 iAmount);
    bool   Buyout (uint32 uiId, CharID buyer);
    void   Tick();
    void   Search (const std::string& rNeedle, std::vector<AuctionListing>& rOut);
private:
    AuctionSystem() : m_uiNext(1) {}
    std::map<uint32, AuctionListing> m_kAll;
    uint32 m_uiNext;
};

class TradeBoard    { public: static void Post(CharID c, const std::string& msg); };
class TradeOnBoard  { public: static void Browse(std::vector<std::string>& rOut); };
class TradeOffBoard { public: static void Pull  (CharID c); };

struct TradeSlot { CharID owner; std::vector<ShineItem> kOffer; int64 iGold; bool bAccepted; bool bLocked; };
class Trade {
public:
    static bool Begin (CharID a, CharID b);
    static bool Put   (CharID self, const ShineItem& kIt);
    static bool Remove(CharID self, uint32 uiItemId);
    static bool Accept(CharID self);
    static bool Cancel(CharID self);
};
class TradeMisc { public: static int64 GoldCapPerTrade(); };

class StreetBooth {
public:
    static bool Open (CharID owner, const std::string& rTitle);
    static void Close(CharID owner);
};
class StreetBoothBuy  { public: static bool BuyFromBooth (CharID self, CharID booth, uint32 uiItemId); };
class StreetBoothSell { public: static bool ListInBooth  (CharID self, const ShineItem& kIt, int64 iPrice); };
class BoothManager    { public: static void Tick(); };

} // namespace shine
#endif
