// Server/DataReader/DataBox.h
// runtime registry of every loaded ITableBase<*>. Mirrors PDB symbol DataBox.
#ifndef SHINE_DATABOX_H
#define SHINE_DATABOX_H
#include "../Shared/ShineTypes.h"
#include "DataReader.h"
#include <map>
#include <string>

namespace shine {

class IDataTable {
public:
    virtual ~IDataTable() {}
    virtual bool Load(DataReader& rReader)         = 0;
    virtual void BeforeTerminate()                  = 0;
    virtual uint32 GetTotal() const                 = 0;
    virtual const char* LogicalName() const         = 0;
};

class DataBox {
public:
    static DataBox& Get();
    void   Register(IDataTable* pkTable);
    bool   LoadAll (DataReader& rReader);
    void   Shutdown();
    IDataTable* Find(const char* szLogical);
    size_t Size() const { return m_kAll.size(); }
private:
    std::vector<IDataTable*>          m_kAll;
    std::map<std::string, IDataTable*> m_kByName;
};

// SingleDataMap: convenience pointer holder for global "the" instances.
template <class T>
class SingleDataMap {
public:
    static T& Get() { static T s; return s; }
};

} // namespace shine
#endif
