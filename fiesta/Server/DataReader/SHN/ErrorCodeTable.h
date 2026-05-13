// Server/DataReader/SHN/ErrorCodeTable.h
// Auto-generated: one-file-per-SHN split for ErrorCodeTable.shn
#ifndef SHINE_DATAREADER_SHN_ERRORCODETABLE_H
#define SHINE_DATAREADER_SHN_ERRORCODETABLE_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace shine {

struct ErrorCodeTableRow {
    uint32           uiErrorCode;
    std::string      kErrorMessageIndex;
};

class ErrorCodeTableShn {
public:
    static ErrorCodeTableShn& Get();
    void Load();
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<ErrorCodeTableRow>& Rows() const { return m_kRows; }
private:
    std::vector<ErrorCodeTableRow>         m_kRows;
};

} // namespace shine
#endif // SHINE_DATAREADER_SHN_ERRORCODETABLE_H
