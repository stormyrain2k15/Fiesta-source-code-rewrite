// Server/DataReader/SHN/ErrorCodeTable.cpp
// Auto-generated: one-file-per-SHN split for ErrorCodeTable.shn
#include "ErrorCodeTable.h"
#include "../../Shared/ShineLogSystem.h"

namespace shine {

ErrorCodeTableShn& ErrorCodeTableShn::Get() { static ErrorCodeTableShn s; return s; }

void ErrorCodeTableShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("ErrorCodeTable");
    if (!t) { SHINELOG_WARN("ErrorCodeTable.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        ErrorCodeTableRow rec;
        rec.uiErrorCode = ShnGetU32(*t, _r, "ErrorCode");
        rec.kErrorMessageIndex = ShnGetStr(*t, _r, "ErrorMessageIndex");
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("ErrorCodeTable.shn: %u rows", (uint32)m_kRows.size());
}

} // namespace shine
