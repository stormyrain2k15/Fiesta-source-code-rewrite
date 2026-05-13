// Server/Zone/tests/TestBase.h
// Tiny VS2010-compatible assert harness for the per-system tests. Each
// test_*.cpp registers cases through TEST_CASE(name); main() walks the
// registry and prints PASS/FAIL.
#ifndef SHINE_ZONE_TESTBASE_H
#define SHINE_ZONE_TESTBASE_H
#include <string>
#include <vector>
#include <stdio.h>

namespace shine {

typedef bool (*TestFn)();

struct TestCase { std::string name; TestFn fn; };

class TestRegistry {
public:
    static TestRegistry& Get() { static TestRegistry s; return s; }
    void Add(const std::string& n, TestFn f) { TestCase c={n,f}; m_kCases.push_back(c); }
    int  RunAll() {
        int passed = 0, failed = 0;
        for (size_t i = 0; i < m_kCases.size(); ++i) {
            bool ok = m_kCases[i].fn();
            printf("[%s] %s\n", ok ? "PASS" : "FAIL", m_kCases[i].name.c_str());
            if (ok) ++passed; else ++failed;
        }
        printf("\n%d passed, %d failed\n", passed, failed);
        return failed;
    }
private:
    std::vector<TestCase> m_kCases;
};

#define TEST_CASE(name)                                            \
    static bool name##_fn();                                        \
    static struct name##_reg {                                      \
        name##_reg() { TestRegistry::Get().Add(#name, &name##_fn); }\
    } name##_reg_inst;                                              \
    static bool name##_fn()

#define ASSERT_TRUE(x)  do { if (!(x)) { printf("  ASSERT_TRUE failed: %s\n", #x); return false; } } while (0)
#define ASSERT_EQ(a,b)  do { if ((a)!=(b)) { printf("  ASSERT_EQ failed\n"); return false; } } while (0)

} // namespace shine
#endif
