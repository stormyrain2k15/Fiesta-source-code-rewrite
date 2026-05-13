// Client/ResSystem/ActionDat.cpp
// Layout verified against ActionReader v12 / ActionFile.cs
#include "ActionDat.h"
#include "../../Server/Shared/ShineLogSystem.h"
#include <fstream>
#include <cstring>

namespace shine {

ActionDat::ActionDat() : m_bLoaded(false) {}

bool ActionDat::Load(const std::string& rPath) {
    m_bLoaded = false;
    m_kBlocks.clear();
    m_kByType.clear();
    m_kBySeq.clear();
    m_kByAnimID.clear();

    size_t slash = rPath.find_last_of("\\/");
    std::string kFile = (slash != std::string::npos) ? rPath.substr(slash+1) : rPath;
    size_t dot = kFile.rfind('.');
    m_kInxName = (dot != std::string::npos) ? kFile.substr(0, dot) : kFile;

    std::ifstream fin(rPath.c_str(), std::ios::binary);
    if (!fin.is_open()) {
        SHINELOG_WARN("ActionDat: cannot open '%s'", rPath.c_str());
        return false;
    }
    fin.seekg(0, std::ios::end);
    size_t uiSize = (size_t)fin.tellg();
    fin.seekg(0);
    if (uiSize < 4) return false;

    std::vector<uint8> buf(uiSize);
    fin.read((char*)buf.data(), (std::streamsize)uiSize);
    fin.close();

    const uint8* p = buf.data();
    size_t pos = 0;

    int32 iCount = 0;
    memcpy(&iCount, p + pos, 4); pos += 4;
    m_kBlocks.reserve((size_t)iCount);

    for (int32 i = 0; i < iCount; ++i) {
        if (pos + 348 > uiSize) break;

        ActionBlock blk;
        memset(blk.aSomething,     0, sizeof(blk.aSomething));
        memset(blk.aSomethingElse, 0, sizeof(blk.aSomethingElse));

        // int16 ActID
        memcpy(&blk.iActID, p + pos, 2); pos += 2;
        // char[33] SHNPath
        blk.kSHNPath  = ReadString(p, pos, 0x21);
        // char[33] FilePath
        blk.kFilePath = ReadString(p, pos, 0x21);
        // int32 loop (-1, discard)
        pos += 4;
        // char[36] Text
        blk.kText     = ReadString(p, pos, 0x24);
        // int32 NifAnimID
        memcpy(&blk.iNifAnimID, p + pos, 4); pos += 4;
        // int32 zeros
        pos += 4;
        // WeirdBlock s1[33], s2[33], s3[34]
        blk.kS1 = ReadWeirdBlock(p, pos, 33);
        blk.kS2 = ReadWeirdBlock(p, pos, 33);
        blk.kS3 = ReadWeirdBlock(p, pos, 34);
        // byte[48] something
        if (pos + 48 <= uiSize) memcpy(blk.aSomething, p + pos, 48);
        pos += 48;
        // WeirdBlock Sound1[33], Sound2[35]
        blk.kSound1 = ReadWeirdBlock(p, pos, 33);
        blk.kSound2 = ReadWeirdBlock(p, pos, 35);
        // int32 Signature
        memcpy(&blk.iSignature, p + pos, 4); pos += 4;
        // byte[16] somthingelse
        if (pos + 16 <= uiSize) memcpy(blk.aSomethingElse, p + pos, 16);
        pos += 16;

        if (!blk.kText.empty() && m_kByType.find(blk.kText) == m_kByType.end())
            m_kByType[blk.kText] = m_kBlocks.size();
        if (!blk.kSHNPath.empty())
            m_kBySeq[blk.kSHNPath] = m_kBlocks.size();
        m_kByAnimID[blk.iNifAnimID] = m_kBlocks.size();

        m_kBlocks.push_back(blk);
    }

    m_bLoaded = true;
    SHINELOG_INFO("ActionDat: '%s' loaded %u blocks", m_kInxName.c_str(), (uint32)m_kBlocks.size());
    return true;
}

const ActionBlock* ActionDat::FindByType(const std::string& rType) const {
    std::map<std::string,size_t>::const_iterator it = m_kByType.find(rType);
    return (it == m_kByType.end()) ? NULL : &m_kBlocks[it->second];
}
const ActionBlock* ActionDat::FindBySeq(const std::string& rSeq) const {
    std::map<std::string,size_t>::const_iterator it = m_kBySeq.find(rSeq);
    return (it == m_kBySeq.end()) ? NULL : &m_kBlocks[it->second];
}
const ActionBlock* ActionDat::FindByAnimID(int32 iAnimID) const {
    std::map<int32,size_t>::const_iterator it = m_kByAnimID.find(iAnimID);
    return (it == m_kByAnimID.end()) ? NULL : &m_kBlocks[it->second];
}

std::string ActionDat::ReadString(const uint8* p, size_t& rPos, int iLen) {
    std::string s;
    for (int i = 0; i < iLen; ++i) {
        uint8 b = p[rPos + i];
        if (b == 0 || b == 0xCD) break;
        s += (char)b;
    }
    rPos += (size_t)iLen;
    return s;
}

std::string ActionDat::ReadWeirdBlock(const uint8* p, size_t& rPos, int iLen) {
    // 0x2D 0x?? = '-' marker = empty field
    if (p[rPos] == 0x2D) {
        rPos += (size_t)iLen;
        return std::string();
    }
    return ReadString(p, rPos, iLen);
}

ActionDatCache& ActionDatCache::Get() { static ActionDatCache s; return s; }

const ActionDat* ActionDatCache::Acquire(const std::string& rInxName,
                                          const std::string& rActionDir) {
    std::map<std::string,ActionDat>::iterator it = m_kCache.find(rInxName);
    if (it != m_kCache.end()) return &it->second;
    std::string kPath = rActionDir + "\\" + rInxName + ".dat";
    ActionDat& dat = m_kCache[rInxName];
    if (!dat.Load(kPath)) { m_kCache.erase(rInxName); return NULL; }
    return &m_kCache[rInxName];
}

void ActionDatCache::Clear() { m_kCache.clear(); }

} // namespace shine

// ── PE resource acquire (checks embedded resources before disk) ───────────────
// Included separately so ActionDat.cpp compiles without PEResourceReader
// if building server-side tooling. Define SHINE_CLIENT to enable.
#ifdef SHINE_CLIENT
#include "PEResourceReader.h"

const ActionDat* ActionDatCache::AcquireFromPE(const std::string& rInxName) {
    std::map<std::string,ActionDat>::iterator it = m_kCache.find(rInxName);
    if (it != m_kCache.end()) return &it->second;

    DWORD dwSize = 0;
    const void* pData = shine::PEResourceReader::Find(
        rInxName.c_str(), "DAT", dwSize);

    if (!pData || dwSize == 0) return NULL;

    ActionDat& dat = m_kCache[rInxName];
    if (!dat.LoadFromMemory(pData, dwSize, rInxName)) {
        m_kCache.erase(rInxName);
        return NULL;
    }
    return &m_kCache[rInxName];
}
#endif // SHINE_CLIENT
