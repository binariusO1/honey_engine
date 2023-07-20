#include <gmock/gmock.h>
#include <iostream>

int main(int argc, char** argv)
{
    ::testing::InitGoogleMock(&argc, argv);
    std::cout << "[INFO] ut_main START";
    auto ret = RUN_ALL_TESTS();
    return ret;
}

