#include "window/Window.hpp"

#include <memory>
#include <thread>
#include "exception/render_error.hpp"
#include "logger/Logger.hpp"

namespace
{
void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    (void)window;
}
} // namespace

namespace he::window
{
////////////////////////////////////////////////////////////
Window::Window(int width, int height, const char* name) : window{nullptr} , m_Chronometer()
{
    auto result = createWindow(width, height, name);
    if (result < 0)
    {
        throw gfx::render_error();
    }
}


////////////////////////////////////////////////////////////
Window::~Window()
{
    if (not isWindowClosed)
    {
        terminate();
    }
}


////////////////////////////////////////////////////////////
void Window::closeWindow()
{
    glfwSetWindowShouldClose(window, GLFW_TRUE);
    isWindowClosed = true;
}


////////////////////////////////////////////////////////////
void Window::terminate()
{
    currentEvent.release();
    glfwDestroyWindow(window);
    glfwTerminate();
    window = nullptr;
    glfwMakeContextCurrent(nullptr);
}


////////////////////////////////////////////////////////////
bool Window::isWindowOpen()
{
    return not glfwWindowShouldClose(window);
}


////////////////////////////////////////////////////////////
void Window::clear()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}


////////////////////////////////////////////////////////////
GLFWwindow* Window::getWindow()
{ 
    return window;
}


////////////////////////////////////////////////////////////
geometry::Point2Dd Window::getCursorPos() const
{
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    return geometry::Point2Dd{xpos, ypos};
}


////////////////////////////////////////////////////////////
void Window::swapBuffers()
{
   glfwSwapBuffers(window); 
}



////////////////////////////////////////////////////////////
void Window::startFrameGuard()
{
    m_Chronometer.start();
    m_frames = 0;
}


////////////////////////////////////////////////////////////
void Window::endFrameGuard()
{
    m_Chronometer.end();
    m_frames = 0;
}


////////////////////////////////////////////////////////////
void Window::guardFrame()
{
    m_frames++;

    if (m_Chronometer.getElapsedTime() >= SECOND)
    {
        auto realFrameTime = SECOND / m_frames;
        if (realFrameTime > m_frameTimeLimit)
        {
            LOG_ERROR << "Framerate time limit (" << m_frameTimeLimit << ") has been exceeded! Framerate: " << realFrameTime << " (" << m_frames << ") frames";
        }
        // LOG_DEBUG << "Real frame time: " << realFrameTime << " , frames:" << m_frames;
        m_frames = 0;
        m_Chronometer.reset();
    }
}


////////////////////////////////////////////////////////////
void Window::setFramerateLimit(unsigned int limit)
{
    if (limit > 0)
    {
        m_frameTimeLimit = static_cast<std::chrono::microseconds>(SECOND / limit);
        LOG_INFO << "Set framerate limit: " << m_frameTimeLimit;
    }
    else
    {
        m_frameTimeLimit = std::chrono::microseconds{0};
    }
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// PRIVATE
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
int Window::createWindow(int width, int height, const char* name)
{
    const int status = glfwInit();

    glfwSetErrorCallback([](int errorCode, const char* errorDesc){
        LOG_WARNING << errorDesc << ", " << errorCode;
    });

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(width, height, name, nullptr, nullptr);

    if (window == nullptr)
    {
        glfwTerminate();
        return -1;
    }

    // window = glfwGetCurrentContext();

    glfwMakeContextCurrent(window);

    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    if (not gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        return -1;
    }  
    if (status == GL_FALSE)
    {
        return -1;
    }

    glViewport(0, 0, width, height);
    LOG_DEBUG << "Create window \"" << name << "\". Size: " << width << "x" << height;
    return 0;
}
} // namespace he::window