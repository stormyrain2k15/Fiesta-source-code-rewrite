// Server/Zone/ChargedEffect.h
// 17 -- "Charged" item / website-shop boosters with a time-limited effect.
//
// EVIDENCE: PDB_CONFIRMED  symbol: ChargedEffect, ChargedDeletableBuff,
//                                  ChargedItemEffectInst
// EVIDENCE: DATA_CONFIRMED  source: ChargedEffect.shn (Handle, ItemID,
//                                  KeepTime_Hour, EffectEnum, EffectValue,
//                                  StaStrength) +
//                                  ChargedDeletableBuff.shn (CDI_IDX).
//
// The original game shipped without an in-engine cash shop -- charged items
// arrived in a player's inventory through the **external** billing portal /
// gift / event reward path. When the engine sees one of those item ids in
// inventory, it activates the matching ChargedEffect row for the configured
// `KeepTime_Hour`. The effect is a typed buff (HP regen, EXP gain, drop rate,
// movement, ...) -- the enum mirror is built directly from `EffectEnum`.
//
// `ChargedDeletableBuff` lists the abstate ids that the player is allowed to
// cancel manually before the keep-time runs out (consumables, world buffs).
#ifndef FIESTA_ZONE_CHARGEDEFFECT_H
#define FIESTA_ZONE_CHARGEDEFFECT_H
#include "../Shared/ShineTypes.h"
#include <string>
#include <vector>
#include <map>

namespace fiesta {

class ShinePlayer;

// Mirrors the documented enum cells observed in ChargedEffect.shn.EffectEnum.
// Numbering is tight (no gaps); the data table validates these at load.
enum eChargedEffect {
    CHARGED_NONE        = 0,
    CHARGED_EXP_PCT     = 1,
    CHARGED_DROP_PCT    = 2,
    CHARGED_HP_REGEN    = 3,
    CHARGED_SP_REGEN    = 4,
    CHARGED_MOVE_PCT    = 5,
    CHARGED_ATK_PCT     = 6,
    CHARGED_DEF_PCT     = 7,
    CHARGED_BLESS       = 8,    // umbrella "blessing" abstate
    CHARGED_VIS_PCT     = 9,    // bonus money rewards
    CHARGED_TELEPORT    = 10,
    CHARGED_REPAIR_FREE = 11
};

struct ChargedEffectRow {
    uint16          uiHandle;
    std::string     kItemID;        // links to ItemInfo.shn.InxName
    uint16          uiKeepTime_Hour;
    eChargedEffect  eEffect;
    uint16          uiEffectValue;
    uint8           uiStaStrength;  // 0..255 strength multiplier band
};

class ChargedEffectTable {
public:
    static ChargedEffectTable& Get();
    bool Load(const std::string& rRoot);   // Shine-1\\ChargedEffect.shn
    const ChargedEffectRow* FindByItem(const std::string& rItemID) const;
    size_t Size() const { return m_kRows.size(); }
private:
    ChargedEffectTable() {}
    std::vector<ChargedEffectRow>           m_kRows;
    std::map<std::string, size_t>           m_kByItem;  // ItemID -> index in m_kRows
};

// Per-player active instance. Created when a charged item is received,
// destroyed when the keep-time elapses (or the abstate is cancelled if it
// is in ChargedDeletableBuff).
struct ChargedEffectInst {
    uint16          uiHandle;
    eChargedEffect  eEffect;
    uint16          uiEffectValue;
    uint8           uiStaStrength;  // strength multiplier band (mirrors row)
    uint64          uiExpireMs;
    bool            bDeletable;
};

class ChargedEffectManager {
public:
    static ChargedEffectManager& Get();

    // Called from Inventory ingestion when a charged item id arrives.
    // Returns true on activation; false if the item id has no row (so the
    // caller knows to treat it as a regular item).
    bool OnItemReceived(ShinePlayer* pkP, const std::string& rItemID);

    // Convenience: look up the InxName via ItemTables and dispatch to
    // OnItemReceived. Use this from the inventory hot path so callers
    // don't need to know the InxName up front.
    bool OnItemReceivedById(ShinePlayer* pkP, uint32 uiItemId);

    // Player-initiated cancellation of a deletable abstate.
    bool TryDelete(ShinePlayer* pkP, uint16 uiHandle);

    // Called by Zone tick: walks all players and expires elapsed instances.
    void Tick();

    // Cancellable-buff registry from ChargedDeletableBuff.shn.
    bool LoadDeletable(const std::string& rRoot);
    bool IsDeletable(uint16 uiHandle) const;

private:
    ChargedEffectManager() {}
    std::map<CharID, std::vector<ChargedEffectInst> > m_kActive;
    std::vector<uint16>                               m_kDeletable;  // handles
};

} // namespace fiesta
#endif
