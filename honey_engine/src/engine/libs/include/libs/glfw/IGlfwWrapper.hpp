#pragma once

#include <utility>
#include <GLFW/glfw3.h>

namespace he
{
namespace window
{
class IGlfwWrapper 
{
public:
    virtual ~IGlfwWrapper() = default;

    virtual void setMouseButtonCallback(GLFWmousebuttonfun) = 0;
    virtual void setMouseMovedCallback(GLFWcursorposfun) = 0;
    virtual void setKeyCallback(GLFWkeyfun) = 0;
    virtual void setScrollCallback( GLFWscrollfun) = 0;
    virtual void setWindowSize(int, int) = 0;
    virtual void setWindowStateToMaximized() = 0;
    virtual void setWindowStateToMinimized() = 0;
    virtual void setWindowStateToWindowed() = 0;
    virtual void setWindowStateToWindowedFullScreen() = 0;
    virtual GLFWmonitor* getPrimaryMonitor() = 0;
    virtual std::pair<int, int> const getMaxWindowSize() const = 0;
    virtual std::pair<int, int> const getWindowSize() const = 0;
    virtual std::pair<int, int> const getCursorPos() const = 0;
    virtual void pollEvents() = 0;
};
} // namespace window
} // namespace he