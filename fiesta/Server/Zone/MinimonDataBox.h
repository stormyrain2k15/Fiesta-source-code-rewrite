// Server/Zone/MinimonDataBox.h
#ifndef FIESTA_ZONE_MINIONMONDATABOX_H
#define FIESTA_ZONE_MINIONMONDATABOX_H
namespace fiesta {
class MinimonDataBox {
public:
    static MinimonDataBox& Get();
    bool Load();
};
} // namespace fiesta
#endif
