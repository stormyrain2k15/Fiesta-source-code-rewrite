// Server/DataReader/SHN/MoverItem.h
// Auto-generated: one-file-per-SHN split for MoverItem.shn
#ifndef SHINE_DATAREADER_SHN_MOVERITEM_H
#define SHINE_DATAREADER_SHN_MOVERITEM_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace shine {

struct MoverItemRow {
    std::string      kMoverIDX;
    std::string      kItemIDX;
};

class MoverItemShn {
public:
    static MoverItemShn& Get();
    void Load();
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<MoverItemRow>& Rows() const { return m_kRows; }
private:
    std::vector<MoverItemRow>         m_kRows;
};

} // namespace shine
#endif // SHINE_DATAREADER_SHN_MOVERITEM_H
