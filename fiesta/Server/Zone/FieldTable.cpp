// Server/Zone/FieldTable.cpp
#include "FieldTable.h"
#include "../DataReader/TableScriptFile.h"
#include "../Shared/ShineLogSystem.h"
#include <stdlib.h>

namespace fiesta {

static uint32 U32(const TsTable& t, size_t r, const char* k, uint32 d = 0) {
    return (uint32)t.GetInt(r, k, (int64)d);
}
static uint16 U16(const TsTable& t, size_t r, const char* k, uint16 d = 0) {
    return (uint16)t.GetInt(r, k, (int64)d);
}
static uint8  U8 (const TsTable& t, size_t r, const char* k, uint8 d = 0) {
    return (uint8) t.GetInt(r, k, (int64)d);
}
static std::string S(const TsTable& t, size_t r, const char* k) {
    return t.GetStr(r, k, "");
}

FieldTable& FieldTable::Get() { static FieldTable s; return s; }

bool FieldTable::Load(const std::string& rRoot) {
    m_kRows.clear(); m_kBySerial.clear(); m_kByName.clear();
    std::string path = rRoot + "\\World\\Field.txt";
    TableScriptFile f;
    if (!f.Load(path)) return false;
    const TsTable* tab = f.Find("FieldList");
    if (!tab) {
        SHINELOG_ERROR("FieldTable: World/Field.txt has no FieldList table");
        return false;
    }
    for (size_t r = 0; r < tab->kRecords.size(); ++r) {
        FieldRow row;
        row.kMapIDClient        = S (*tab, r, "MapIDClient");
        row.kSubFrom            = S (*tab, r, "SubFrom");
        row.kSubTo              = S (*tab, r, "SubTo");
        row.uiSerial            = U8 (*tab, r, "Serial");
        row.kMapName            = S (*tab, r, "MapName");
        // Replace '#' with ' ' (the file uses #Exchange to do this; some rows
        // may not have been processed if exchange ran before tokenisation).
        for (size_t i = 0; i < row.kMapName.size(); ++i)
            if (row.kMapName[i] == '#') row.kMapName[i] = ' ';
        row.uiKQ                = U8 (*tab, r, "KQ");
        row.uiXSize             = U32(*tab, r, "xsize");
        row.uiYSize             = U32(*tab, r, "ysize");
        row.uiImmortalSec       = U16(*tab, r, "immortalsec");
        row.kScriptName         = S (*tab, r, "ScriptName");
        row.uiItemDropByKilled  = U8 (*tab, r, "ItemDropByKilled");
        row.uiPker              = U8 (*tab, r, "Pker");
        row.uiEnemyGuild        = U8 (*tab, r, "EnemyGuild");
        row.uiParty             = U8 (*tab, r, "Party");
        row.uiMonster           = U8 (*tab, r, "Monster");
        row.uiSummoned          = U8 (*tab, r, "Summoned");
        row.uiIsPKKQ            = U8 (*tab, r, "IsPKKQ");
        row.uiIsFreePK          = U8 (*tab, r, "IsFreePK");
        row.uiIsPartyBattle     = U8 (*tab, r, "IsPartyBattle");
        row.uiNameHide          = U8 (*tab, r, "NameHide");
        row.uiLinkIN            = U8 (*tab, r, "LinkIN");
        row.uiLinkOut           = U8 (*tab, r, "LinkOut");
        row.uiSystemMap         = U8 (*tab, r, "SystemMap");
        row.kRegenCity          = S (*tab, r, "RegenCity");
        row.uiRegenXA           = U32(*tab, r, "RegenXA");
        row.uiRegenYA           = U32(*tab, r, "RegenYA");
        row.uiRegenXB           = U32(*tab, r, "RegenXB");
        row.uiRegenYB           = U32(*tab, r, "RegenYB");
        row.uiRegenXC           = U32(*tab, r, "RegenXC");
        row.uiRegenYC           = U32(*tab, r, "RegenYC");
        row.uiRegenXD           = U32(*tab, r, "RegenXD");
        row.uiRegenYD           = U32(*tab, r, "RegenYD");
        row.uiRegenSpot         = U8 (*tab, r, "RegenSpot");
        row.uiCanRestart        = U8 (*tab, r, "CanRestart");
        row.uiCanTrade          = U8 (*tab, r, "CanTrade");
        row.uiCanMiniHouse      = U8 (*tab, r, "CanMiniHouse");
        row.uiCanItem           = U8 (*tab, r, "CanItem");
        row.uiCanSkill          = U8 (*tab, r, "CanSkill");
        row.uiCanChat           = U8 (*tab, r, "Chat");
        row.uiCanShout          = U8 (*tab, r, "Shout");
        row.uiCanBooth          = U8 (*tab, r, "CanBooth");
        row.uiCanProduce        = U8 (*tab, r, "CanProduce");
        row.uiCanRide           = U8 (*tab, r, "CanRide");
        row.uiCanStone          = U8 (*tab, r, "CanStone");
        row.uiCanParty          = U8 (*tab, r, "CanParty");
        row.uiExpLostAtDeadByMob= U16(*tab, r, "ExpLostAtDeadByMob");
        row.uiExpLostAtDeadByPly= U16(*tab, r, "ExpLostAtDeadByPly");
        row.uiUsrSubLayer       = U8 (*tab, r, "UsrSubLayer");
        row.uiCheckSum          = U8 (*tab, r, "CheckSum");
        row.uiFiesta            = U8 (*tab, r, "Fiesta");

        m_kBySerial[row.uiSerial] = m_kRows.size();
        if (!row.kMapIDClient.empty()) m_kByName[row.kMapIDClient] = m_kRows.size();
        m_kRows.push_back(row);
    }
    SHINELOG_INFO("FieldTable: %u rows", (uint32)m_kRows.size());
    return true;
}

const FieldRow* FieldTable::FindBySerial(uint8 uiSerial) const {
    std::map<uint8, size_t>::const_iterator it = m_kBySerial.find(uiSerial);
    return (it == m_kBySerial.end()) ? NULL : &m_kRows[it->second];
}
const FieldRow* FieldTable::FindByName(const std::string& rN) const {
    std::map<std::string, size_t>::const_iterator it = m_kByName.find(rN);
    return (it == m_kByName.end()) ? NULL : &m_kRows[it->second];
}

} // namespace fiesta
