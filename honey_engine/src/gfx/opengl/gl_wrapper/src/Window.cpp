#include "gl_wrapper/window/Window.hpp"
#include <memory>
#include <thread>
#include "logger/Logger.hpp"

namespace
{
void ErrorCallback(int, const char* err_str)
{
    LOG_ERROR << "GLFW Error: " << err_str;
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    (void)window;
}

void windowCloseCallback(GLFWwindow* window)
{
    glfwSetWindowShouldClose(window, GLFW_TRUE);
}

} // namespace

namespace gl_wrapper
{
Window::Window() : window{nullptr} , m_Chronometer()
{
}

int Window::createWindow(int width, int height, const char* name)
{
    int status = glfwInit();

    glfwSetErrorCallback(ErrorCallback);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(width, height, name, nullptr, nullptr);

    if (window == nullptr)
    {
        LOG_ERROR << "Failed to create GLFW window";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    if (not gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        LOG_ERROR << "Failed to initialize GLAD";
        return -1;
    }  

    if (status == GL_FALSE)
    {
        LOG_ERROR << "GLFW Error!";
    }

    glViewport(0, 0, width, height);

    LOG_DEBUG << "Create window \"" << name << "\". Size: " << width << "x" << height;
    return status;
}

void Window::terminate()
{
    currentEvent.release();
    glfwDestroyWindow(window);
    glfwTerminate();
}

bool Window::isWindowOpen()
{
    return not glfwWindowShouldClose(window);
}

geometry::Point2Dd Window::getCursorPos() const
{
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    return geometry::Point2Dd{xpos, ypos}; //TODO: double
}

void Window::clear()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Window::swapBuffers()
{
   glfwSwapBuffers(window); 
}

void Window::closeWindow()
{
    glfwSetWindowCloseCallback(window, windowCloseCallback);

    // immediately close
    glfwSetWindowShouldClose(window, GLFW_TRUE);
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


std::unique_ptr<sfml2::Event> Window::currentEvent{};

//  PRIVATE

} // namespace gl_wrapper