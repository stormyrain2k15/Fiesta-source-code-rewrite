// Server/Zone/Tables/SkillTables.cpp
// FEATURE: world-creation -- ActiveSkill / ActiveSkillInfoServer /
// PassiveSkill / AreaSkill binders.
//
// Skills bind here at the schema level only -- the deep behavior
// columns (cancel-cast, push-back, channel, knockdown frames,
// party-bonus) are still on the per-system column-audit RED list and
// will land in /Tables/ skill subsystem files when the skill engine
// gets its own sprint.
#include "BindMacros.h"
#include "../GroupTables.h"

namespace fiesta {

SkillTables& SkillTables::Get() { static SkillTables s; return s; }

void SkillTables::Bind() {
    {
        // FEATURE: world-creation -- column read: ID, InxName, Class,
        // MaxLevel, SP, CoolTime, CastingTime
        BIND_BEGIN(t, "ActiveSkill")
        m_kActive.reserve(t->Rows().size());
        ITER_ROWS(t) {
            ActiveSkillRow rec;
            rec.uiID       = ShnGetU32(*t, _r, "ID");
            rec.kInxName   = ShnGetStr(*t, _r, "InxName");
            rec.uiClass    = ShnGetU32(*t, _r, "Class");
            rec.uiMaxLevel = ShnGetU32(*t, _r, "MaxLevel");
            rec.uiSP       = ShnGetU32(*t, _r, "SP");
            rec.uiCoolMs   = ShnGetU32(*t, _r, "CoolTime");
            rec.uiCastMs   = ShnGetU32(*t, _r, "CastingTime");
            m_kActiveById[rec.uiID] = m_kActive.size();
            m_kActive.push_back(rec);
        }
        SHINELOG_INFO("ActiveSkill.shn: %u rows", (uint32)m_kActive.size());
    }
    {
        // FEATURE: world-creation -- column read: ID, BaseDamage,
        // TargetType, Range, AggroBonus
        BIND_BEGIN(t, "ActiveSkillInfoServer")
        m_kActiveS.reserve(t->Rows().size());
        ITER_ROWS(t) {
            ActiveSkillInfoServerRow rec;
            rec.uiID         = ShnGetU32(*t, _r, "ID");
            rec.uiBaseDamage = ShnGetU32(*t, _r, "BaseDamage");
            rec.uiTargetType = ShnGetU32(*t, _r, "TargetType");
            rec.uiRange      = ShnGetU32(*t, _r, "Range");
            rec.uiAggroBonus = ShnGetU32(*t, _r, "AggroBonus");
            m_kActiveSById[rec.uiID] = m_kActiveS.size();
            m_kActiveS.push_back(rec);
        }
    }
    {
        // FEATURE: world-creation -- column read: ID, InxName, Class,
        // MaxLevel
        BIND_BEGIN(t, "PassiveSkill")
        m_kPassive.reserve(t->Rows().size());
        ITER_ROWS(t) {
            PassiveSkillRow rec;
            rec.uiID       = ShnGetU32(*t, _r, "ID");
            rec.kInxName   = ShnGetStr(*t, _r, "InxName");
            rec.uiClass    = ShnGetU32(*t, _r, "Class");
            rec.uiMaxLevel = ShnGetU32(*t, _r, "MaxLevel");
            m_kPassiveById[rec.uiID] = m_kPassive.size();
            m_kPassive.push_back(rec);
        }
    }
    {
        // AreaSkill.shn -- bound separately by AreaSkillTable
        // (Server/Zone/MoreTables.cpp). NA2016's AreaSkill.shn schema
        // is (AS_SkillInx, AS_Step, AS_BMPIndex, AS_ImagePin,
        // AS_IsDirection) which doesn't fit this SkillTables surface.
        // The accessor stubs below return NULL; callers should query
        // AreaSkillTable::Get().Find(...) instead.
    }
}

const ActiveSkillRow* SkillTables::FindActive(uint32 uiID) const {
    std::map<uint32, size_t>::const_iterator it = m_kActiveById.find(uiID);
    return (it == m_kActiveById.end()) ? NULL : &m_kActive[it->second];
}
const ActiveSkillInfoServerRow* SkillTables::FindActiveS(uint32 uiID) const {
    std::map<uint32, size_t>::const_iterator it = m_kActiveSById.find(uiID);
    return (it == m_kActiveSById.end()) ? NULL : &m_kActiveS[it->second];
}
const PassiveSkillRow* SkillTables::FindPassive(uint32 uiID) const {
    std::map<uint32, size_t>::const_iterator it = m_kPassiveById.find(uiID);
    return (it == m_kPassiveById.end()) ? NULL : &m_kPassive[it->second];
}
const AreaSkillRow* SkillTables::FindArea(uint32 uiID) const {
    std::map<uint32, size_t>::const_iterator it = m_kAreaById.find(uiID);
    return (it == m_kAreaById.end()) ? NULL : &m_kArea[it->second];
}

} // namespace fiesta
