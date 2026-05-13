// Server/Zone/RecallPointTable.cpp
// Recall-point registry. Each recall point is a (map, x, y) you can teleport
// to; goddess-wing items (SparrowWing) port by recall-point id.
#include "../DataReader/ShnRegistry.h"
#include <map>
namespace shine {
struct RecallPoint { uint32 uiId; uint16 uiMap; float x, y; };
class RecallPointTable {
public:
    static RecallPointTable& Get() { static RecallPointTable s; return s; }
    bool Load() {
        const ShnFile* pkT = ShnRegistry::Get().GetTable("RecallPoint");
        if (!pkT) return false;
        for (uint32 i = 0; i < pkT->RecordCount(); ++i) {
            RecallPoint p;
            p.uiId  = (uint32)ShnGetI32(*pkT, i, "ID");
            p.uiMap = (uint16)ShnGetI32(*pkT, i, "Map");
            p.x     = (float) ShnGetI32(*pkT, i, "X");
            p.y     = (float) ShnGetI32(*pkT, i, "Y");
            m_kRows[p.uiId] = p;
        }
        return true;
    }
    const RecallPoint* Find(uint32 uiId) const {
        std::map<uint32, RecallPoint>::const_iterator it = m_kRows.find(uiId);
        return (it == m_kRows.end()) ? NULL : &it->second;
    }
private:
    std::map<uint32, RecallPoint> m_kRows;
};
} // namespace shine
