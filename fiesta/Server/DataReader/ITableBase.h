// Server/DataReader/ITableBase.h
// 02 DataReader -- common base for every *Tab table loader. Original symbol pattern:
//   ITableBase<T>::Load / ::DataProcess / ::BeforeTerminate / ::GetTotal / ::ms_pkTable
// EVIDENCE: PDB_CONFIRMED  symbol: ITableBase<*>
#ifndef FIESTA_ITABLEBASE_H
#define FIESTA_ITABLEBASE_H
#include "../Shared/ShineTypes.h"
#include <map>
#include <vector>

namespace fiesta {

class DataReader;

template <class TRecord>
class ITableBase {
public:
    typedef TRecord Record;

    ITableBase() : m_uiTotal(0) {}
    virtual ~ITableBase() { BeforeTerminate(); }

    // Load the SHN/TXT identified by szLogicalName via DataReader. Subclasses
    // override DataProcess() to per-row decode.
    bool   Load(DataReader& rReader, const char* szLogicalName);
    virtual void DataProcess(const std::vector<std::string>& rRow) {} // override in subclasses
    virtual void BeforeTerminate() { m_kRows.clear(); m_kIndex.clear(); m_uiTotal = 0; }

    uint32 GetTotal() const { return m_uiTotal; }
    const TRecord* Find(uint32 uiKey) const {
        typename std::map<uint32, size_t>::const_iterator it = m_kIndex.find(uiKey);
        return (it == m_kIndex.end()) ? NULL : &m_kRows[it->second];
    }
    const std::vector<TRecord>& Rows() const { return m_kRows; }

    static ITableBase<TRecord>* ms_pkTable;  // matches PDB pattern
protected:
    void Push(uint32 uiKey, const TRecord& rRec) {
        m_kIndex[uiKey] = m_kRows.size();
        m_kRows.push_back(rRec);
        ++m_uiTotal;
    }
    std::vector<TRecord>     m_kRows;
    std::map<uint32, size_t> m_kIndex;
    uint32                   m_uiTotal;
};

template <class TRecord> ITableBase<TRecord>* ITableBase<TRecord>::ms_pkTable = NULL;

} // namespace fiesta
#endif
