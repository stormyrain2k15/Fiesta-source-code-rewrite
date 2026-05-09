// Server/DataReader/ShnFile.h
// real SHN binary reader. Format derived from the SHN_Editor_4.7
// reference implementation provided by the project owner. Original tool is
// MIT-style developer-shared; this header expresses the same wire format in
// fresh C++ code, with the same type codes (1/2/3/5/9/11/12/13/16/18/20/21/22/24/26/27)
// and the same symmetric XOR-rolling decrypt.
// File frame:
//   [0x20 crypt header (passthrough)] [uint32 totalLen] [encrypted body of (totalLen - 0x24) bytes]
// Decrypted body:
//   [uint32 Header] [uint32 RecordCount] [uint32 DefaultRecordLength] [uint32 ColumnCount]
//   for each column: [0x30 name (zero-padded)] [uint32 Type] [int32 Length]
//   for each record:  [uint16 rowSize] [columns concatenated per Type]
#ifndef FIESTA_DATAREADER_SHNFILE_H
#define FIESTA_DATAREADER_SHNFILE_H
#include "../Shared/ShineTypes.h"
#include <vector>
#include <string>

namespace fiesta {

enum ShnType {
    SHN_T_U8       = 1,
    SHN_T_U16      = 2,
    SHN_T_U32      = 3,
    SHN_T_F32      = 5,
    SHN_T_STR_FIX1 = 9,
    SHN_T_U32_B    = 11,
    SHN_T_U8_B     = 12,
    SHN_T_I16      = 13,
    SHN_T_U8_C     = 0x10,
    SHN_T_U32_C    = 0x12,
    SHN_T_I8       = 20,
    SHN_T_I16_B    = 0x15,
    SHN_T_I32      = 0x16,
    SHN_T_STR_FIX2 = 0x18,
    SHN_T_STR_VAR  = 0x1A,   // null-terminated, variable length
    SHN_T_U32_D    = 0x1B
};

struct ShnColumn {
    std::string kName;
    uint32      uiType;
    int32       iLength;
};

// One field value, tagged by type code. Strings live in kStr; numeric in iVal/fVal.
struct ShnValue {
    uint32      uiType;
    int64       iVal;     // signed/unsigned numerics promoted into 64-bit slot
    float       fVal;
    std::string kStr;
};

class ShnFile {
public:
    ShnFile();

    // Decrypt + parse one .shn file from disk. Returns false on IO/format error.
    bool LoadFromFile(const std::string& rPath);
    // Decrypt + parse from an in-memory blob already containing the on-disk frame
    // (i.e. crypt header + length + body). Used when loading from a pak.
    bool LoadFromMemory(const uint8* p, size_t n);

    uint32 Header()        const { return m_uiHeader; }
    uint32 RecordCount()   const { return (uint32)m_kRows.size(); }
    uint32 RecordLength()  const { return m_uiDefaultRecLen; }
    const std::vector<ShnColumn>&            Columns() const { return m_kColumns; }
    const std::vector<std::vector<ShnValue> >& Rows()    const { return m_kRows; }

    // Convenience: render rows as vectors of strings (column index aligned).
    void ExportAsStringRows(std::vector<std::vector<std::string> >& rOut) const;

    // Quest/scenario SHNs use a dedicated on-disk shape. EnumerateShn
    // tags them at boot via this entry point so the dedicated quest
    // loader (Server/DataReader/QuestShnReader.{h,cpp} / consumed by
    // Server/Zone/QuestSystem) can pick the file up later. A quest-
    // deferred ShnFile carries no rows or columns; calling code must
    // check IsQuestDeferred() before treating it as a generic table.
    void               MarkAsQuestDeferred(const std::string& rPath);
    bool               IsQuestDeferred()  const { return m_bQuestDeferred; }
    const std::string& QuestDeferredPath() const { return m_kQuestPath; }

    // Symmetric XOR-rolling crypt (encrypt and decrypt are the same operation).
    static void Crypt(uint8* p, size_t n);

private:
    bool ParseDecrypted(const uint8* p, size_t n);

    uint8                                       m_aCryptHeader[32];
    uint32                                      m_uiHeader;
    uint32                                      m_uiDefaultRecLen;
    std::vector<ShnColumn>                      m_kColumns;
    std::vector<std::vector<ShnValue> >         m_kRows;
    bool                                        m_bQuestDeferred;
    std::string                                 m_kQuestPath;
};

} // namespace fiesta
#endif
