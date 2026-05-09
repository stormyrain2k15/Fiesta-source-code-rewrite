// Server/DataReader/ShnRegistry.h
// 02 -- universal SHN loader.
//
// Owns one parsed `ShnFile` per logical name. Every other system that needs
// data from any of the 199 .shn files in `Data/Shine/` calls
// `ShnRegistry::Get().GetTable("Foo")` and either reads rows directly or
// hands the result to a typed group accessor (`MobTables`, `ItemTables`,
// `SkillTables`, ...) for column-mapped lookup.
//
// Loading is one-shot at boot. The registry walks the supplied root, parses
// every `*.shn`, and caches the result in-memory. The original game shipped
// ~25 MB of SHN data; in-process residency is fine.
//
// EVIDENCE: PROJECT_REQUIREMENT  source: "use every file in the folders as
//                                         intended" (2026-02 user note).
#ifndef FIESTA_DATAREADER_SHNREGISTRY_H
#define FIESTA_DATAREADER_SHNREGISTRY_H
#include "ShnFile.h"
#include <map>

namespace fiesta {

class ShnRegistry {
public:
    static ShnRegistry& Get();

    // Walks `<rRoot>/Shine/*.shn` (and `<rRoot>/Shine-1/*.shn` if the
    // alternate canonical drop is present) and parses every file. Files
    // that fail to parse are skipped with a warning; the registry stays
    // usable for the rest. Returns the count of successfully loaded SHNs.
    size_t LoadAll(const std::string& rRoot);

    // Direct table access. Logical name is the file stem without the .shn
    // suffix (e.g. "MobInfoServer", "ItemUpgrade"). Returns NULL if the
    // table was never loaded.
    const ShnFile* GetTable(const std::string& rLogicalName) const;

    // Convenience: "<table>.<column>" string addressing.
    // Returns false on missing table / missing column.
    bool ColumnIndex(const std::string& rTable, const std::string& rColumn,
                     uint32& uiOut) const;

    // Iteration helper used by typed group accessors that want to bulk-scan.
    typedef std::map<std::string, ShnFile*>::const_iterator iterator;
    iterator begin() const { return m_kAll.begin(); }
    iterator end()   const { return m_kAll.end();   }
    size_t   size()  const { return m_kAll.size();  }

private:
    ShnRegistry() {}
    ~ShnRegistry();

    std::map<std::string, ShnFile*> m_kAll;
};

// Helper: look up a column by name on a row vector. Returns the stringified
// value or empty if missing.
std::string ShnGetStr(const ShnFile& rTab, size_t uiRow, const std::string& rColumn);
uint32      ShnGetU32(const ShnFile& rTab, size_t uiRow, const std::string& rColumn);
int32       ShnGetI32(const ShnFile& rTab, size_t uiRow, const std::string& rColumn);
float       ShnGetF32(const ShnFile& rTab, size_t uiRow, const std::string& rColumn);

} // namespace fiesta
#endif
