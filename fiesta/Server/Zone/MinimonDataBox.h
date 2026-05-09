// Server/Zone/MinimonDataBox.h
// WIRE-19 (Lyra, May 2026)
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
