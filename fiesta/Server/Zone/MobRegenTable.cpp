// Server/Zone/MobRegenTable.cpp
#include "MobRegenTable.h"
#include "../DataReader/TableScriptFile.h"
#include "../Shared/ShineLogSystem.h"

namespace shine {

bool MobRegenTable::Load(const std::string& rPath) {
    m_kGroups.clear(); m_kRegens.clear();
    TableScriptFile f;
    if (!f.Load(rPath)) return false;
    if (const TsTable* t = f.Find("MobRegenGroup")) {
        for (size_t r = 0; r < t->kRecords.size(); ++r) {
            MobRegenGroupRow g;
            g.kGroupIndex   = t->GetStr(r, "GroupIndex");
            g.kIsFamily     = t->GetStr(r, "IsFamily");
            g.uiCenterX     = (uint32)t->GetInt(r, "CenterX");
            g.uiCenterY     = (uint32)t->GetInt(r, "CenterY");
            g.uiWidth       = (uint32)t->GetInt(r, "Width");
            g.uiHeight      = (uint32)t->GetInt(r, "Height");
            g.uiRangeDegree = (uint32)t->GetInt(r, "RangeDegree");
            m_kGroups.push_back(g);
        }
    }
    if (const TsTable* t = f.Find("MobRegen")) {
        for (size_t r = 0; r < t->kRecords.size(); ++r) {
            MobRegenRow x;
            x.kRegenIndex  = t->GetStr(r, "RegenIndex");
            x.kMobIndex    = t->GetStr(r, "MobIndex");
            x.uiMobNum     = (uint8) t->GetInt(r, "MobNum");
            x.uiKillNum    = (uint8) t->GetInt(r, "KillNum");
            x.uiRegStandard= (uint32)t->GetInt(r, "RegStandard");
            x.uiRegMin     = (uint32)t->GetInt(r, "RegMin");
            x.uiRegMax     = (uint32)t->GetInt(r, "RegMax");
            char buf[16];
            for (int i = 0; i < 6; ++i) {
                sprintf_s(buf, sizeof(buf), "RegDelta%d", i); x.aDelta[i] = (int32)t->GetInt(r, buf);
                sprintf_s(buf, sizeof(buf), "RegSec%d",   i); x.aSec  [i] = (int32)t->GetInt(r, buf);
            }
            m_kRegens.push_back(x);
        }
    }
    return true;
}

MobRegenBox& MobRegenBox::Get() { static MobRegenBox s; return s; }

const MobRegenTable* MobRegenBox::Load(const std::string& rRoot, const std::string& rMapId) {
    std::string path = rRoot + "\\MobRegen\\" + rMapId + ".txt";
    MobRegenTable* pk = new MobRegenTable();
    if (!pk->Load(path)) { delete pk; return NULL; }
    m_kAll[rMapId] = pk;
    SHINELOG_DEBUG("MobRegen '%s' groups=%u regens=%u", rMapId.c_str(),
                   (uint32)pk->Groups().size(), (uint32)pk->Regens().size());
    return pk;
}
const MobRegenTable* MobRegenBox::Find(const std::string& rMapId) const {
    std::map<std::string, MobRegenTable*>::const_iterator it = m_kAll.find(rMapId);
    return (it == m_kAll.end()) ? NULL : it->second;
}
void MobRegenBox::Clear() {
    for (std::map<std::string, MobRegenTable*>::iterator it = m_kAll.begin(); it != m_kAll.end(); ++it)
        delete it->second;
    m_kAll.clear();
}

} // namespace shine
