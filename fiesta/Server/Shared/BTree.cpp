// Server/Shared/BTree.cpp
// Generic B-tree -- the canonical-named alias for the original
// red-black map the Korean codebase used. Modern map<K,V> is the
// default; this file is the symbol bridge for any caller using
// the original BTree<T> template.
#include "ShineTypes.h"
#include <map>

namespace fiesta {

template <typename K, typename V>
class BTree {
public:
    void Insert(const K& k, const V& v)         { m_kBackend[k] = v; }
    void Erase (const K& k)                     { m_kBackend.erase(k); }
    bool Find  (const K& k, V& rOut) const {
        typename std::map<K, V>::const_iterator it = m_kBackend.find(k);
        if (it == m_kBackend.end()) return false;
        rOut = it->second; return true;
    }
private:
    std::map<K, V> m_kBackend;
};

} // namespace fiesta
