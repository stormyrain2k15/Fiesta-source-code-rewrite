// Server/Shared/PacketBuffer.h
// contiguous byte buffer with little-endian read/write.
#ifndef SHINE_PACKETBUFFER_H
#define SHINE_PACKETBUFFER_H
#include "ShineTypes.h"

namespace shine {

class PacketBuffer {
public:
    PacketBuffer();
    explicit PacketBuffer(size_t uiInitialCap);
    PacketBuffer(const PacketBuffer& rOther);
    PacketBuffer& operator=(const PacketBuffer& rOther);
    ~PacketBuffer();

    void          Clear();
    size_t        Size()      const { return m_uiSize; }
    size_t        ReadCursor()const { return m_uiRead; }
    size_t        Remaining() const { return (m_uiRead < m_uiSize) ? (m_uiSize - m_uiRead) : 0; }
    const uint8*  Data()      const { return m_pData; }
    uint8*        Data()            { return m_pData; }
    void          Reserve(size_t uiCap);

    // write
    void WriteU8 (uint8  v);
    void WriteU16(uint16 v);
    void WriteU32(uint32 v);
    void WriteI32(int32  v);
    void WriteU64(uint64 v);
    void WriteI64(int64  v);
    void WriteF32(float  v);
    void WriteBytes(const void* p, size_t n);
    void WriteString(const char* s);          // length-prefixed (uint16)
    void WriteString(const std::string& s) { WriteString(s.c_str()); }
    void WriteWString(const wchar_t* s);

    // read
    bool ReadU8 (uint8&  v);
    bool ReadU16(uint16& v);
    bool ReadU32(uint32& v);
    bool ReadI32(int32&  v);
    bool ReadU64(uint64& v);
    bool ReadI64(int64&  v);
    bool ReadF32(float&  v);
    bool ReadBytes(void* p, size_t n);
    bool ReadString(std::string& s);
private:
    uint8* m_pData;
    size_t m_uiSize;
    size_t m_uiCap;
    size_t m_uiRead;
};

} // namespace shine
#endif
