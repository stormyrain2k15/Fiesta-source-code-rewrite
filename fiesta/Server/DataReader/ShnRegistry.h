// Server/DataReader/ShnRegistry.h
// universal SHN loader.
// Owns one parsed `ShnFile` per logical name. Every other system that needs
// data from any of the 199 .shn files in `Data/Shine/` calls
// `ShnRegistry::Get().GetTable("Foo")` and either reads rows directly or
// hands the result to a typed group accessor (`MobTables`, `ItemTables`,
// `SkillTables`, ...) for column-mapped lookup.
// Loading is one-shot at boot. The registry walks the supplied root, parses
// every `*.shn`, and caches the result in-memory. The original game shipped
// ~25 MB of SHN data; in-process residency is fine.
//                                         intended" (2026-02 user note).
#ifndef SHINE_DATAREADER_SHNREGISTRY_H
#define SHINE_DATAREADER_SHNREGISTRY_H
#include "ShnFile.h"
#include <map>

namespace shine {

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

    // Tooling guard: returns true if the SHN logical name (file stem,
    // case-insensitive) is a quest/scenario file. Generic CSV exporters,
    // audit parsers, and SchemaGen MUST consult this before touching the
    // file -- quest/PineScript SHNs use a dedicated on-disk shape and
    // must not be parsed by the generic ShnFile loader. The runtime
    // server still LOADS them via EnumerateShn, but they're tagged as
    // "quest deferred" placeholders for the dedicated quest loader to
    // pick up.
    static bool IsQuestShn(const std::string& rStem);

    // Tooling guard: returns true if the SHN logical name is a
    // CLIENT-ONLY view table (everything under Shine\View\*View*.shn).
    // The NA2016 server intentionally never consumes these -- skill
    // icons, mob nameplates, item tooltips, minimap labels, etc. are
    // pure client-side display data. The registry still loads them so
    // generic tooling (CSV export, schema dumper) can introspect, but
    // the boot-time "unowned table" audit treats them as expected
    // unread tables instead of warnings.
    static bool IsClientViewShn(const std::string& rStem);

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

// ----- Column auditor ------------------------------------------
// Hooked transparently inside `GetTable()` and `ShnGet*`. Call EmitReport
// at the end of boot to log each column the binders never read.
void ShnAudit_BeginTable(const std::string& rT);
void ShnAudit_RecordRead(const std::string& rC);
void ShnAudit_EmitReport(const ShnRegistry& rReg);

} // namespace shine
#endif
