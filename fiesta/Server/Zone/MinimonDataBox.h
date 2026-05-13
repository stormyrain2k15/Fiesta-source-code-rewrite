// Server/Zone/MinimonDataBox.h
#ifndef SHINE_ZONE_MINIONMONDATABOX_H
#define SHINE_ZONE_MINIONMONDATABOX_H
namespace shine {
class MinimonDataBox {
public:
    static MinimonDataBox& Get();
    bool Load();
};
} // namespace shine
#endif
