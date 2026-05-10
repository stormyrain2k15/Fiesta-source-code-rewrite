// Server/Zone/Tables/PineScriptTable.cpp
// FEATURE: world-creation -- World/PineScript.txt binder.
// PineScript is a list of script paths (Cat/Name or Cat/Sub/Name) that
// the runtime walks at boot to register every scenario script with
// ScenarioBookShelf. Wedding and Guild ceremonies are always-on so
// they're appended unconditionally.
#include "../WorldTables.h"
#include "../ScenarioScript.h"
#include "../../DataReader/TableScriptFile.h"

namespace fiesta {

PineScriptTable& PineScriptTable::Get() { static PineScriptTable s; return s; }

bool PineScriptTable::Load(const std::string& rRoot) {
    m_kScripts.clear();
    TableScriptFile f;
    if (!f.Load(rRoot + "\\World\\PineScript.txt")) return false;
    const TsTable* t = f.Find("PineScript"); if (!t) return false;
    // FEATURE: world-creation -- column read: ScriptName
    for (size_t r = 0; r < t->kRecords.size(); ++r) {
        std::string s = t->GetStr(r, "ScriptName");
        if (!s.empty()) m_kScripts.push_back(s);
    }
    return true;
}

// Helper used at boot: load PineScript.txt then dispatch every entry
// through ScenarioBookShelf::Load. Lives here (not as a member of
// PineScriptTable) because the helper composes two systems.
size_t LoadAllPineScripts(const std::string& rRoot) {
    PineScriptTable::Get().Load(rRoot);
    const std::vector<std::string>& list = PineScriptTable::Get().Scripts();
    size_t loaded = 0;
    for (size_t i = 0; i < list.size(); ++i) {
        const std::string& s = list[i];
        size_t slash = s.find('/');
        if (slash == std::string::npos) continue;
        std::string cat  = s.substr(0, slash);
        std::string tail = s.substr(slash + 1);
        size_t s2 = tail.rfind('/');
        std::string name = (s2 == std::string::npos) ? tail : tail.substr(s2 + 1);
        std::string subcat = (s2 == std::string::npos) ? cat : (cat + "\\" + tail.substr(0, s2));
        if (ScenarioBookShelf::Get().Load(rRoot, subcat, name)) ++loaded;
    }
    if (ScenarioBookShelf::Get().Load(rRoot, "Wedding", "Wedding"))                ++loaded;
    if (ScenarioBookShelf::Get().Load(rRoot, "Guild",   "GuildTournament"))        ++loaded;
    if (ScenarioBookShelf::Get().Load(rRoot, "Guild",   "GuildTournament1"))       ++loaded;
    return loaded;
}

} // namespace fiesta
