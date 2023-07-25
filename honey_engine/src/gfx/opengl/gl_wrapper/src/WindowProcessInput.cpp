#include "gl_wrapper/window/Window.hpp"
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
    sfml2::Event event{};
    geometry::Point2Dd position(getCursorPos(window));
    if (button == GLFW_MOUSE_BUTTON_LEFT)
    {
        event.mouseButton = sfml2::Event::MouseButtonAction{sfml2::Mouse::Button::Left, position.x, position.y};
    }
    else if (button == GLFW_MOUSE_BUTTON_RIGHT)
    {
        event.mouseButton = sfml2::Event::MouseButtonAction{sfml2::Mouse::Button::Right, position.x, position.y};
    }

    if (action == GLFW_PRESS)
    {
        event.type = sfml2::Event::EventType::MouseButtonPressed;
    }
    else if(action == GLFW_RELEASE)
    {
        event.type = sfml2::Event::EventType::MouseButtonReleased;
    }

    // TODO (honey)
    // gl_wrapper::Window::currentEvent = std::make_unique<sfml2::Event>(event);
}

bool checkPos(geometry::Point2Dd oldPos, geometry::Point2Dd newPos)
{
    return ((oldPos.x == newPos.x) and (oldPos.y == newPos.y));
}

bool setStatus(sfml2::Event& event)
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

namespace gl_wrapper
{
void Window::pollEvent()
{
    glfwPollEvents();
}

bool Window::processInput(sfml2::Event& event)
{
    event = sfml2::Event{}; // note: deleting event here will unblock key events flood
    bool status{false};

    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    status = setStatus(event);
    if (isMouseMoved() and not status)
    {
        event.mouseMove = sfml2::Event::MouseMoveEvent{::getCursorPos(window).x, ::getCursorPos(window).y};
        event.type = sfml2::Event::EventType::MouseMoved;
        gl_wrapper::Window::currentEvent  = nullptr;
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

} // gl_wrapper