#include <gtest/gtest.h>
#include <limits>
#include <string>
#include "gl_wrapper/window/Window.hpp"

namespace
{
} // namespace
namespace  gl_wrapper
{
class WindowTest : public testing::Test
{
};

class WindowCreatingTest : public testing::TestWithParam<std::tuple<int, int, const char*, int>>
{
};

TEST_P(WindowCreatingTest, whenCreateWindow_shouldReturnCorrectOutput)
{
    Window window;
    const auto [width, height, windowName, result] = GetParam();

    ASSERT_EQ(window.createWindow(width, height, windowName), result);
}

INSTANTIATE_TEST_SUITE_P(
    WindowCreatingInstantiate, 
    WindowCreatingTest,
    testing::Values(
        std::make_tuple(1024, 768, "window test", 1),
        std::make_tuple(0, 1, "window test", -1),
        std::make_tuple(1, 0, "window test", -1),
        std::make_tuple(1024, 768, "", 1),
        std::make_tuple(std::numeric_limits<int>::max(), std::numeric_limits<int>::max(), "window test", 1),
        std::make_tuple(std::numeric_limits<int>::min(), std::numeric_limits<int>::min(), "window test", -1)));
} // namespace gl_wrapper