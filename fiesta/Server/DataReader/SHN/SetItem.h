// Server/DataReader/SHN/SetItem.h
// Auto-generated: one-file-per-SHN split for SetItem.shn
#ifndef SHINE_DATAREADER_SHN_SETITEM_H
#define SHINE_DATAREADER_SHN_SETITEM_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace shine {

struct SetItemRow {
    std::string      kIndex;
    uint8            uiPiece;
    std::string      kEffect;
};

class SetItemShn {
public:
    static SetItemShn& Get();
    void Load();
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<SetItemRow>& Rows() const { return m_kRows; }
private:
    std::vector<SetItemRow>         m_kRows;
};

} // namespace shine
#endif // SHINE_DATAREADER_SHN_SETITEM_H
