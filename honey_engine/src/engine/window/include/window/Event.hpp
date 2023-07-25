#pragma once

#include "window/Keyboard.hpp"
#include "window/Mouse.hpp"

namespace he
{
namespace window
{
class Event
{
public:
    struct SizeEvent
    {
        unsigned int width;     ///< New width, in pixels
        unsigned int height;    ///< New height, in pixels
    };

    struct KeyEvent
    {
        he::window::Keyboard::Key code;      ///< Code of the key that has been pressed
        bool          alt;              ///< Is the Alt key pressed?
        bool          control;          ///< Is the Control key pressed?
        bool          shift;            ///< Is the Shift key pressed?
        bool          system;           ///< Is the System key pressed?
    };

    struct TextEvent
    {
         std::uint32_t unicode;     ///< UTF-32 Unicode value of the character
    };

    struct MouseMoveEvent
    {
        double x;       ///< X position of the mouse pointer, relative to the left of the owner window
        double y;       ///< Y position of the mouse pointer, relative to the top of the owner window
    };

    struct MouseButtonAction
    {
        he::window::Mouse::Button button;    ///< Code of the button that has been pressed
        double           x;             ///< X position of the mouse pointer, relative to the left of the owner window
        double           y;             ///< Y position of the mouse pointer, relative to the top of the owner window
    };

    struct MouseWheelEvent
    {
        int delta;      ///< Number of ticks the wheel has moved (positive is up, negative is down)
        double x;       ///< X position of the mouse pointer, relative to the left of the owner window
        double y;       ///< Y position of the mouse pointer, relative to the top of the owner window
    };

    struct MouseWheelScrollEvent
    {
        he::window::Mouse::Wheel wheel;  ///< Which wheel (for mice with multiple ones)
        float        delta;         ///< Wheel offset (positive is up/left, negative is down/right). High-precision mice may use non-integral offsets.
        double          x;          ///< X position of the mouse pointer, relative to the left of the owner window
        double          y;          ///< Y position of the mouse pointer, relative to the top of the owner window
    };

    enum EventType
    {
        Closed,                 ///< The window requested to be closed (no data)
        Resized,                ///< The window was resized (data in event.size)
        LostFocus,              ///< The window lost the focus (no data)
        GainedFocus,            ///< The window gained the focus (no data)
        TextEntered,            ///< A character was entered (data in event.text)
        KeyPressed,             ///< A key was pressed (data in event.key)
        KeyReleased,            ///< A key was released (data in event.key)
        MouseWheelMoved,        ///< The mouse wheel was scrolled (data in event.mouseWheel) (deprecated)
        MouseWheelScrolled,     ///< The mouse wheel was scrolled (data in event.mouseWheelScroll)
        MouseButtonPressed,     ///< A mouse button was pressed (data in event.mouseButton)
        MouseButtonReleased,    ///< A mouse button was released (data in event.mouseButton)
        MouseMoved,             ///< The mouse cursor moved (data in event.mouseMove)
        MouseEntered,           ///< The mouse cursor entered the area of the window (no data)
        MouseLeft,              ///< The mouse cursor left the area of the window (no data)

        Count                   ///< Keep last -- the total number of event types
    };

    EventType type;     ///< Type of the event

    union
    {
        SizeEvent             size;                 ///< Size event parameters (Event::Resized)
        KeyEvent              key;                  ///< Key event parameters (Event::KeyPressed, Event::KeyReleased)
        TextEvent             text;                 ///< Text event parameters (Event::TextEntered)
        MouseMoveEvent        mouseMove;            ///< Mouse move event parameters (Event::MouseMoved)
        MouseButtonAction      mouseButton;         ///< Mouse button event parameters (Event::MouseButtonPressed, Event::MouseButtonReleased)
        MouseWheelEvent       mouseWheel;           ///< Mouse wheel event parameters (Event::MouseWheelMoved) (deprecated)
        MouseWheelScrollEvent mouseWheelScroll;     ///< Mouse wheel event parameters (Event::MouseWheelScrolled)
    };
};

inline const char* toString(const Event::EventType& type)
{
    switch(type)
    {
        case Event::EventType::Closed: return "Closed";
        case Event::EventType::Resized: return "Resized";
        case Event::EventType::LostFocus: return "LostFocus";
        case Event::EventType::GainedFocus: return "GainedFocus";
        case Event::EventType::TextEntered: return "TextEntered";
        case Event::EventType::KeyPressed: return "KeyPressed";
        case Event::EventType::KeyReleased: return "KeyReleased";
        case Event::EventType::MouseWheelMoved: return "MouseWheelMoved";
        case Event::EventType::MouseWheelScrolled: return "MouseWheelScrolled";
        case Event::EventType::MouseButtonPressed: return "MouseButtonPressed";
        case Event::EventType::MouseButtonReleased: return "MouseButtonReleased";
        case Event::EventType::MouseMoved: return "MouseMoved";
        case Event::EventType::MouseEntered: return "MouseEntered";
        case Event::EventType::MouseLeft: return "MouseLeft";
        case Event::EventType::Count: return "Count";
        default: return "None";
    }
}
} // namespace window
} // namespace he