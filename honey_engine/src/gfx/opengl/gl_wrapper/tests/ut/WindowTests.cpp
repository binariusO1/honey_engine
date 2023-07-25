#include <gtest/gtest.h>
#include <limits>
#include <memory>
#include <string>
#include "exception/render_error.hpp"
#include "gl_wrapper/window/Window.hpp"

namespace
{
const char* testName{"window test"};
} // namespace
namespace  gl_wrapper
{
class WindowTests : public testing::Test
{
public:
    WindowTests() = default;
    ~WindowTests() override {};

    void createSut()
    {
        sut = std::make_unique<gl_wrapper::Window>(1024, 768, testName);
    }

    std::unique_ptr<gl_wrapper::Window> sut;
};

class WindowCreateFailTests : public testing::TestWithParam<std::tuple<int, int, const char*>>
{
};

TEST_P(WindowCreateFailTests, whenCreateWindow_shouldThrowException)
{
    const auto [width, height, windowName] = GetParam();
    EXPECT_THROW(Window(width, height, windowName), gfx::render_error);
}

INSTANTIATE_TEST_SUITE_P(
    WindowCreateFailInstantiate, 
    WindowCreateFailTests,
    testing::Values(
        std::make_tuple(0, 1, testName),
        std::make_tuple(1, 0, testName),
        std::make_tuple(std::numeric_limits<int>::min(), std::numeric_limits<int>::min(), testName)));

TEST_F(WindowTests, shouldCloseWindow)
{
    createSut();
    sut->closeWindow();
    ASSERT_EQ(sut->isWindowOpen(), false);
}

TEST_F(WindowTests, shouldTerminateWindow)
{
    createSut();
    sut->terminate();
    ASSERT_EQ(sut->isWindowOpen(), true);
}

TEST_F(WindowTests, whenGetWindow_shouldReturnInitializedInstance)
{
    createSut();
    auto newWindow = sut->getWindow();
    ASSERT_NE(newWindow, nullptr);
}

TEST_F(WindowTests, whenGetCursorPos_shouldReturnSpecificValue)
{
    createSut();
    auto cursorPos = sut->getCursorPos();
    ASSERT_NE(cursorPos.x, 0.0);
    ASSERT_NE(cursorPos.y, 0.0);
}

} // namespace gl_wrapper