// Server/Shared/BinaryDataCollection.cpp
// Generic raw-bytes collection -- used as a backing store for SHN row
// blobs and for any opaque binary the engine needs to pass around
// without copying.
#include "ShineTypes.h"
#include <vector>

namespace shine {

class BinaryDataCollection {
public:
    void           Append(const uint8* p, size_t n)         { m_kBuf.insert(m_kBuf.end(), p, p + n); }
    const uint8*   Data() const                              { return m_kBuf.empty() ? NULL : &m_kBuf[0]; }
    size_t         Size() const                              { return m_kBuf.size(); }
    void           Clear()                                   { m_kBuf.clear(); }
private:
    std::vector<uint8> m_kBuf;
};

} // namespace shine
