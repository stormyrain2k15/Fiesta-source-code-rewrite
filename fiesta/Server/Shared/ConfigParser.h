// Server/Shared/ConfigParser.h
// typed-record config parser for the original ServerInfo.txt /
// LoginServerInfo.txt / ZoneServerInfo.txt format. Real syntax (from the
// project owner's supplied templates):
//   #DEFINE NAME
//     <STRING>
//     <INTEGER>
//     ...
//   #ENDDEFINE
//   NAME "string", 1, 2, "another"          ; trailing comment
//   #include "../9Data/ServerInfo/ServerInfo.txt"
//   #END
// Lines starting with ';' are comments. Trailing ';' comments on data lines
// are honored. Records may repeat (multiple SERVER_INFO / ODBC_INFO rows).
// LoginServerInfo.txt templates (May 2026).
#ifndef FIESTA_SHARED_CONFIGPARSER_H
#define FIESTA_SHARED_CONFIGPARSER_H
#include "ShineTypes.h"
#include <map>
#include <string>
#include <vector>

namespace fiesta {

enum CfgFieldType { CFT_NONE = 0, CFT_STRING = 1, CFT_INTEGER = 2 };

struct CfgValue {
    CfgFieldType eType;
    std::string  kStr;
    int64        iNum;
    CfgValue() : eType(CFT_NONE), iNum(0) {}
    bool   AsBool() const { return iNum != 0; }
    int32  AsInt32() const { return (int32)iNum; }
    uint16 AsU16()   const { return (uint16)iNum; }
    const std::string& AsStr() const { return kStr; }
};

typedef std::vector<CfgValue> CfgRecord;

class ConfigParser {
public:
    ConfigParser();

    // Loads a file. #include directives resolve relative to the including file.
    bool Load(const std::string& rPath);

    // All records of a given record-name (e.g., "SERVER_INFO", "ODBC_INFO").
    const std::vector<CfgRecord>& Records(const std::string& rName) const;

    // First record of a name, or NULL.
    const CfgRecord* First(const std::string& rName) const;

    // Schema (declared field types per record name) -- diagnostic.
    const std::vector<CfgFieldType>* Schema(const std::string& rName) const;
private:
    bool LoadInternal(const std::string& rPath, std::vector<std::string>& rIncludeStack);
    bool ParseLine(const std::string& rLine, const std::string& rDir,
                   std::vector<std::string>& rIncludeStack);
    bool ParseRecordLine(const std::string& rName, const std::string& rArgs);

    std::map<std::string, std::vector<CfgFieldType> >    m_kSchema;
    std::map<std::string, std::vector<CfgRecord>     >   m_kRecords;
    std::vector<CfgRecord>                                m_kEmpty;
    // Parser state for #DEFINE blocks.
    std::string  m_kCurrentDefine;
    bool         m_bInDefine;
    bool         m_bDoneEnd;
};

} // namespace fiesta
#endif
