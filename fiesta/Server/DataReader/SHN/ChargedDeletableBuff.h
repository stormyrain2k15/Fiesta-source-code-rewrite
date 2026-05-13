// Server/DataReader/SHN/ChargedDeletableBuff.h
// Auto-generated: one-file-per-SHN split for ChargedDeletableBuff.shn
#ifndef SHINE_DATAREADER_SHN_CHARGEDDELETABLEBUFF_H
#define SHINE_DATAREADER_SHN_CHARGEDDELETABLEBUFF_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace shine {

struct ChargedDeletableBuffRow {
    std::string      kCDI_IDX;
};

class ChargedDeletableBuffShn {
public:
    static ChargedDeletableBuffShn& Get();
    void Load();
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<ChargedDeletableBuffRow>& Rows() const { return m_kRows; }
private:
    std::vector<ChargedDeletableBuffRow>         m_kRows;
};

} // namespace shine
#endif // SHINE_DATAREADER_SHN_CHARGEDDELETABLEBUFF_H
