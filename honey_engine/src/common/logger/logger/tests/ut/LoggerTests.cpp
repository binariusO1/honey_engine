#include <gtest/gtest.h>
#include <string>
#include "logger/Logger.hpp"

namespace
{
    const std::string correctOutput = "this is the correct output";
    const std::string notCorrectOutput = "2389237h&2j1odhq][";
} // namespace

class LoggerTests : public testing::Test
{
public:
    void toBeTested(std::ostream& output)
    {
        output << correctOutput;
    }
    void productionCode()
    {
        toBeTested(LOG_INFO);
    }
};

TEST_F(LoggerTests, logOuputIsConsistentToCorrect)
{
    std::stringstream ss;
    productionCode();
    toBeTested(ss);
    EXPECT_EQ(ss.str(), correctOutput);
}

TEST_F(LoggerTests, logOuputIsNotConsistentToCorrect)
{
    std::stringstream ss;
    productionCode();
    toBeTested(ss);
    EXPECT_NE(ss.str(), notCorrectOutput);
}