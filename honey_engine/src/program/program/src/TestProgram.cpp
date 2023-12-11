#include "Program.hpp"

#include <cstring>
#include "logger/Logger.hpp"

namespace he
{


//////////////////////////////////////////////////////////////////////
int Program::runTest(const char* testName)
{
    if ( not (strncmp(testName, "test_", 5) == 0))
    {
        LOG_ERROR << "Running test program with incorrect argument! argv: " << testName;
        return 1;
    }

    LOG_INFO << LOG_BRIGHT_BLUE << "Run test program: " << testName << LOG_WHITE;

    return 0;
}

} // namespace he