#pragma once

#include <gmock/gmock-function-mocker.h>
#include "libs/glfw/IGlfwWrapper.hpp"

namespace he
{
namespace window
{
using stdPair = std::pair<int, int>;

class GlfwMock : public IGlfwWrapper
{
public:
    GlfwMock() = default;
    ~GlfwMock() override = default;
    MOCK_METHOD(void, setMouseButtonCallback, (GLFWmousebuttonfun), (override));
    MOCK_METHOD(void, setMouseMovedCallback, (GLFWcursorposfun), (override));
    MOCK_METHOD(void, setKeyCallback, (GLFWkeyfun), (override));
    MOCK_METHOD(void, setScrollCallback, (GLFWscrollfun), (override));
    MOCK_METHOD(void, setWindowSize, (int, int), (override));
    MOCK_METHOD(void, setWindowStateToMaximized, (), (override));
    MOCK_METHOD(void, setWindowStateToMinimized, (), (override));
    MOCK_METHOD(void, setWindowStateToWindowed, (), (override));
    MOCK_METHOD(void, setWindowStateToWindowedFullScreen, (), (override));
    MOCK_METHOD(const stdPair, getMaxWindowSize, (), (const override));
    MOCK_METHOD(const stdPair, getWindowSize, (), (const override));
    MOCK_METHOD(const stdPair, getCursorPos, (), (const override));
    MOCK_METHOD(GLFWmonitor*, getPrimaryMonitor, (), (override));
    MOCK_METHOD(void, pollEvents, (), (override));
};
} // namespace window
} // namespace he