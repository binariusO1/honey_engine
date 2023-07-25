#include "window/window/Window.hpp"
#include "logger/Logger.hpp"

namespace
{
bool checkPos(he::window::Point2Dd oldPos, he::window::Point2Dd newPos)
{
    return ((oldPos.first == newPos.first) and (oldPos.second == newPos.second));
}

} // namespace

namespace he
{
namespace window
{
////////////////////////////////////////////////////////////
void Window::update()
{
    m_glfwWrapper->pollEvents();
}


////////////////////////////////////////////////////////////
void Window::update(he::window::Event& event)
{
    m_glfwWrapper->pollEvents();
    if (not process_event(event))
    {
        m_currentMousePosition = m_glfwWrapper->getCursorPos(); 
        fillEvent(event);
    }
    m_currentEvent = event;
}


////////////////////////////////////////////////////////////
const he::window::Event& Window::getEvent() const
{
    return m_currentEvent;
}


////////////////////////////////////////////////////////////
void Window::addEventInputListener(IEventInputListener* listener)
{
    m_eventInputListeners.push_back(listener);
}


////////////////////////////////////////////////////////////
void Window::removeEventInputListener(IEventInputListener* listener)
{
    m_eventInputListeners.remove(listener);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// PRIVATE
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
bool Window::process_event(he::window::Event& event)
{
    switch(event.type)
    {
        case Event::EventType::closed:
            closeWindow();
            return true;
        case Event::EventType::resized:
            resize(event.size.width, event.size.height, event.size.state);
            return true;
        case Event::EventType::lostFocus:
            LOG_WARNING << "Not implemented";
            return true;
        case Event::EventType::gainedFocus:
            LOG_WARNING << "Not implemented";
            return true;
        case Event::EventType::textEntered:
            LOG_WARNING << "Not implemented yet";
            return true;
        case Event::EventType::none:
        case Event::EventType::keyPressed:
        case Event::EventType::keyReleased:
        case Event::EventType::mouseWheelMoved:
        case Event::EventType::mouseWheelScrolled:
        case Event::EventType::mouseButtonPressed:
        case Event::EventType::mouseButtonReleased:
        case Event::EventType::mouseMoved:
        case Event::EventType::mouseEntered:
        case Event::EventType::mouseLeft:
        default:
            return false;
    }
}


////////////////////////////////////////////////////////////
void Window::handleKeyEvent(int key, int scancode, int action, int mods)
{
    using namespace he::window;
    he::window::Event event{};
    bool isAltPressed{false};
    bool isCtrlPressed{false};
    bool isShiftPressed{false};
    bool isSystemPressed{false};

    if (scancode > 0 or mods > 0)
    {
    }

    if (key == GLFW_KEY_LEFT_ALT or key == GLFW_KEY_RIGHT_ALT)
    {
        isAltPressed = true;
    }
    if (key == GLFW_KEY_LEFT_CONTROL or key == GLFW_KEY_RIGHT_CONTROL)
    {
        isCtrlPressed = true;
    }
    if (key == GLFW_KEY_LEFT_SHIFT or key == GLFW_KEY_RIGHT_SHIFT)
    {
        isShiftPressed = true;
    }
    if (key == GLFW_KEY_LEFT_SUPER or key == GLFW_KEY_RIGHT_SUPER)
    {
        isSystemPressed = true;
    }

    event.key = he::window::Event::KeyEvent{static_cast<he::window::Keyboard::Key>(key), isAltPressed, isCtrlPressed, isShiftPressed, isSystemPressed};

    if (action == GLFW_PRESS)
    {
        event.type = he::window::Event::EventType::keyPressed;
    }
    else if(action == GLFW_REPEAT)
    {
    }
    else if(action == GLFW_RELEASE)
    {
        event.type = he::window::Event::EventType::keyReleased;
    }
    notifyListeners(event);
    m_currentEvent = event;
}


////////////////////////////////////////////////////////////
void Window::handleMouseEvent(int button, int action)
{
    using namespace he::window;
    he::window::Event event{};
    m_currentMousePosition = m_glfwWrapper->getCursorPos(); 
    if (button == GLFW_MOUSE_BUTTON_LEFT)
    {
        event.mouseButton = he::window::Event::MouseButtonAction{he::window::Mouse::Button::Left, m_currentMousePosition.first, m_currentMousePosition.second};
    }
    else if (button == GLFW_MOUSE_BUTTON_RIGHT)
    {
        event.mouseButton = he::window::Event::MouseButtonAction{he::window::Mouse::Button::Right, m_currentMousePosition.first, m_currentMousePosition.second};
    }

    if (action == GLFW_PRESS)
    {
        event.type = he::window::Event::EventType::mouseButtonPressed;
    }
    else if(action == GLFW_RELEASE)
    {
        event.type = he::window::Event::EventType::mouseButtonReleased;
    }
    m_currentEvent = event;
    notifyListeners(event);
}


////////////////////////////////////////////////////////////
void Window::handleScrollEvent(double xoffset, double yoffset)
{
    using namespace he::window;
    he::window::Event event{};
    if (xoffset != 0)
    {
        event.mouseWheelScroll.wheel = he::window::Mouse::Wheel::HorizontalWheel;
        event.mouseWheelScroll.x = xoffset;
    }

    if (yoffset != 0)
    {
        event.mouseWheelScroll.wheel = he::window::Mouse::Wheel::VerticalWheel;
        event.mouseWheelScroll.y = yoffset;
    }
    event.type = he::window::Event::EventType::mouseWheelScrolled;
    m_currentEvent = event;
    notifyListeners(event);
}


////////////////////////////////////////////////////////////
void Window::notifyListeners(const he::window::Event& event)
{
    auto it = m_eventInputListeners.begin();
    while (it != m_eventInputListeners.end())
    {
        IEventInputListener* element = *it;
        if (element != nullptr) 
        {
            element->process_event(event);
            ++it;
        } 
        else 
        {
            it = m_eventInputListeners.erase(it);
        }
    }
}


////////////////////////////////////////////////////////////
void Window::fillEvent(he::window::Event& event)
{
    event = he::window::Event{};

    if (m_currentEvent.type > 0)
    {
        event = m_currentEvent;
        return;
    }

    if (isMouseMoved() and event.type == 0)
    {
        event.mouseMove = he::window::Event::MouseMoveEvent{m_currentMousePosition.first, m_currentMousePosition.second};
        event.type = he::window::Event::EventType::mouseMoved;
        m_currentEvent = event;
    }
}


////////////////////////////////////////////////////////////
bool Window::isMouseMoved()
{
    const Point2Dd newPos = std::make_pair(m_currentMousePosition.first, m_currentMousePosition.second);
    const bool isPositionEqual = checkPos(m_oldPos, newPos);

    if (not isPositionEqual)
    {
        m_oldPos = newPos;
        return true;
    }
    return false;
}
} // namespace he
} // namespace window