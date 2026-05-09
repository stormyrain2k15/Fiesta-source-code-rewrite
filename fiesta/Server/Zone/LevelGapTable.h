// Server/Zone/LevelGapTable.h
// 12 -- loads & queries the three level-gap damage modifier tables from SHN:
//   damagelvgappve.shn   (player -> mob)
//   damagelvgappvp.shn   (player -> player)
//   damagelvgapevp.shn   (mob -> player)
// Each table maps level difference -> multiplier (x1000). Table is symmetric
// around gap = 0 (1000 = 1.0x = no change).
//
// EVIDENCE: DATA_CONFIRMED  source: F2/Battle/levelgapdamagetable.h shape.
#ifndef FIESTA_ZONE_LEVELGAPTABLE_H
#define FIESTA_ZONE_LEVELGAPTABLE_H
#include "BattleStat.h"
#include "BattleTunables.h"

namespace fiesta {

const int kLvGapTableSize = 200;       // -100..+99
const int kLvGapOffset    = 100;

class LevelGapTable {
public:
    static LevelGapTable& Get();
    bool   Load(const std::string& rRoot);     // root contains "DamageLvGap*.shn"
    int32  GetMultiplier(int32 nAtkLevel, int32 nDefLevel, eLvGapType eType) const;
private:
    LevelGapTable() : m_bLoaded(false) {
        for (int i = 0; i < kLvGapTableSize; ++i) {
            m_aPvE[i] = m_aPvP[i] = m_aEvP[i] = 1000;
        }
    }
    bool   LoadOne(const std::string& rPath, int32* pTable);

    int32 m_aPvE[kLvGapTableSize];
    int32 m_aPvP[kLvGapTableSize];
    int32 m_aEvP[kLvGapTableSize];
    bool  m_bLoaded;
};

} // namespace fiesta
#endif
