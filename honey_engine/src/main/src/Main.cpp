#include "Program.hpp"
#include "sys/honey_engine.h"

int main(int argc, char* argv[])
{
    he::Program program;

    switch (argc)
    {
        case 1:
            return program.run(argv);
        case 2:
            return program.runTest(argv[1]);
        break;
    default:
        break;
    }

    return 0;
}

