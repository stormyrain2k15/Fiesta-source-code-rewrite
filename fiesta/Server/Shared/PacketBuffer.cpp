// Server/Shared/PacketBuffer.cpp
#include "PacketBuffer.h"
#include <stdlib.h>
#include <string.h>

namespace shine {

PacketBuffer::PacketBuffer() : m_pData(NULL), m_uiSize(0), m_uiCap(0), m_uiRead(0) {}

PacketBuffer::PacketBuffer(size_t cap) : m_pData(NULL), m_uiSize(0), m_uiCap(0), m_uiRead(0) {
    Reserve(cap);
}

PacketBuffer::PacketBuffer(const PacketBuffer& o) : m_pData(NULL), m_uiSize(0), m_uiCap(0), m_uiRead(0) {
    *this = o;
}

PacketBuffer& PacketBuffer::operator=(const PacketBuffer& o) {
    if (this == &o) return *this;
    Reserve(o.m_uiSize);
    if (o.m_uiSize) memcpy(m_pData, o.m_pData, o.m_uiSize);
    m_uiSize = o.m_uiSize; m_uiRead = o.m_uiRead;
    return *this;
}

PacketBuffer::~PacketBuffer() { free(m_pData); }

void PacketBuffer::Clear() { m_uiSize = 0; m_uiRead = 0; }

void PacketBuffer::Reserve(size_t cap) {
    if (cap <= m_uiCap) return;
    size_t n = m_uiCap ? m_uiCap : 64;
    while (n < cap) n *= 2;
    uint8* p = (uint8*)realloc(m_pData, n);
    if (!p) return; // OOM in long-running server: leave previous buffer intact
    m_pData = p; m_uiCap = n;
}

void PacketBuffer::WriteU8 (uint8 v) { Reserve(m_uiSize+1); m_pData[m_uiSize++]=v; }
void PacketBuffer::WriteU16(uint16 v){ Reserve(m_uiSize+2); m_pData[m_uiSize++]=(uint8)v; m_pData[m_uiSize++]=(uint8)(v>>8); }
void PacketBuffer::WriteU32(uint32 v){ Reserve(m_uiSize+4); for(int i=0;i<4;++i) m_pData[m_uiSize++]=(uint8)(v>>(i*8)); }
void PacketBuffer::WriteI32(int32  v){ WriteU32((uint32)v); }
void PacketBuffer::WriteU64(uint64 v){ Reserve(m_uiSize+8); for(int i=0;i<8;++i) m_pData[m_uiSize++]=(uint8)(v>>(i*8)); }
void PacketBuffer::WriteI64(int64  v){ WriteU64((uint64)v); }
void PacketBuffer::WriteF32(float  v){ uint32 u; memcpy(&u,&v,4); WriteU32(u); }
void PacketBuffer::WriteBytes(const void* p, size_t n){ Reserve(m_uiSize+n); memcpy(m_pData+m_uiSize,p,n); m_uiSize+=n; }
void PacketBuffer::WriteString(const char* s){
    size_t n = s ? strlen(s) : 0; if (n > 0xFFFF) n = 0xFFFF;
    WriteU16((uint16)n); if (n) WriteBytes(s, n);
}
void PacketBuffer::WriteWString(const wchar_t* s){
    size_t n = s ? wcslen(s) : 0; if (n > 0xFFFF) n = 0xFFFF;
    WriteU16((uint16)n); for (size_t i=0;i<n;++i) WriteU16((uint16)s[i]);
}

bool PacketBuffer::ReadU8 (uint8&  v){ if(m_uiRead+1>m_uiSize) return false; v=m_pData[m_uiRead++]; return true; }
bool PacketBuffer::ReadU16(uint16& v){ if(m_uiRead+2>m_uiSize) return false; v=(uint16)m_pData[m_uiRead]|((uint16)m_pData[m_uiRead+1]<<8); m_uiRead+=2; return true; }
bool PacketBuffer::ReadU32(uint32& v){ if(m_uiRead+4>m_uiSize) return false; v=0; for(int i=0;i<4;++i) v|=((uint32)m_pData[m_uiRead+i])<<(i*8); m_uiRead+=4; return true; }
bool PacketBuffer::ReadI32(int32&  v){ uint32 u; if(!ReadU32(u)) return false; v=(int32)u; return true; }
bool PacketBuffer::ReadU64(uint64& v){ if(m_uiRead+8>m_uiSize) return false; v=0; for(int i=0;i<8;++i) v|=((uint64)m_pData[m_uiRead+i])<<(i*8); m_uiRead+=8; return true; }
bool PacketBuffer::ReadI64(int64&  v){ uint64 u; if(!ReadU64(u)) return false; v=(int64)u; return true; }
bool PacketBuffer::ReadF32(float&  v){ uint32 u; if(!ReadU32(u)) return false; memcpy(&v,&u,4); return true; }
bool PacketBuffer::ReadBytes(void* p, size_t n){ if(m_uiRead+n>m_uiSize) return false; memcpy(p,m_pData+m_uiRead,n); m_uiRead+=n; return true; }
bool PacketBuffer::ReadString(std::string& s){
    uint16 n; if(!ReadU16(n)) return false; if(m_uiRead+n>m_uiSize) return false;
    s.assign((const char*)(m_pData+m_uiRead), n); m_uiRead+=n; return true;
}

} // namespace shine
