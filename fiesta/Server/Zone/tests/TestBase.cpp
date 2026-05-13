// Server/Zone/tests/TestBase.cpp
// Test runner entry point. Linked into the test exe only.
#include "TestBase.h"

namespace shine {

int RunAllTests() { return TestRegistry::Get().RunAll(); }

} // namespace shine

#ifdef SHINE_TEST_MAIN
int main(int /*argc*/, char** /*argv*/) { return shine::RunAllTests(); }
#endif
