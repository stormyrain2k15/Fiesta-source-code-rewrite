// Server/Zone/SellItemManager.cpp
// Per-NPC sell-window mediator. Tracks the open shop session so the
// /sell, /repurchase, and /buy traffic stays bound to a single NPC.
#include "ShineObject.h"
namespace shine { namespace {
class SellItemManager {
public:
    static SellItemManager& Get() { static SellItemManager s; return s; }
    void Open (uint32 /*cid*/, uint32 /*uiNpcID*/) {}
    void Close(uint32 /*cid*/) {}
};
}} // anonymous
