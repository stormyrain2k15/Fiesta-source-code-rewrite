// Server/Zone/Tables/MobBattleTables.cpp
// FEATURE: world-creation -- MobAutoAction.shn + MobWeapon.shn +
// MobResist.shn binders. Three tightly-coupled mob-combat SHNs that
// drive every mob's auto-attack pattern, weapon stats, and elemental
// resists. Shipping as one .cpp because they form one MobBattleTables
// surface; the singleton crosses all three.
#include "BindMacros.h"
#include "../MoreTables.h"

namespace shine {

MobBattleTables& MobBattleTables::Get() { static MobBattleTables s; return s; }

void MobBattleTables::Bind() {
    // FEATURE: world-creation -- column read: MobInx, Attack, Target,
    // ActionType, StateInx, Strength, EffectRate, Range
    if (const ShnFile* t = ShnRegistry::Get().GetTable("MobAutoAction")) {
        ITER_ROWS(t) {
            AutoActionRow r;
            r.kMobInx     = ShnGetStr(*t, _r, "MobInx");
            r.uiAttack    = ShnGetU32(*t, _r, "Attack");
            r.uiTarget    = ShnGetU32(*t, _r, "Target");
            r.uiActionType= ShnGetU32(*t, _r, "ActionType");
            r.kStateInx   = ShnGetStr(*t, _r, "StateInx");
            r.uiStrength  = ShnGetU32(*t, _r, "Strength");
            r.uiEffectRate= ShnGetU32(*t, _r, "EffectRate");
            r.uiRange     = ShnGetU32(*t, _r, "Range");
            m_kAuto.push_back(r);
        }
    }
    // FEATURE: world-creation -- column read: ID, InxName, Skill,
    // AtkSpd, BlastRate, AtkDly, SwingTime, HitTime, AtkType, MinWC,
    // MaxWC, TH, MinMA, MaxMA, MH, Range, MopAttackTarget, HitType,
    // StaName, StaStrength, StaRate, AggroInitialize
    if (const ShnFile* t = ShnRegistry::Get().GetTable("MobWeapon")) {
        ITER_ROWS(t) {
            WeaponRow r;
            r.uiID            = ShnGetU32(*t, _r, "ID");
            r.kInxName        = ShnGetStr(*t, _r, "InxName");
            r.kSkill          = ShnGetStr(*t, _r, "Skill");
            r.uiAtkSpd        = ShnGetU32(*t, _r, "AtkSpd");
            r.uiBlastRate     = ShnGetU32(*t, _r, "BlastRate");
            r.uiAtkDly        = ShnGetU32(*t, _r, "AtkDly");
            r.uiSwingTime     = ShnGetU32(*t, _r, "SwingTime");
            r.uiHitTime       = ShnGetU32(*t, _r, "HitTime");
            r.uiAtkType       = ShnGetU32(*t, _r, "AtkType");
            r.uiMinWC         = ShnGetU32(*t, _r, "MinWC");
            r.uiMaxWC         = ShnGetU32(*t, _r, "MaxWC");
            r.uiTH            = ShnGetU32(*t, _r, "TH");
            r.uiMinMA         = ShnGetU32(*t, _r, "MinMA");
            r.uiMaxMA         = ShnGetU32(*t, _r, "MaxMA");
            r.uiMH            = ShnGetU32(*t, _r, "MH");
            r.uiRange         = ShnGetU32(*t, _r, "Range");
            r.uiMopAttackTarget=ShnGetU32(*t, _r, "MopAttackTarget");
            r.uiHitType       = ShnGetU32(*t, _r, "HitType");
            r.kStaName        = ShnGetStr(*t, _r, "StaName");
            r.uiStaStrength   = ShnGetU32(*t, _r, "StaStrength");
            r.uiStaRate       = ShnGetU32(*t, _r, "StaRate");
            r.uiAggroInitialize=ShnGetU32(*t, _r, "AggroInitialize");
            m_kWeaponById[r.uiID] = m_kWeapon.size();
            m_kWeapon.push_back(r);
        }
    }
    // FEATURE: world-creation -- column read: InxName, ResDot, ResStun,
    // ResMoveSpeed, ResFear, ResBinding, ResReverse, ResMesmerize,
    // ResSeverBone, ResKnockBack, ResTBMinus
    if (const ShnFile* t = ShnRegistry::Get().GetTable("MobResist")) {
        ITER_ROWS(t) {
            ResistRow r;
            r.kInxName      = ShnGetStr(*t, _r, "InxName");
            r.iResDot       = ShnGetI32(*t, _r, "ResDot");
            r.iResStun      = ShnGetI32(*t, _r, "ResStun");
            r.iResMoveSpeed = ShnGetI32(*t, _r, "ResMoveSpeed");
            r.iResFear      = ShnGetI32(*t, _r, "ResFear");
            r.iResBinding   = ShnGetI32(*t, _r, "ResBinding");
            r.iResReverse   = ShnGetI32(*t, _r, "ResReverse");
            r.iResMesmerize = ShnGetI32(*t, _r, "ResMesmerize");
            r.iResSeverBone = ShnGetI32(*t, _r, "ResSeverBone");
            r.iResKnockBack = ShnGetI32(*t, _r, "ResKnockBack");
            r.iResTBMinus   = ShnGetI32(*t, _r, "ResTBMinus");
            m_kResistByInx[r.kInxName] = m_kResist.size();
            m_kResist.push_back(r);
        }
    }
}

const MobBattleTables::WeaponRow* MobBattleTables::FindWeapon(uint32 uID) const {
    std::map<uint32, size_t>::const_iterator it = m_kWeaponById.find(uID);
    return (it == m_kWeaponById.end()) ? NULL : &m_kWeapon[it->second];
}
const MobBattleTables::ResistRow* MobBattleTables::FindResist(const std::string& rN) const {
    std::map<std::string, size_t>::const_iterator it = m_kResistByInx.find(rN);
    return (it == m_kResistByInx.end()) ? NULL : &m_kResist[it->second];
}
void MobBattleTables::AutoActionsByMob(const std::string& rN,
                                       std::vector<const AutoActionRow*>& rOut) const {
    rOut.clear();
    for (size_t i = 0; i < m_kAuto.size(); ++i)
        if (m_kAuto[i].kMobInx == rN) rOut.push_back(&m_kAuto[i]);
}

} // namespace shine
