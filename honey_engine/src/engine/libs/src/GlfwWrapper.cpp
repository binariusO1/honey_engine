#include "libs/glfw/GlfwWrapper.hpp"

namespace he
{
namespace window
{
////////////////////////////////////////////////////////////
GlfwWrapper::GlfwWrapper(GLFWwindow* window) : m_window{window}
{
}


////////////////////////////////////////////////////////////
void GlfwWrapper::setMouseButtonCallback(GLFWmousebuttonfun callback)
{
    glfwSetMouseButtonCallback(m_window, callback);
}


////////////////////////////////////////////////////////////
void GlfwWrapper::setMouseMovedCallback(GLFWcursorposfun callback)
{
    glfwSetCursorPosCallback(m_window, callback);
}


////////////////////////////////////////////////////////////
void GlfwWrapper::setKeyCallback(GLFWkeyfun callback)
{
    glfwSetKeyCallback(m_window, callback);
}


////////////////////////////////////////////////////////////
void GlfwWrapper::setScrollCallback(GLFWscrollfun callback)
{
    glfwSetScrollCallback(m_window, callback);
}


////////////////////////////////////////////////////////////
void GlfwWrapper::setWindowStateToMaximized()
{
    glfwMaximizeWindow(m_window);
}


////////////////////////////////////////////////////////////
void GlfwWrapper::setWindowStateToMinimized()
{
    glfwIconifyWindow(m_window);
}


////////////////////////////////////////////////////////////
void GlfwWrapper::setWindowSize(int width, int height)
{
    glfwSetWindowSize(m_window, width, height);
}


////////////////////////////////////////////////////////////
void GlfwWrapper::setWindowStateToWindowed()
{
    glfwRestoreWindow(m_window);
}


////////////////////////////////////////////////////////////
void GlfwWrapper::setWindowStateToWindowedFullScreen()
{
    auto screenSize = getMaxWindowSize();
    glfwRestoreWindow(m_window);
    glfwSetWindowSize(m_window, screenSize.first, screenSize.second);
}


////////////////////////////////////////////////////////////
GLFWmonitor* GlfwWrapper::getPrimaryMonitor()
{
    return glfwGetPrimaryMonitor();
}


////////////////////////////////////////////////////////////
std::pair<int, int> const GlfwWrapper::getMaxWindowSize() const
{
    const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    int screenWidth = mode->width;
    int screenHeight = mode->height;
    return std::make_pair(screenWidth, screenHeight);
}


////////////////////////////////////////////////////////////
std::pair<int, int> const GlfwWrapper::getWindowSize() const
{
    int width, height;
    glfwGetWindowSize(m_window, &width, &height);
    return std::make_pair(width, height);
}


////////////////////////////////////////////////////////////
std::pair<int, int> const GlfwWrapper::getCursorPos() const
{
    double xpos, ypos;
    glfwGetCursorPos(m_window, &xpos, &ypos);
    return std::make_pair(xpos, ypos);
}


////////////////////////////////////////////////////////////
void GlfwWrapper::pollEvents()
{
    glfwPollEvents();
}


} // namespace window
} // namespace he