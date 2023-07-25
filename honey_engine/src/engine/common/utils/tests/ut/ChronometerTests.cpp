#include <gtest/gtest.h>
#include <chrono>
#include <thread>
#include "utils/Chronometer.hpp"
#include "logger/Logger.hpp"
namespace
{
    const std::chrono::seconds timespan(1);
} // namespace
class ChronometerTests : public testing::Test
{
public:
};

TEST_F(ChronometerTests, startEndMeasurement)
{
    utils::Chronometer tm;
    tm.start();
    std::this_thread::sleep_for(timespan);
    tm.end();
    ASSERT_EQ(timespan, std::chrono::duration_cast<std::chrono::seconds>(tm.getElapsedTime()));
}

TEST_F(ChronometerTests, startStartEndMeasurement)
{
    utils::Chronometer tm;
    tm.start();
    tm.start();
    std::chrono::seconds timespan(1);
    std::this_thread::sleep_for(timespan);
    tm.end();
    ASSERT_EQ(timespan, std::chrono::duration_cast<std::chrono::seconds>(tm.getElapsedTime()));
}

TEST_F(ChronometerTests, startEndEndMeasurement)
{
    utils::Chronometer tm;
    tm.start();
    std::chrono::seconds timespan(1);
    std::this_thread::sleep_for(timespan);
    tm.end();
    tm.end();
    ASSERT_EQ(timespan, std::chrono::duration_cast<std::chrono::seconds>(tm.getElapsedTime()));
}

TEST_F(ChronometerTests, OnlyStartMeasurement)
{
    utils::Chronometer tm;
    tm.start();
    std::chrono::seconds timespan(1);
    std::this_thread::sleep_for(timespan);
    ASSERT_EQ(timespan, std::chrono::duration_cast<std::chrono::seconds>(tm.getElapsedTime()));
}
