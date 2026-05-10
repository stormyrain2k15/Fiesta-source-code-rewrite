// Server/Zone/World/DefaultCharacterData.cpp
// Auto-generated: loader for DefaultCharacterData.txt (tab-separated, Shine root).
#include "DefaultCharacterData.h"
#include "../../Shared/ShineLogSystem.h"
#include <fstream>
#include <sstream>
#include <string>

namespace fiesta {

DefaultCharacterDataTable& DefaultCharacterDataTable::Get() { static DefaultCharacterDataTable s; return s; }

void DefaultCharacterDataTable::Load(const std::string& rPath) {
    std::ifstream fin(rPath.c_str());
    if (!fin.is_open()) {
        SHINELOG_WARN("DefaultCharacterData.txt: cannot open '%s'", rPath.c_str());
        return;
    }
    std::string line;
    while (std::getline(fin, line)) {
        if (line.empty() || line[0] == ';' || line[0] == '[') continue;
        std::istringstream ss(line);
        DefaultCharacterDataRow rec;
        { int64 _v = 0; ss >> _v; rec.iClass = (int32)_v; }
        if (!(ss >> rec.kStartMapName)) continue;
        { int64 _v = 0; ss >> _v; rec.iStartPX = (int32)_v; }
        { int64 _v = 0; ss >> _v; rec.iStartPY = (int32)_v; }
        { int64 _v = 0; ss >> _v; rec.iHP = (int32)_v; }
        { int64 _v = 0; ss >> _v; rec.iSP = (int32)_v; }
        { int64 _v = 0; ss >> _v; rec.iHPSoulStoneCount = (int32)_v; }
        { int64 _v = 0; ss >> _v; rec.iSPSoulStoneCount = (int32)_v; }
        { int64 _v = 0; ss >> _v; rec.iMoney = (int32)_v; }
        { int64 _v = 0; ss >> _v; rec.iInitLV = (int32)_v; }
        if (!(ss >> rec.kInitEXP)) continue;
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("DefaultCharacterData.txt: %u rows", (uint32)m_kRows.size());
}

} // namespace fiesta
