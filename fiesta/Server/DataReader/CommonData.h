// Server/DataReader/CommonData.h
// 02 -- template aliases mirroring PDB names. Kept separate from DataReader.h to
// avoid pulling Schemas-using TUs into the header chain.
// EVIDENCE: PDB_CONFIRMED  symbol: CommonData<T>, IndexedCommonData<T>
#ifndef FIESTA_DATAREADER_COMMONDATA_H
#define FIESTA_DATAREADER_COMMONDATA_H
#include "ITableBase.h"
namespace fiesta {
template <class T> class CommonData        : public ITableBase<T> {};
template <class T> class IndexedCommonData : public ITableBase<T> {};
} // namespace fiesta
#endif
