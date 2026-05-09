// Server/Zone/MobAttackSequence.cpp
#include "MobAttackSequence.h"
#include "../DataReader/TableScriptFile.h"
#include "../Shared/ShineLogSystem.h"
#include <stdio.h>

namespace fiesta {

bool MobAttackSequence::Load(const std::string& rPath) {
    m_kSteps.clear();
    TableScriptFile f;
    if (!f.Load(rPath)) return false;

    // The canonical AttSeq table -- columns: Order (WORD), Attack (STRING[33]).
    if (const TsTable* t = f.Find("AttSeq")) {
        for (size_t r = 0; r < t->kRecords.size(); ++r) {
            MobAttackStep s;
            s.kKey = t->GetStr(r, "Attack");
            std::string ord = t->GetStr(r, "Order");
            if (!ord.empty()) s.kArgs.push_back(ord);
            // Treat "-" sentinel as a "no-op"/idle slot but still record it so
            // index-based reads from the AI script line up.
            m_kSteps.push_back(s);
        }
    }
    // Optional second table -- SkillChange (used to override an entry under
    // run-time conditions like HPLow_ChangeOrder). We surface its rows as
    // additional "step overrides" -- key carries Condition+Value, args carry
    // From->To indices.
    if (const TsTable* t = f.Find("SkillChange")) {
        for (size_t r = 0; r < t->kRecords.size(); ++r) {
            MobAttackStep s;
            s.kKey = t->GetStr(r, "Condition");
            s.kArgs.push_back(t->GetStr(r, "Value"));
            s.kArgs.push_back(t->GetStr(r, "From"));
            s.kArgs.push_back(t->GetStr(r, "To"));
            m_kSteps.push_back(s);
        }
    }
    return !m_kSteps.empty();
}

MobAttackSequenceBox& MobAttackSequenceBox::Get() { static MobAttackSequenceBox s; return s; }

const MobAttackSequence* MobAttackSequenceBox::Load(const std::string& rRoot, const std::string& rName) {
    std::string path = rRoot + "\\MobAttackSequence\\" + rName + ".txt";
    MobAttackSequence* pk = new MobAttackSequence();
    if (!pk->Load(path)) { delete pk; return NULL; }
    m_kAll[rName] = pk;
    SHINELOG_DEBUG("MobAttackSequence '%s' steps=%u", rName.c_str(), (uint32)pk->Steps());
    return pk;
}

const MobAttackSequence* MobAttackSequenceBox::Find(const std::string& rName) const {
    std::map<std::string, MobAttackSequence*>::const_iterator it = m_kAll.find(rName);
    return (it == m_kAll.end()) ? NULL : it->second;
}

void MobAttackSequenceBox::Clear() {
    for (std::map<std::string, MobAttackSequence*>::iterator it = m_kAll.begin(); it != m_kAll.end(); ++it)
        delete it->second;
    m_kAll.clear();
}

} // namespace fiesta
