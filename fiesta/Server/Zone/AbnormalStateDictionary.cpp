// Server/Zone/AbnormalStateDictionary.cpp
// Real implementation -- formerly a stub in an anonymous namespace.
#include "AbnormalStateDictionary.h"
#include "../DataReader/ShnRegistry.h"
#include "../Shared/ShineLogSystem.h"

namespace fiesta {

AbnormalStateDictionary& AbnormalStateDictionary::Get() {
    static AbnormalStateDictionary s; return s;
}

bool AbnormalStateDictionary::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("AbState");
    if (!t) {
        SHINELOG_WARN("AbnormalStateDictionary: AbState.shn not loaded");
        return false;
    }
    m_kByName.clear();
    for (uint32 i = 0; i < t->RecordCount(); ++i) {
        std::string n  = ShnGetStr(*t, i, "InxName");
        int32       id = ShnGetI32(*t, i, "ID");
        if (!n.empty()) m_kByName[n] = (uint32)id;
    }
    SHINELOG_INFO("AbnormalStateDictionary: loaded %u entries",
                  (uint32)m_kByName.size());
    return true;
}

uint32 AbnormalStateDictionary::Lookup(const std::string& rName) const {
    std::map<std::string, uint32>::const_iterator it = m_kByName.find(rName);
    return (it == m_kByName.end()) ? 0 : it->second;
}

} // namespace fiesta
