// Server/Zone/SingleDataMap.cpp
// SingleData.shn -- compact key->value table for engine constants
// (NPC menu max items, per-field aggro radius, AbState duration cap,
// recall-delay seconds). Looked up by string key at boot.
#include "../DataReader/ShnRegistry.h"
#include <map>
#include <string>
namespace shine {
class SingleDataMap {
public:
    static SingleDataMap& Get() { static SingleDataMap s; return s; }
    bool Load() {
        const ShnFile* pkT = ShnRegistry::Get().GetTable("SingleData");
        if (!pkT) return false;
        for (uint32 i = 0; i < pkT->RecordCount(); ++i) {
            std::string k = ShnGetStr(*pkT, i, "Name");
            int32       v = ShnGetI32(*pkT, i, "Value");
            m_kVals[k] = v;
        }
        return true;
    }
    int32 Get_(const std::string& rKey, int32 iDef) const {
        std::map<std::string, int32>::const_iterator it = m_kVals.find(rKey);
        return (it == m_kVals.end()) ? iDef : it->second;
    }
private:
    std::map<std::string, int32> m_kVals;
};
} // namespace shine
