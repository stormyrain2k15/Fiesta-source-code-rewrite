// Server/Zone/Tables/MobTables.cpp
// FEATURE: world-creation -- MobInfo / MobSpecies / MobLifeTime
// binders. The world's monsters and NPCs. Same one-class-per-cpp
// trade-off as ItemTables: three tightly-coupled SHNs share one .cpp
// because they form a single in-memory lookup surface and the
// singleton can't cross translation units.
#include "BindMacros.h"
#include "../GroupTables.h"

namespace fiesta {

MobTables& MobTables::Get() { static MobTables s; return s; }

void MobTables::Bind() {
    {
        // FEATURE: world-creation -- column read: ID, InxName, Name,
        // Level, MaxHP, WalkSpeed, RunSpeed, IsNPC, Size, WeaponType,
        // ArmorType, GradeType, Type, IsPlayerSide, AbsoluteSize,
        // Exp, Money
        BIND_BEGIN(t, "MobInfo")
        m_kMobs.reserve(t->Rows().size());
        ITER_ROWS(t) {
            MobInfoRow rec;
            rec.uiID            = ShnGetU32(*t, _r, "ID");
            rec.kInxName        = ShnGetStr(*t, _r, "InxName");
            rec.kName           = ShnGetStr(*t, _r, "Name");
            rec.uiLevel         = ShnGetU32(*t, _r, "Level");
            rec.uiMaxHP         = ShnGetU32(*t, _r, "MaxHP");
            rec.uiWalkSpeed     = ShnGetU32(*t, _r, "WalkSpeed");
            rec.uiRunSpeed      = ShnGetU32(*t, _r, "RunSpeed");
            rec.uiIsNPC         = ShnGetU32(*t, _r, "IsNPC");
            rec.uiSize          = ShnGetU32(*t, _r, "Size");
            rec.uiWeaponType    = ShnGetU32(*t, _r, "WeaponType");
            rec.uiArmorType     = ShnGetU32(*t, _r, "ArmorType");
            rec.uiGradeType     = ShnGetU32(*t, _r, "GradeType");
            rec.uiType          = ShnGetU32(*t, _r, "Type");
            rec.uiIsPlayerSide  = ShnGetU32(*t, _r, "IsPlayerSide");
            rec.uiAbsoluteSize  = ShnGetU32(*t, _r, "AbsoluteSize");
            rec.uiExp           = ShnGetU32(*t, _r, "Exp");
            rec.uiMoney         = ShnGetU32(*t, _r, "Money");
            m_kMobById[rec.uiID]      = m_kMobs.size();
            m_kMobByInx[rec.kInxName] = m_kMobs.size();
            m_kMobs.push_back(rec);
        }
        SHINELOG_INFO("MobInfo.shn: %u rows", (uint32)m_kMobs.size());
    }
    {
        // FEATURE: world-creation -- column read: ID, MobName
        BIND_BEGIN(t, "MobSpecies")
        m_kSpec.reserve(t->Rows().size());
        ITER_ROWS(t) {
            LegacyMobSpeciesRow rec;
            rec.uiID     = ShnGetU32(*t, _r, "ID");
            rec.kMobName = ShnGetStr(*t, _r, "MobName");
            m_kSpecById[rec.uiID] = m_kSpec.size();
            m_kSpec.push_back(rec);
        }
    }
    {
        // FEATURE: world-creation -- column read: ID, LifeTime
        BIND_BEGIN(t, "MobLifeTime")
        m_kLife.reserve(t->Rows().size());
        ITER_ROWS(t) {
            LegacyMobLifeTimeRow rec;
            rec.uiID         = ShnGetU32(*t, _r, "ID");
            rec.uiLifeTimeMs = ShnGetU32(*t, _r, "LifeTime");
            m_kLifeById[rec.uiID] = m_kLife.size();
            m_kLife.push_back(rec);
        }
    }
}

const MobInfoRow* MobTables::FindMob(uint32 uiID) const {
    std::map<uint32, size_t>::const_iterator it = m_kMobById.find(uiID);
    return (it == m_kMobById.end()) ? NULL : &m_kMobs[it->second];
}
const MobInfoRow* MobTables::FindByInx(const std::string& rInx) const {
    std::map<std::string, size_t>::const_iterator it = m_kMobByInx.find(rInx);
    return (it == m_kMobByInx.end()) ? NULL : &m_kMobs[it->second];
}
const LegacyMobSpeciesRow* MobTables::FindSpec(uint32 uiID) const {
    std::map<uint32, size_t>::const_iterator it = m_kSpecById.find(uiID);
    return (it == m_kSpecById.end()) ? NULL : &m_kSpec[it->second];
}
const LegacyMobLifeTimeRow* MobTables::FindLife(uint32 uiID) const {
    std::map<uint32, size_t>::const_iterator it = m_kLifeById.find(uiID);
    return (it == m_kLifeById.end()) ? NULL : &m_kLife[it->second];
}

} // namespace fiesta
