#include <gtest/gtest.h>
#include <chrono>
#include <thread>
#include "utils/Chronometer.hpp"
#include "utils/Timer.hpp"
#include "utils/TimeScheduler.hpp"
#include "logger/Logger.hpp"

namespace
{
} // namespace
class TimeSchedulerTests : public testing::Test
{
public:
};

TEST_F(TimeSchedulerTests, addTimerTwice)
{
    ASSERT_EQ(true, true);
}
