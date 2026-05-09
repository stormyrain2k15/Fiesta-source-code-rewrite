// Server/Zone/Manager.cpp
// Top-level singleton container. Holds references to every per-zone
// manager so global init / shutdown is one call.
#include "ZoneServer.h"
namespace fiesta {
class Manager {
public:
    static Manager& Get() { static Manager s; return s; }
    void Init    () {}
    void Shutdown() {}
};
} // namespace fiesta
