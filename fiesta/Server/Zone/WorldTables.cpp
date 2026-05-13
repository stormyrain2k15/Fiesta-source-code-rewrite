// Server/Zone/WorldTables.cpp
//
// World/*.txt binders -- one .cpp per .txt under /Tables/, per the
// per-SHN convention adopted 2026-02 (see docs/PER_SHN_CONVENTION.md).
// This file is now intentionally short:
//   - The 12 standard World/*.txt binders moved to /Tables/<Name>.cpp.
//   - World/Karen.txt stays here for now because it's flagged for
//     relocation -- gate-generation belongs in a dedicated gate-gen
//     module, not under /Tables/. The KarenAttackTable will move with
//     it when that module lands.
//
// Splits owned by /Tables/:
//   QuestTable.cpp             World/Quest.txt
//   ExpRecalcTable.cpp         World/ExpRecalculation.txt
//   RecallCoordTable.cpp       World/RecallCoord.txt
//   DamageByAngleTable.cpp     World/DamageByAngle.txt
//   DamageBySoulTable.cpp      World/DamageBySoul.txt
//   ItemUseFunctionTable.cpp   World/ItemUseFunction.txt
//   RandomOptionTable.cpp      World/RandomOptionTable.txt
//   ItemDropGroupTable.cpp     World/ItemDropGroup.txt
//   PineScriptTable.cpp        World/PineScript.txt + LoadAllPineScripts
//   SubLayerInteractTable.cpp  World/SubLayerInteract.txt
//   NPCActionTable.cpp         World/NPCAction.txt
//   MobChatTable.cpp           World/MobChat.txt
//
// Already-separate per-file binders (pre-existing):
//   ChrCommonTable.cpp         World/ChrCommon.txt
//   ShineNPCTable.cpp          World/NPC.txt
//   FieldTable.cpp             World/Field.txt
#include "WorldTables.h"
#include "../DataReader/TableScriptFile.h"
#include "../Shared/ShineLogSystem.h"

namespace shine {

// =============================================================================
// KarenAttackTable -- World/Karen.txt
// PROVISIONAL_LOCATION: belongs in a future Server/Zone/GateGen/ module
// alongside the gate-generation runtime; left here until that module
// is added. Do not split this binder into /Tables/ -- it is not a
// per-table world-data binder, it is gate-attack-sequence behavior.
// =============================================================================
KarenAttackTable& KarenAttackTable::Get() { static KarenAttackTable s; return s; }

bool KarenAttackTable::Load(const std::string& rRoot) {
    m_kRows.clear();
    TableScriptFile f;
    if (!f.Load(rRoot + "\\World\\Karen.txt")) return false;
    const TsTable* t = f.Find("AttSeq"); if (!t) return false;
    for (size_t r = 0; r < t->kRecords.size(); ++r) {
        uint16 ord = (uint16)t->GetInt(r, "Order");
        std::string atk = t->GetStr(r, "Attack");
        if (atk == "-") atk.clear();
        m_kRows[ord] = atk;
    }
    SHINELOG_INFO("Karen.txt: %u attack steps loaded", (uint32)m_kRows.size());
    return true;
}

const std::string& KarenAttackTable::AttackAt(uint16 uiOrder) const {
    std::map<uint16, std::string>::const_iterator it = m_kRows.find(uiOrder);
    return (it == m_kRows.end()) ? m_kEmpty : it->second;
}

} // namespace shine
