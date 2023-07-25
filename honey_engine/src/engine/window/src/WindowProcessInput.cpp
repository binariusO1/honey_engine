#include "window/Window.hpp"
#include "logger/Logger.hpp"
// #include "sys/config/Config.hpp"

namespace
{
geometry::Point2Dd getCursorPos(GLFWwindow* window)
{
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    // return geometry::Point2Dd{xpos, sys::Config::Get().settings->getWindowHeight() - ypos};
    return geometry::Point2Dd{xpos, 1024.0 - ypos};
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    he::window::Event event{};
    geometry::Point2Dd position(getCursorPos(window));
    if (button == GLFW_MOUSE_BUTTON_LEFT)
    {
        event.mouseButton = he::window::Event::MouseButtonAction{he::window::Mouse::Button::Left, position.x, position.y};
    }
    else if (button == GLFW_MOUSE_BUTTON_RIGHT)
    {
        event.mouseButton = he::window::Event::MouseButtonAction{he::window::Mouse::Button::Right, position.x, position.y};
    }

    if (action == GLFW_PRESS)
    {
        event.type = he::window::Event::EventType::MouseButtonPressed;
    }
    else if(action == GLFW_RELEASE)
    {
        event.type = he::window::Event::EventType::MouseButtonReleased;
    }

    // TODO (honey)
    // gl_wrapper::Window::currentEvent = std::make_unique<he::window::Event>(event);
}

bool checkPos(geometry::Point2Dd oldPos, geometry::Point2Dd newPos)
{
    return ((oldPos.x == newPos.x) and (oldPos.y == newPos.y));
}

bool setStatus(he::window::Event& event)
{
    // TODO (honey)
    /*
    if(gl_wrapper::Window::currentEvent  != nullptr)
    {
        event = *gl_wrapper::Window::currentEvent;
        gl_wrapper::Window::currentEvent  = nullptr;
        return true;
    }
    */
    return false;
}
} // namespace

namespace he
{
namespace window
{
void Window::pollEvent()
{
    glfwPollEvents();
}

bool Window::processInput(he::window::Event& event)
{
    event = he::window::Event{}; // note: deleting event here will unblock key events flood
    bool status{false};

    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    status = setStatus(event);
    if (isMouseMoved() and not status)
    {
        event.mouseMove = he::window::Event::MouseMoveEvent{::getCursorPos(window).x, ::getCursorPos(window).y};
        event.type = he::window::Event::EventType::MouseMoved;
        he::window::Window::currentEvent  = nullptr;
    }

    return status;
}

// PRIVATE

bool Window::isMouseMoved()
{
    geometry::Point2Dd newPos = geometry::Point2Dd{::getCursorPos(window).x, ::getCursorPos(window).y};
    if (not (checkPos(oldPos, newPos)))
    {
        oldPos = newPos;
        return true;
    }
    return false;
}
} // namespace he
} // namespace window