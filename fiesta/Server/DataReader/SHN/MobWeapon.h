// Server/DataReader/SHN/MobWeapon.h
// Auto-generated: one-file-per-SHN split for MobWeapon.shn
#ifndef FIESTA_DATAREADER_SHN_MOBWEAPON_H
#define FIESTA_DATAREADER_SHN_MOBWEAPON_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace fiesta {

struct MobWeaponRow {
    uint32           uiID;
    std::string      kInxName;
    std::string      kSkill;
    uint16           uiAtkSpd;
    uint16           uiBlastRate;
    uint16           uiAtkDly;
    uint16           uiSwingTime;
    uint16           uiHitTime;
    uint32           uiAtkType;
    uint32           uiMinWC;
    uint32           uiMaxWC;
    uint16           uiTH;
    uint32           uiMinMA;
    uint32           uiMaxMA;
    uint16           uiMH;
    uint16           uiRange;
    uint32           uiMopAttackTarget;
    uint32           uiHitType;
    std::string      kStaName;
    uint16           uiStaStrength;
    uint16           uiStaRate;
    uint16           uiAggroInitialize;
};

class MobWeaponShn {
public:
    static MobWeaponShn& Get();
    void Load();
    const MobWeaponRow* FindById(uint32 uiID) const;
    const MobWeaponRow* FindByInx(const std::string& rInx) const;
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<MobWeaponRow>& Rows() const { return m_kRows; }
private:
    std::vector<MobWeaponRow>         m_kRows;
    std::map<uint32,      size_t>  m_kById;
    std::map<std::string, size_t>  m_kByInx;
};

} // namespace fiesta
#endif // FIESTA_DATAREADER_SHN_MOBWEAPON_H
