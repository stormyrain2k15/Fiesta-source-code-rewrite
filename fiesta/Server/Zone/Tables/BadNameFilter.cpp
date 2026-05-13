// Server/Zone/Tables/BadNameFilter.cpp
// FEATURE: world-creation -- BadNameFilter.shn binder.
// Substring-match blocklist for character/guild names.
#include "BindMacros.h"
#include "../MiscTables.h"

namespace shine {

BadNameFilter& BadNameFilter::Get() { static BadNameFilter s; return s; }

void BadNameFilter::Bind() {
    // FEATURE: world-creation -- column read: Pattern
    if (const ShnFile* t = ShnRegistry::Get().GetTable("BadNameFilter")) {
        m_kPatterns.reserve(t->Rows().size());
        for (size_t i = 0; i < t->Rows().size(); ++i)
            m_kPatterns.push_back(ShnGetStr(*t, i, "Pattern"));
        SHINELOG_INFO("BadNameFilter: %u patterns", (uint32)m_kPatterns.size());
    }
}

bool BadNameFilter::IsBlocked(const std::string& rName) const {
    for (size_t i = 0; i < m_kPatterns.size(); ++i) {
        const std::string& p = m_kPatterns[i];
        if (p.empty()) continue;
        if (rName.find(p) != std::string::npos) return true;
    }
    return false;
}

} // namespace shine
