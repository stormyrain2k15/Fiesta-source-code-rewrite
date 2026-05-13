// Server/Zone/AreaBMP.h
// 1-bit / grayscale bitmaps that define the on-ground shape of AOE skills.
// Files live under AreaBMP/<SkillName>_*.bmp. Each pixel is a cell in the
// skill's footprint (origin = caster, scale per-cell from skill metadata).
#ifndef SHINE_ZONE_AREABMP_H
#define SHINE_ZONE_AREABMP_H
#include "../Shared/ShineTypes.h"
#include <string>
#include <vector>
#include <map>

namespace shine {

struct AreaMask {
    uint16             uiW, uiH;
    std::vector<uint8> kCells;   // 0 = miss, non-0 = hit
    bool   Hit(int x, int y) const {
        if (x < 0 || y < 0 || (uint16)x >= uiW || (uint16)y >= uiH) return false;
        size_t i = (size_t)y * uiW + (size_t)x;
        return i < kCells.size() && kCells[i] != 0;
    }
};

class AreaBMPBox {
public:
    static AreaBMPBox& Get();
    const AreaMask* Load(const std::string& rRoot, const std::string& rSkillKey);
    const AreaMask* Find(const std::string& rSkillKey) const;
    void   Clear();
private:
    std::map<std::string, AreaMask*> m_kAll;
};

// Minimal Windows BMP reader (BI_RGB 8/24/32 bpp, top-down or bottom-up).
bool LoadBmp(const std::string& rPath, AreaMask& rOut);

} // namespace shine
#endif
