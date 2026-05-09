// Server/DataReader/TableScriptFile.h
// 02 DataReader -- generic parser for the "#Table / #ColumnType / #ColumnName /
// #Record / #recordin / #End" text format used throughout the supplied data
// archive (World/, Script/, MobAttackSequence/, MobRegen/, MobRoam/,
// MobSetting/Action/, NPCItemList/, ChrCommon, ItemDropTable, Quest, etc.).
//
// File layout (based on the actual files in the project owner's Shine.zip):
//   ; comment line
//   #ignore       \o042                    ; characters to drop entirely
//   #exchange     #   \x20                 ; replace # with space before split
//   #delimiter    \x20                     ; whitespace is the field delimiter
//   #Table        <TableName>
//   #ColumnType   <type1> <type2> ...
//   #ColumnName   <name1> <name2> ...
//   #Record       <field1> <field2> ...
//   #recordin     <TableName> <field1> ... ; explicit-table record line
//   #End
//
// One file may declare multiple #Table blocks. The parser keeps every table by
// declared name and exposes typed field accessors (string / int64 / double).
//
// EVIDENCE: DATA_CONFIRMED  source: project-owner-supplied Shine.zip
//                                   (World/Field.txt, Script/Event.txt,
//                                    MobRegen/*.txt, ItemDropTable.txt, ...).
#ifndef FIESTA_DATAREADER_TABLESCRIPTFILE_H
#define FIESTA_DATAREADER_TABLESCRIPTFILE_H
#include "../Shared/ShineTypes.h"
#include <map>
#include <string>
#include <vector>

namespace fiesta {

// Declared column type enum -- mirrors the canonical token vocabulary observed
// in the supplied files. The interpretation is "wide enough to carry the
// value"; clients query as int64 / string / double.
enum TsFieldType {
    TS_T_UNKNOWN = 0,
    TS_T_BYTE,        // BYTE, byte
    TS_T_WORD,        // WORD, word
    TS_T_DWORD,       // DWORD, dword, DWRD, dwrd
    TS_T_INDEX,       // INDEX (32-bit id token, often a string id)
    TS_T_STRING,      // STRING / String / string (with optional [len])
    TS_T_FLOAT        // FLOAT, single, double
};

struct TsColumn {
    std::string  kName;
    TsFieldType  eType;
    int          iLen;        // STRING[N] -> N (else 0)
};

struct TsRecord {
    std::vector<std::string> kCells;   // raw text per column
};

struct TsTable {
    std::string             kName;
    std::vector<TsColumn>   kColumns;
    std::vector<TsRecord>   kRecords;

    int          ColIndex(const char* szName) const;
    bool         GetCell(size_t row, size_t col, std::string& rOut) const;
    bool         GetCell(size_t row, const char* szCol, std::string& rOut) const;
    int64        GetInt (size_t row, const char* szCol, int64 iDefault = 0) const;
    double       GetReal(size_t row, const char* szCol, double dDefault = 0.0) const;
    std::string  GetStr (size_t row, const char* szCol, const char* szDefault = "") const;
};

class TableScriptFile {
public:
    TableScriptFile();

    // Load and parse a text file from disk.
    bool Load(const std::string& rPath);

    // Total number of declared tables (after Load).
    size_t      Count() const { return m_kTables.size(); }
    // Lookup by table name; returns NULL if absent.
    const TsTable* Find(const std::string& rName) const;
    // Direct-by-index access.
    const TsTable& At(size_t i) const { return m_kTables[i]; }
    const std::vector<TsTable>& All() const { return m_kTables; }

private:
    // Parser state (resets per file).
    std::vector<TsTable>     m_kTables;
    std::map<std::string,size_t> m_kIndex;

    // #ignore / #exchange / #delimiter accumulators (apply to all later lines).
    std::vector<char>        m_kIgnore;        // characters to delete from raw line
    std::vector<std::pair<std::string,std::string> > m_kExchange; // textual replace
    std::string              m_kDelim;         // active delimiter set (default WS)

    // Current "active" table for #Record / lone field lines (some files lack
    // explicit #Table headers and rely on the most-recent declaration).
    std::string              m_kCurTable;

    // Helpers.
    static std::string ResolveEscape(const std::string& rTok);
    static void Trim(std::string& s);
    static void SplitFields(const std::string& rLine, const std::string& rDelim,
                            std::vector<std::string>& rOut);
    static TsFieldType  ParseType(const std::string& rTok, int& rLenOut);

    bool ApplyDirective(const std::string& rDir, const std::vector<std::string>& rArgs);
    bool BeginTable(const std::string& rName);
    bool ApplyColumnTypes(const std::vector<std::string>& rArgs);
    bool ApplyColumnNames(const std::vector<std::string>& rArgs);
    bool AppendRecord(const std::string& rTableName, const std::vector<std::string>& rFields);
};

// Pass 1.26 audit hooks for TS-format files. Every TsTable::GetCell-by-
// name call stamps (table, column) into a static set; `TsAudit_VisitTable`
// is invoked from a per-loader walker at boot end to warn for any
// declared column that was never read.
void TsAudit_Record        (const std::string& rTable, const std::string& rCol);
void TsAudit_Reset         ();
void TsAudit_RegisterLoaded(const TsTable& rT);
void TsAudit_VisitTable    (const TsTable& rT);
void TsAudit_EmitReport    ();

} // namespace fiesta
#endif
