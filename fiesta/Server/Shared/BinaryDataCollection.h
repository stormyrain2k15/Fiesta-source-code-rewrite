// Server/Shared/BinaryDataCollection.h
// intrusive object pool / fixed-block allocator used widely
// (BinaryDataCollection, AntiBloatAllocator references in PDB).
#ifndef FIESTA_BINARYDATACOLLECTION_H
#define FIESTA_BINARYDATACOLLECTION_H
#include "ShineTypes.h"
#include <vector>

namespace fiesta {

template <class T>
class BinaryDataCollection {
public:
    BinaryDataCollection() {}
    ~BinaryDataCollection() { Clear(); }

    T*     NewItem() {
        if (!m_kFree.empty()) { T* p = m_kFree.back(); m_kFree.pop_back(); new (p) T(); return p; }
        T* p = (T*)::operator new(sizeof(T)); new (p) T(); m_kAll.push_back(p); return p;
    }
    void   DeleteItem(T* p) {
        if (!p) return;
        p->~T();
        m_kFree.push_back(p);
    }
    size_t Size() const { return m_kAll.size(); }
    void   Clear() {
        // call dtors only on allocated-but-not-freed
        std::vector<T*> kAlive(m_kAll);
        for (size_t i = 0; i < m_kFree.size(); ++i) {
            for (size_t j = 0; j < kAlive.size(); ++j)
                if (kAlive[j] == m_kFree[i]) { kAlive[j] = NULL; break; }
        }
        for (size_t i = 0; i < kAlive.size(); ++i) if (kAlive[i]) kAlive[i]->~T();
        for (size_t i = 0; i < m_kAll.size(); ++i) ::operator delete(m_kAll[i]);
        m_kAll.clear(); m_kFree.clear();
    }
private:
    std::vector<T*> m_kAll;
    std::vector<T*> m_kFree;
};

} // namespace fiesta
#endif
