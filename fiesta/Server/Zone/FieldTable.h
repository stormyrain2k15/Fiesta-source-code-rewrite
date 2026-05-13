// Server/Zone/FieldTable.h
// typed loader for World/Field.txt (FieldList table).
// Source format: TableScript (#Table FieldList / #ColumnType / #Record ...).
#ifndef SHINE_ZONE_FIELDTABLE_H
#define SHINE_ZONE_FIELDTABLE_H
#include "../Shared/ShineTypes.h"
#include <map>
#include <string>
#include <vector>

namespace shine {

struct FieldRow {
    std::string kMapIDClient;     // e.g. "Rou", "Eld", "RouCos01"
    std::string kSubFrom;
    std::string kSubTo;
    uint8       uiSerial;
    std::string kMapName;         // human-readable, '#' replaced with space
    uint8       uiKQ;
    uint32      uiXSize;
    uint32      uiYSize;
    uint16      uiImmortalSec;
    std::string kScriptName;
    uint8       uiItemDropByKilled;
    uint8       uiPker;
    uint8       uiEnemyGuild;
    uint8       uiParty;
    uint8       uiMonster;
    uint8       uiSummoned;
    uint8       uiIsPKKQ;
    uint8       uiIsFreePK;
    uint8       uiIsPartyBattle;
    uint8       uiNameHide;
    uint8       uiLinkIN;
    uint8       uiLinkOut;
    uint8       uiSystemMap;
    std::string kRegenCity;
    uint32      uiRegenXA, uiRegenYA;
    uint32      uiRegenXB, uiRegenYB;
    uint32      uiRegenXC, uiRegenYC;
    uint32      uiRegenXD, uiRegenYD;
    uint8       uiRegenSpot;
    uint8       uiCanRestart;
    uint8       uiCanTrade;
    uint8       uiCanMiniHouse;
    uint8       uiCanItem;
    uint8       uiCanSkill;
    uint8       uiCanChat;
    uint8       uiCanShout;
    uint8       uiCanBooth;
    uint8       uiCanProduce;
    uint8       uiCanRide;
    uint8       uiCanStone;
    uint8       uiCanParty;
    uint16      uiExpLostAtDeadByMob;
    uint16      uiExpLostAtDeadByPly;
    uint8       uiUsrSubLayer;
    uint8       uiCheckSum;
    uint8       uiShine;
};

class FieldTable {
public:
    static FieldTable& Get();

    // Load from the supplied data root (the directory that contains "World").
    bool   Load(const std::string& rRoot);
    size_t Count() const { return m_kRows.size(); }

    const FieldRow* FindBySerial(uint8 uiSerial) const;
    const FieldRow* FindByName  (const std::string& rMapIDClient) const;
    const std::vector<FieldRow>& Rows() const { return m_kRows; }

private:
    FieldTable() {}
    std::vector<FieldRow>            m_kRows;
    std::map<uint8, size_t>          m_kBySerial;
    std::map<std::string, size_t>    m_kByName;
};

} // namespace shine
#endif
