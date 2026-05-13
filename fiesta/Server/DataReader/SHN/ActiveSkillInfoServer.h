// Server/DataReader/SHN/ActiveSkillInfoServer.h
// Auto-generated: one-file-per-SHN split for ActiveSkillInfoServer.shn
#ifndef SHINE_DATAREADER_SHN_ACTIVESKILLINFOSERVER_H
#define SHINE_DATAREADER_SHN_ACTIVESKILLINFOSERVER_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace shine {

struct ActiveSkillInfoServerRow {
    uint16           uiID;
    std::string      kInxName;
    uint8            uiUsualAttack;
    uint32           uiSkilPyHitRate;
    uint32           uiSkilMaHitRate;
    uint32           uiPsySucRate;
    uint32           uiMagSucRate;
    uint8            uiStaLevel;
    uint32           uiDmgIncRate;
    uint16           uiDmgIncValue;
    uint32           uiSkillHitType;
    uint8            uiItremUseSkill;
    uint32           uiAggroPerDamage;
    uint32           uiAbsoluteAggro;
    uint8            uiAttackStart;
    uint8            uiAttackEnd;
    uint16           uiSwingTime;
    uint16           uiHitTime;
    uint8            uiAddSoul;
};

class ActiveSkillInfoServerShn {
public:
    static ActiveSkillInfoServerShn& Get();
    void Load();
    const ActiveSkillInfoServerRow* FindById(uint32 uiID) const;
    const ActiveSkillInfoServerRow* FindByInx(const std::string& rInx) const;
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<ActiveSkillInfoServerRow>& Rows() const { return m_kRows; }
private:
    std::vector<ActiveSkillInfoServerRow>         m_kRows;
    std::map<uint32,      size_t>  m_kById;
    std::map<std::string, size_t>  m_kByInx;
};

} // namespace shine
#endif // SHINE_DATAREADER_SHN_ACTIVESKILLINFOSERVER_H
