#include "window/window/Window.hpp"

#include <memory>
#include <thread>
#include "exception/render_error.hpp"
#include "logger/Logger.hpp"

namespace
{
int WINDOW_MIN_SIZE = 20;

void framebufferSizeCallback(GLFWwindow*, int width, int height)
{
    glViewport(0, 0, width, height);
}
} // namespace

namespace he
{
namespace window
{
////////////////////////////////////////////////////////////
Window::Window(int width, int height, const char* name, WindowState state) 
: m_window{nullptr} 
, m_windowState{state}
, m_windowSize{width, height}
, m_glfwWrapper{nullptr}
{
    auto result = createWindow(width, height, name);
    if (result < 0)
    {
        throw he::common::render_error();
    }
    glfwSetWindowUserPointer(m_window, this);
    m_glfwWrapper = std::make_shared<he::window::GlfwWrapper>(m_window);
}


////////////////////////////////////////////////////////////
Window::~Window()
{
    if (not m_isWindowClosed)
    {
        terminate();
    }
}


////////////////////////////////////////////////////////////
void Window::terminate()
{
    m_currentEvent = he::window::Event{};
    glfwDestroyWindow(m_window);
    glfwTerminate();
    m_window = nullptr;
    glfwMakeContextCurrent(nullptr);
}


////////////////////////////////////////////////////////////
bool Window::isWindowOpen() const
{
    return not glfwWindowShouldClose(m_window);
}


////////////////////////////////////////////////////////////
void Window::clear()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}


////////////////////////////////////////////////////////////
GLFWwindow* Window::getWindow() const
{ 
    return m_window;
}


////////////////////////////////////////////////////////////
Size2d const Window::getWindowSize() const
{
    return std::make_pair(m_windowSize.width, m_windowSize.height);
}


////////////////////////////////////////////////////////////
WindowState Window::getWindowState() const
{
    return m_windowState;
}


////////////////////////////////////////////////////////////
void Window::swapBuffers()
{
   glfwSwapBuffers(m_window); 
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// PRIVATE
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
int Window::createWindow(int widthWin, int heightWin, const char* name)
{
    int width = widthWin;
    int height = heightWin;
    const int status = glfwInit();

    glfwSetErrorCallback([](int errorCode, const char* errorDesc){
        LOG_WARNING << errorDesc << ", " << errorCode;
    });

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // TODO: move to wrapper
    GLFWmonitor* primaryMonitor{nullptr};
    if (m_windowState == he::window::WindowState::maximized)
    {
        primaryMonitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* videoMode = glfwGetVideoMode(primaryMonitor);
        width = videoMode->width;
        height = videoMode->height;
    }

    m_window = glfwCreateWindow(width, height, name, primaryMonitor, nullptr);

    if (m_window == nullptr)
    {
        glfwTerminate();
        return -1;
    }

    // TODO co to ?
    // window = glfwGetCurrentContext();

    glfwMakeContextCurrent(m_window);

    glfwSetFramebufferSizeCallback(m_window, framebufferSizeCallback);

    if (not gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        LOG_ERROR << "Failed to initialize GLAD";
        return -1;
    }  
    if (status == GL_FALSE)
    {
        LOG_ERROR << "GLFW Error!";
        return -1;
    }

    glViewport(0, 0, width, height);
    LOG_DEBUG << "Create window: \"" << name << "\", size: " << width << "x" << height;
    m_windowSize.width = width;
    m_windowSize.height = height;
    return 0;
}


////////////////////////////////////////////////////////////
void Window::enableEvents()
{
    m_glfwWrapper->setKeyCallback([](GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        he::window::Window* windowPtr = (he::window::Window*)glfwGetWindowUserPointer(window);
        windowPtr->handleKeyEvent(key, scancode, action, mods);
    });

    m_glfwWrapper->setMouseButtonCallback([](GLFWwindow* window, int button, int action, int)
    {
        he::window::Window* windowPtr = (he::window::Window*)glfwGetWindowUserPointer(window);
        windowPtr->handleMouseEvent(button, action);
    });

    m_glfwWrapper->setScrollCallback([](GLFWwindow* window, double xoffset, double yoffset)
    {
        he::window::Window* windowPtr = (he::window::Window*)glfwGetWindowUserPointer(window);
        windowPtr->handleScrollEvent(xoffset, yoffset);
    });
}


////////////////////////////////////////////////////////////
void Window::disableEvents()
{
    m_glfwWrapper->setKeyCallback(nullptr);

    m_glfwWrapper->setMouseButtonCallback(nullptr);
}


////////////////////////////////////////////////////////////
void Window::closeWindow()
{
    glfwSetWindowShouldClose(m_window, GLFW_TRUE);
    m_isWindowClosed = true;
}


////////////////////////////////////////////////////////////
void Window::resize(int width, int height, WindowState state)
{
    switch(state)
    {
        case he::window::WindowState::windowed:
            if (m_windowState != he::window::WindowState::windowed)
            {
                m_glfwWrapper->setWindowStateToWindowed();
            }
            if (m_windowSize.width != width or m_windowSize.height != height)
            {
                if (width > WINDOW_MIN_SIZE and height > WINDOW_MIN_SIZE)
                {
                    m_glfwWrapper->setWindowSize(width, height);
                }
            }
            break;
        case he::window::WindowState::windowedFullScreen:
            m_glfwWrapper->setWindowStateToWindowedFullScreen();
            break;
        case he::window::WindowState::minimized:
            m_glfwWrapper->setWindowStateToMinimized();
            break;
        case he::window::WindowState::maximized:
            m_glfwWrapper->setWindowStateToMaximized();
            break;
        default:
            LOG_ERROR << "Cannot change window state. Unknown WindowState case";
            return;
    }
    auto screenSize = m_glfwWrapper->getWindowSize();
    m_windowSize.width = screenSize.first;
    m_windowSize.height = screenSize.second;
    m_windowState = state;
}
} // namespace window
} // namespace he