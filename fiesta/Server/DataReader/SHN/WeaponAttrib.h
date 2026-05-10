// Server/DataReader/SHN/WeaponAttrib.h
// Auto-generated: one-file-per-SHN split for WeaponAttrib.shn
#ifndef FIESTA_DATAREADER_SHN_WEAPONATTRIB_H
#define FIESTA_DATAREADER_SHN_WEAPONATTRIB_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace fiesta {

struct WeaponAttribRow {
    uint32           uiWeaponType;
    uint16           uiUsableDegree;
    uint8            uiIsUsableInMoving;
    int16            iHitRate;
    int16            iUnkCol4;
    int16            iUnkCol5;
};

class WeaponAttribShn {
public:
    static WeaponAttribShn& Get();
    void Load();
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<WeaponAttribRow>& Rows() const { return m_kRows; }
private:
    std::vector<WeaponAttribRow>         m_kRows;
};

} // namespace fiesta
#endif // FIESTA_DATAREADER_SHN_WEAPONATTRIB_H
