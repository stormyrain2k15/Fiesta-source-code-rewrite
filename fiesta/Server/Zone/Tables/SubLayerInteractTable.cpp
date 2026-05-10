// Server/Zone/Tables/SubLayerInteractTable.cpp
// FEATURE: world-creation -- World/SubLayerInteract.txt binder.
// 8 x 6 visibility / attackability matrix between sub-layers
// (Base/RangerStealth/Observer/AdminHide/GMDoor/GMPlayer). Drives
// stealth detection and PvP gating across stealth levels.
#include "../WorldTables.h"
#include "../../DataReader/TableScriptFile.h"
#include <string.h>

namespace fiesta {

SubLayerInteractTable& SubLayerInteractTable::Get() { static SubLayerInteractTable s; return s; }

bool SubLayerInteractTable::Load(const std::string& rRoot) {
    memset(m_aBrief, 0, sizeof(m_aBrief));
    memset(m_aAttack, 0, sizeof(m_aAttack));
    TableScriptFile f;
    if (!f.Load(rRoot + "\\World\\SubLayerInteract.txt")) return false;
    static const char* kCols[6] = {
        "Base","RangerStealth","Observer","AdminHide","GMDoor","GMPlayer"
    };
    // FEATURE: world-creation -- column read: CheckSumA, Base,
    // RangerStealth, Observer, AdminHide, GMDoor, GMPlayer
    if (const TsTable* t = f.Find("SendMyBrief")) {
        for (size_t r = 0; r < t->kRecords.size(); ++r) {
            uint8 my = (uint8)t->GetInt(r, "CheckSumA");
            if (my >= 8) continue;
            for (uint8 c = 0; c < 6; ++c)
                m_aBrief[my][c] = (uint8)t->GetInt(r, kCols[c]);
        }
    }
    if (const TsTable* t = f.Find("CanAttack")) {
        for (size_t r = 0; r < t->kRecords.size(); ++r) {
            uint8 my = (uint8)t->GetInt(r, "CheckSumA");
            if (my >= 8) continue;
            for (uint8 c = 0; c < 6; ++c)
                m_aAttack[my][c] = (uint8)t->GetInt(r, kCols[c]);
        }
    }
    return true;
}

bool SubLayerInteractTable::CanSeeMyBrief(uint8 m, uint8 o) const {
    return (m < 8 && o < 8) ? (m_aBrief[m][o] != 0) : false;
}
bool SubLayerInteractTable::CanAttack(uint8 m, uint8 o) const {
    return (m < 8 && o < 8) ? (m_aAttack[m][o] != 0) : false;
}

} // namespace fiesta
