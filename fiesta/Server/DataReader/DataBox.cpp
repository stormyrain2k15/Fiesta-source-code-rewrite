// Server/DataReader/DataBox.cpp
#include "DataBox.h"
#include "../Shared/ShineLogSystem.h"

namespace shine {

DataBox& DataBox::Get() { static DataBox s; return s; }

void DataBox::Register(IDataTable* pk) {
    if (!pk) return;
    m_kAll.push_back(pk);
    if (pk->LogicalName()) m_kByName[pk->LogicalName()] = pk;
}

bool DataBox::LoadAll(DataReader& r) {
    bool bAllOk = true;
    for (size_t i = 0; i < m_kAll.size(); ++i) {
        if (!m_kAll[i]->Load(r)) {
            SHINELOG_WARN("DataBox: %s did not load (skipped)", m_kAll[i]->LogicalName());
            bAllOk = false;
        }
    }
    SHINELOG_INFO("DataBox: %u tables registered", (uint32)m_kAll.size());
    return bAllOk;
}

void DataBox::Shutdown() {
    for (size_t i = 0; i < m_kAll.size(); ++i) m_kAll[i]->BeforeTerminate();
}

IDataTable* DataBox::Find(const char* szLogical) {
    if (!szLogical) return NULL;
    std::map<std::string, IDataTable*>::iterator it = m_kByName.find(szLogical);
    return (it == m_kByName.end()) ? NULL : it->second;
}

} // namespace shine
