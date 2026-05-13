// Server/DataReader/CommonData.h
// template aliases mirroring PDB names. Kept separate from DataReader.h to
// avoid pulling Schemas-using TUs into the header chain.
#ifndef SHINE_DATAREADER_COMMONDATA_H
#define SHINE_DATAREADER_COMMONDATA_H
#include "ITableBase.h"
namespace shine {
template <class T> class CommonData        : public ITableBase<T> {};
template <class T> class IndexedCommonData : public ITableBase<T> {};
} // namespace shine
#endif
