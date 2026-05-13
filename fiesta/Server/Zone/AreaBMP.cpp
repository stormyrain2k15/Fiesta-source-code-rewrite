// Server/Zone/AreaBMP.cpp
#include "AreaBMP.h"
#include "../Shared/ShineLogSystem.h"
#include <stdio.h>
#include <string.h>

namespace shine {

#pragma pack(push, 1)
struct BmpFileHeader { uint16 bfType; uint32 bfSize; uint16 bfRes1; uint16 bfRes2; uint32 bfOffBits; };
struct BmpInfoHeader { uint32 biSize; int32 biWidth; int32 biHeight; uint16 biPlanes;
                       uint16 biBitCount; uint32 biCompression; uint32 biSizeImage;
                       int32 biXPelsPerMeter; int32 biYPelsPerMeter;
                       uint32 biClrUsed; uint32 biClrImportant; };
#pragma pack(pop)

bool LoadBmp(const std::string& rPath, AreaMask& rOut) {
    FILE* fp = NULL; fopen_s(&fp, rPath.c_str(), "rb");
    if (!fp) return false;
    BmpFileHeader fh; BmpInfoHeader ih;
    if (fread(&fh, 1, sizeof(fh), fp) != sizeof(fh) ||
        fread(&ih, 1, sizeof(ih), fp) != sizeof(ih) ||
        fh.bfType != 0x4D42 /* 'BM' */ || ih.biCompression != 0) {
        fclose(fp); return false;
    }
    int w = ih.biWidth;
    int h = (ih.biHeight < 0) ? -ih.biHeight : ih.biHeight;
    bool topDown = (ih.biHeight < 0);
    int bpp = ih.biBitCount;
    if ((bpp != 8 && bpp != 24 && bpp != 32) || w <= 0 || h <= 0) { fclose(fp); return false; }
    int rowStride = ((w * bpp + 31) / 32) * 4;
    fseek(fp, fh.bfOffBits, SEEK_SET);
    std::vector<uint8> kRaw((size_t)rowStride * (size_t)h);
    if (fread(&kRaw[0], 1, kRaw.size(), fp) != kRaw.size()) { fclose(fp); return false; }
    fclose(fp);

    rOut.uiW = (uint16)w; rOut.uiH = (uint16)h;
    rOut.kCells.assign((size_t)w * (size_t)h, 0);
    for (int y = 0; y < h; ++y) {
        int srcY = topDown ? y : (h - 1 - y);
        const uint8* row = &kRaw[(size_t)srcY * rowStride];
        for (int x = 0; x < w; ++x) {
            uint8 v = 0;
            if (bpp == 8)  v = row[x];
            if (bpp == 24) { uint8 b = row[x*3], g = row[x*3+1], r = row[x*3+2];
                             v = (uint8)((r + g + b) / 3); }
            if (bpp == 32) { uint8 b = row[x*4], g = row[x*4+1], r = row[x*4+2];
                             v = (uint8)((r + g + b) / 3); }
            rOut.kCells[(size_t)y * w + x] = (v >= 0x80) ? 1 : 0;
        }
    }
    return true;
}

AreaBMPBox& AreaBMPBox::Get() { static AreaBMPBox s; return s; }

const AreaMask* AreaBMPBox::Load(const std::string& rRoot, const std::string& rKey) {
    std::string path = rRoot + "\\AreaBMP\\" + rKey + ".bmp";
    AreaMask* pk = new AreaMask();
    if (!LoadBmp(path, *pk)) { delete pk; return NULL; }
    m_kAll[rKey] = pk;
    SHINELOG_DEBUG("AreaBMP '%s' %ux%u", rKey.c_str(), pk->uiW, pk->uiH);
    return pk;
}

const AreaMask* AreaBMPBox::Find(const std::string& rKey) const {
    std::map<std::string, AreaMask*>::const_iterator it = m_kAll.find(rKey);
    return (it == m_kAll.end()) ? NULL : it->second;
}

void AreaBMPBox::Clear() {
    for (std::map<std::string, AreaMask*>::iterator it = m_kAll.begin(); it != m_kAll.end(); ++it)
        delete it->second;
    m_kAll.clear();
}

} // namespace shine
