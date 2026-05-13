// Server/Zone/ScenarioScript.cpp
#include "ScenarioScript.h"
#include "../Shared/ShineLogSystem.h"

namespace shine {

bool ScenarioScript::Load(const std::string& rPath) {
    return m_kFile.Load(rPath);
}

ScenarioBookShelf& ScenarioBookShelf::Get() { static ScenarioBookShelf s; return s; }

const ScenarioScript* ScenarioBookShelf::Load(const std::string& rRoot,
                                              const std::string& rCategory,
                                              const std::string& rName) {
    std::string path = rRoot + "\\ScenarioBookShelf\\" + rCategory + "\\" + rName + ".ps";
    ScenarioScript* pk = new ScenarioScript();
    if (!pk->Load(path)) { delete pk; return NULL; }
    std::string key = rCategory + "/" + rName;
    m_kAll[key] = pk;
    SHINELOG_DEBUG("Scenario '%s' blocks=%u", key.c_str(), (uint32)pk->BlockCount());
    return pk;
}

const ScenarioScript* ScenarioBookShelf::Find(const std::string& rKey) const {
    std::map<std::string, ScenarioScript*>::const_iterator it = m_kAll.find(rKey);
    return (it == m_kAll.end()) ? NULL : it->second;
}

void ScenarioBookShelf::Clear() {
    for (std::map<std::string, ScenarioScript*>::iterator it = m_kAll.begin(); it != m_kAll.end(); ++it)
        delete it->second;
    m_kAll.clear();
}

} // namespace shine
