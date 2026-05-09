// Server/Zone/MainOption.cpp
// Server-side main options -- read from MainOption.shn / ServerInfo.txt
// at boot. Holds drop-rate scaler, exp-rate scaler, KQ wave count, etc.
#include "../DataReader/ShnRegistry.h"
namespace fiesta {
class MainOption {
public:
    static MainOption& Get() { static MainOption s; return s; }
    int32 ExpRate     () const { return m_iExpRate; }
    int32 DropRate    () const { return m_iDropRate; }
    int32 MoneyRate   () const { return m_iMoneyRate; }
    void  SetExpRate  (int32 v) { m_iExpRate   = v; }
    void  SetDropRate (int32 v) { m_iDropRate  = v; }
    void  SetMoneyRate(int32 v) { m_iMoneyRate = v; }
    bool  Load() { return ShnRegistry::Get().GetTable("MainOption") != NULL; }
private:
    MainOption() : m_iExpRate(100), m_iDropRate(100), m_iMoneyRate(100) {}
    int32 m_iExpRate, m_iDropRate, m_iMoneyRate;
};
} // namespace fiesta
