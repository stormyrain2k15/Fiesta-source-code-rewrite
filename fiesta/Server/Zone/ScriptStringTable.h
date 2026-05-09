// Server/Zone/ScriptStringTable.h
// 23 -- typed loader for Script/<Set>.txt (NPC dialog / system message catalogs).
// Each file declares a "Script" table with two columns: ScrIndex (INDEX) and
// ScrString (STRING[128]).
//
// EVIDENCE: DATA_CONFIRMED  source: project-owner-supplied Script/Event.txt etc.
#ifndef FIESTA_ZONE_SCRIPTSTRINGTABLE_H
#define FIESTA_ZONE_SCRIPTSTRINGTABLE_H
#include "../Shared/ShineTypes.h"
#include <map>
#include <string>
#include <vector>

namespace fiesta {

class ScriptStringTable {
public:
    bool   Load(const std::string& rPath);
    bool   Lookup(const std::string& rIndex, std::string& rOut) const;
    size_t Count() const { return m_kEntries.size(); }
private:
    std::map<std::string, std::string> m_kEntries;
};

class ScriptStringBox {
public:
    static ScriptStringBox& Get();
    bool LoadAll(const std::string& rRoot);                        // Script/*.txt
    bool Lookup (const std::string& rIndex, std::string& rOut) const;
    void Clear();
private:
    std::vector<ScriptStringTable*> m_kTables;
    // Flat union map for fast lookup; later files override earlier ones (the
    // game ships per-event scripts that reuse entry names like "SystemMsg_01").
    std::map<std::string, size_t>   m_kFlat;
    std::vector<std::string>        m_kFlatVal;
};

} // namespace fiesta
#endif
