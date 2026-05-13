// Server/Zone/MobResistTable.cpp
#include "MobResistTable.h"
#include "../DataReader/ShnFile.h"
#include "../Shared/ShineLogSystem.h"
#include <stdlib.h>

namespace shine {

MobResistTable& MobResistTable::Get() { static MobResistTable s; return s; }

bool MobResistTable::Load(const std::string& rRoot) {
    m_kRows.clear();
    std::string path = rRoot + "\\MobResist.shn";
    ShnFile f;
    if (!f.LoadFromFile(path)) {
        SHINELOG_WARN("MobResist.shn missing -- mobs will take full elemental damage.");
        return false;
    }
    std::vector<std::vector<std::string> > rows;
    f.ExportAsStringRows(rows);
    if (rows.size() < 2) return false;

    // Build column index.
    std::map<std::string, int> col;
    for (size_t c = 0; c < rows[0].size(); ++c) col[rows[0][c]] = (int)c;

    // Helper to read one cell.
    struct H {
        static int32 Get(const std::vector<std::string>& r, int idx, int32 def) {
            if (idx < 0 || (size_t)idx >= r.size() || r[idx].empty() || r[idx] == "-")
                return def;
            return atoi(r[idx].c_str());
        }
        static int Idx(const std::map<std::string,int>& m, const char* k) {
            std::map<std::string,int>::const_iterator it = m.find(k);
            return (it == m.end()) ? -1 : it->second;
        }
    };

    int iId = H::Idx(col, "MobID");
    if (iId < 0) iId = H::Idx(col, "InxNo");
    if (iId < 0) iId = 0;

    const char* eleNames[ELEMENT_MAX] = {
        "", "FireResist", "WaterResist", "EarthResist",
        "WindResist", "LightResist", "DarkResist"
    };
    int iEle[ELEMENT_MAX]; iEle[0] = -1;
    for (int e = 1; e < ELEMENT_MAX; ++e) iEle[e] = H::Idx(col, eleNames[e]);

    int iPhys  = H::Idx(col, "PhysResist");
    int iMag   = H::Idx(col, "MagicResist");
    int iRange = H::Idx(col, "RangedResist");

    int iDot         = H::Idx(col, "ResDot");
    int iStun        = H::Idx(col, "ResStun");
    int iMoveSpd     = H::Idx(col, "ResMoveSpeed");
    int iFear        = H::Idx(col, "ResFear");
    int iBinding     = H::Idx(col, "ResBinding");
    int iReverse     = H::Idx(col, "ResReverse");
    int iMesmerize   = H::Idx(col, "ResMesmerize");
    int iSeverBone   = H::Idx(col, "ResSeverBone");
    int iKnockBack   = H::Idx(col, "ResKnockBack");
    int iTBMinus     = H::Idx(col, "ResTBMinus");

    for (size_t r = 1; r < rows.size(); ++r) {
        const std::vector<std::string>& row = rows[r];
        if (row.empty()) continue;
        LegacyMobResistRow x;
        memset(&x, 0, sizeof(x));
        x.uiMobID = (uint16)H::Get(row, iId, 0);
        for (int e = 1; e < ELEMENT_MAX; ++e) x.aEleResist[e] = H::Get(row, iEle[e], 0);
        x.iPhysResist    = H::Get(row, iPhys,  0);
        x.iMagicResist   = H::Get(row, iMag,   0);
        x.iRangedResist  = H::Get(row, iRange, 0);
        x.iResDot        = H::Get(row, iDot, 0);
        x.iResStun       = H::Get(row, iStun, 0);
        x.iResMoveSpeed  = H::Get(row, iMoveSpd, 0);
        x.iResFear       = H::Get(row, iFear, 0);
        x.iResBinding    = H::Get(row, iBinding, 0);
        x.iResReverse    = H::Get(row, iReverse, 0);
        x.iResMesmerize  = H::Get(row, iMesmerize, 0);
        x.iResSeverBone  = H::Get(row, iSeverBone, 0);
        x.iResKnockBack  = H::Get(row, iKnockBack, 0);
        x.iResTBMinus    = H::Get(row, iTBMinus, 0);
        m_kRows[x.uiMobID] = x;
    }
    SHINELOG_INFO("MobResistTable: %u rows", (uint32)m_kRows.size());
    return true;
}

const LegacyMobResistRow* MobResistTable::Find(uint16 uiMobID) const {
    std::map<uint16, LegacyMobResistRow>::const_iterator it = m_kRows.find(uiMobID);
    return (it == m_kRows.end()) ? NULL : &it->second;
}
int32 MobResistTable::GetElementResist(uint16 uiMobID, eElement eElem) const {
    if (eElem <= ELEMENT_NONE || eElem >= ELEMENT_MAX) return 0;
    const LegacyMobResistRow* p = Find(uiMobID);
    return p ? p->aEleResist[eElem] : 0;
}
int32 MobResistTable::GetDamageTypeResist(uint16 uiMobID, int32 nDmgTypeFlags) const {
    const LegacyMobResistRow* p = Find(uiMobID);
    if (!p) return 0;
    if (nDmgTypeFlags & DMG_TYPE_MAGIC)  return p->iMagicResist;
    if (nDmgTypeFlags & DMG_TYPE_RANGED) return p->iRangedResist;
    return p->iPhysResist;
}

} // namespace shine
