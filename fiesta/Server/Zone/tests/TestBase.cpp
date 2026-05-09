// Server/Zone/tests/TestBase.cpp
// Test runner entry point. Linked into the test exe only.
#include "TestBase.h"

namespace fiesta {

int RunAllTests() { return TestRegistry::Get().RunAll(); }

} // namespace fiesta

#ifdef FIESTA_TEST_MAIN
int main(int /*argc*/, char** /*argv*/) { return fiesta::RunAllTests(); }
#endif
