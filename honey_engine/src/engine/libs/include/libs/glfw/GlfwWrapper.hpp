#pragma once

#include "libs/glfw/IGlfwWrapper.hpp"

namespace he
{
namespace window
{
class GlfwWrapper : public IGlfwWrapper
{
public:
    GlfwWrapper(GLFWwindow*);
    ~GlfwWrapper() override {};

    void setMouseButtonCallback(GLFWmousebuttonfun) override;
    void setMouseMovedCallback(GLFWcursorposfun) override;
    void setKeyCallback(GLFWkeyfun) override;
    void setScrollCallback(GLFWscrollfun) override;
    void setWindowSize(int width, int height) override;
    void setWindowStateToMaximized() override;
    void setWindowStateToMinimized() override;
    void setWindowStateToWindowed() override;
    void setWindowStateToWindowedFullScreen() override;
    GLFWmonitor* getPrimaryMonitor() override;
    std::pair<int, int> const getMaxWindowSize() const override;
    std::pair<int, int> const getWindowSize() const override;
    std::pair<int, int> const getCursorPos() const override;
    void pollEvents() override;

private:
    GLFWwindow* m_window;
};
} // namespace window
} // namespace he