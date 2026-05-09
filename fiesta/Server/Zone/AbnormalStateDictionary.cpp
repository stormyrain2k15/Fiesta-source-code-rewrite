// Server/Zone/AbnormalStateDictionary.cpp
// Name -> AbState id lookup (used by /abstate admin commands and Lua).
#include "../DataReader/ShnRegistry.h"
#include <map>
#include <string>
namespace fiesta { namespace {
class AbnormalStateDictionary {
public:
    static AbnormalStateDictionary& Get() { static AbnormalStateDictionary s; return s; }
    bool Load() {
        const ShnFile* pkT = ShnRegistry::Get().GetTable("AbState");
        if (!pkT) return false;
        for (uint32 i = 0; i < pkT->RecordCount(); ++i) {
            std::string n = ShnGetStr(*pkT, i, "InxName");
            int32 id = ShnGetI32(*pkT, i, "ID");
            m_kByName[n] = (uint32)id;
        }
        return true;
    }
    uint32 Lookup(const std::string& rName) const {
        std::map<std::string, uint32>::const_iterator it = m_kByName.find(rName);
        return (it == m_kByName.end()) ? 0 : it->second;
    }
private:
    std::map<std::string, uint32> m_kByName;
};
}} // anonymous
