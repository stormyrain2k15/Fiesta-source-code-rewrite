// Server/Zone/ScenarioScript.h
// 23 -- runtime wrapper for ScenarioBookShelf/<Cat>/*.ps. Same DSL as the mob
// behavior scripts, but used to drive promote / KQ / wedding / instance-dungeon
// scenarios. Verbs include: doorbuild, doorclose, dooropen, scriptfile,
// waitlogin, findhandle, call, npcchat, broadcast, linkto, pause sec N,
// effectobj, itemcreate, questresult, break, mobregen, etc.
//
// EVIDENCE: DATA_CONFIRMED  source: project-owner-supplied ScenarioBookShelf/.
#ifndef FIESTA_ZONE_SCENARIOSCRIPT_H
#define FIESTA_ZONE_SCENARIOSCRIPT_H
#include "../Shared/ShineTypes.h"
#include "../DataReader/PsScriptFile.h"
#include <map>
#include <string>

namespace fiesta {

class ScenarioScript {
public:
    bool   Load(const std::string& rPath);
    const  PsScriptFile& File() const { return m_kFile; }
    size_t BlockCount() const { return m_kFile.BlockCount(); }

private:
    PsScriptFile m_kFile;
};

class ScenarioBookShelf {
public:
    static ScenarioBookShelf& Get();
    const ScenarioScript* Load(const std::string& rRoot,
                               const std::string& rCategory,    // ID/Promote/Wedding/...
                               const std::string& rName);
    const ScenarioScript* Find(const std::string& rKey) const;
    void Clear();
private:
    std::map<std::string, ScenarioScript*> m_kAll;
};

} // namespace fiesta
#endif
