// Server/Zone/GuildStorageManager.cpp
// Guild bank -- shared inventory. Persisted via CharDB p_GuildStorage_*.
#include "Inventory.h"
namespace shine { class GuildStorageManager { public: static GuildStorageManager& Get(){ static GuildStorageManager s; return s; } }; }
