// Server/Zone/MobResistTable.h
// elemental + damage-type resist lookup per mob species.
// Source: MobResist.shn.
#ifndef FIESTA_ZONE_MOBRESISTTABLE_H
#define FIESTA_ZONE_MOBRESISTTABLE_H
#include "BattleStat.h"
#include <map>
#include <string>

namespace fiesta {

struct MobResistRow {
    uint16 uiMobID;                 // links to MobInfo.shn
    int32  aEleResist[ELEMENT_MAX]; // 0..100 (% reduction)
    int32  iPhysResist;
    int32  iMagicResist;
    int32  iRangedResist;
    // Status resists matching the names actually in MobResist (from
    // AIScript/Defense.lua references): ResDot/ResStun/ResMoveSpeed/ResFear/
    // ResBinding/ResReverse/ResMesmerize/ResSeverBone/ResKnockBack/ResTBMinus.
    int32  iResDot, iResStun, iResMoveSpeed, iResFear, iResBinding;
    int32  iResReverse, iResMesmerize, iResSeverBone, iResKnockBack, iResTBMinus;
};

class MobResistTable {
public:
    static MobResistTable& Get();
    bool Load(const std::string& rRoot);

    const MobResistRow* Find(uint16 uiMobID) const;
    int32 GetElementResist(uint16 uiMobID, eElement eElem) const;
    int32 GetDamageTypeResist(uint16 uiMobID, int32 nDmgTypeFlags) const;

private:
    MobResistTable() {}
    std::map<uint16, MobResistRow> m_kRows;
};

} // namespace fiesta
#endif
