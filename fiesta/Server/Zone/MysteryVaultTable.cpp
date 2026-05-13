// Server/Zone/MysteryVaultTable.cpp
// Mystery-vault drop pool -- a sealed inventory granted on opening the
// "Mystery Vault" item; on open it rolls one item from the pool.
#include "../DataReader/ShnRegistry.h"
namespace shine { namespace {
class MysteryVaultTable {
public:
    static MysteryVaultTable& Get() { static MysteryVaultTable s; return s; }
    bool Load() { return ShnRegistry::Get().GetTable("MysteryVault") != NULL; }
};
}} // anonymous
