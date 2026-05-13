// Server/DataReader/SHN/PassiveSkill.h
// Auto-generated: one-file-per-SHN split for PassiveSkill.shn
#ifndef SHINE_DATAREADER_SHN_PASSIVESKILL_H
#define SHINE_DATAREADER_SHN_PASSIVESKILL_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace shine {

struct PassiveSkillRow {
    uint16           uiID;
    std::string      kInxName;
    std::string      kName;
    uint32           uiWeaponMastery;
    std::string      kDemandSk;
    uint32           uiMstRtTmp;
    uint32           uiMstRtSword1;
    uint32           uiMstRtHammer1;
    uint32           uiMstRtSword2;
    uint32           uiMstRtAxe2;
    uint32           uiMstRtMace1;
    uint32           uiMstRtBow2;
    uint32           uiMstRtCrossBow2;
    uint32           uiMstRtWand2;
    uint32           uiMstRtStaff2;
    uint32           uiMstRtClaw;
    uint32           uiMstRtDSword;
    uint32           uiMstPlTmp;
    uint32           uiMstPlSword1;
    uint32           uiMstPlHammer1;
    uint32           uiMstPlSword2;
    uint32           uiMstPlAxe2;
    uint32           uiMstPlMace1;
    uint32           uiMstPlBow2;
    uint32           uiMstPlCrossBow2;
    uint32           uiMstPlWand2;
    uint32           uiMstPlStaff2;
    uint32           uiMstPlClaw;
    uint32           uiMstPlDSword;
    uint32           uiSPRecover;
    uint32           uiTB;
    uint32           uiMaxSP;
    uint32           uiMaxLP;
    uint32           uiIntel;
    uint32           uiCastingTime;
    uint16           uiMACriRate;
    uint32           uiWCRateUp;
    uint32           uiMARateUp;
    uint16           uiHpDownDamegeUp;
    uint16           uiDownDamegeHp;
    uint16           uiHpDownAcUp;
    uint16           uiDownAcHp;
    uint16           uiMoveTBUpPlus;
    uint16           uiHealUPRate;
    uint16           uiKeepTimeBuffUPRate;
    uint16           uiCriDmgUpRate;
    uint32           uiActiveSkillGroupInx;
    uint16           uiDMG_MinusRate;
};

class PassiveSkillShn {
public:
    static PassiveSkillShn& Get();
    void Load();
    const PassiveSkillRow* FindById(uint32 uiID) const;
    const PassiveSkillRow* FindByInx(const std::string& rInx) const;
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<PassiveSkillRow>& Rows() const { return m_kRows; }
private:
    std::vector<PassiveSkillRow>         m_kRows;
    std::map<uint32,      size_t>  m_kById;
    std::map<std::string, size_t>  m_kByInx;
};

} // namespace shine
#endif // SHINE_DATAREADER_SHN_PASSIVESKILL_H
