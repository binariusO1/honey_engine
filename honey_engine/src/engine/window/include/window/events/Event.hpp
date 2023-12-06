#pragma once

#include <cstdint>
#include <functional>
#include <string>
#include "Keyboard.hpp"
#include "Mouse.hpp"
#include "WindowState.hpp"

namespace he
{
namespace window
{
class Event
{
public:
    struct SizeEvent
    {
        he::window::WindowState state;
        unsigned int width;                 ///< New width, in pixels
        unsigned int height;                ///< New height, in pixels
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
        int x;       ///< X position of the mouse pointer, relative to the left of the owner window
        int y;       ///< Y position of the mouse pointer, relative to the top of the owner window
    };

    struct MouseButtonAction
    {
        he::window::Mouse::Button button;    ///< Code of the button that has been pressed
        int           x;             ///< X position of the mouse pointer, relative to the left of the owner window
        int           y;             ///< Y position of the mouse pointer, relative to the top of the owner window
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
        none,
        closed,                 ///< The window requested to be closed (no data)
        resized,                ///< The window was resized (data in event.size)
        lostFocus,              ///< The window lost the focus (no data)
        gainedFocus,            ///< The window gained the focus (no data)
        textEntered,            ///< A character was entered (data in event.text)
        keyPressed,             ///< A key was pressed (data in event.key)
        keyReleased,            ///< A key was released (data in event.key)
        mouseWheelMoved,        ///< The mouse wheel was scrolled (data in event.mouseWheel) (deprecated)
        mouseWheelScrolled,     ///< The mouse wheel was scrolled (data in event.mouseWheelScroll)
        mouseButtonPressed,     ///< A mouse button was pressed (data in event.mouseButton)
        mouseButtonReleased,    ///< A mouse button was released (data in event.mouseButton)
        mouseMoved,             ///< The mouse cursor moved (data in event.mouseMove)
        mouseEntered,           ///< The mouse cursor entered the area of the window (no data)
        mouseLeft,              ///< The mouse cursor left the area of the window (no data)
    };

    EventType type;

    union
    {
        SizeEvent             size;                 ///< Size event parameters (Event::resized)
        KeyEvent              key;                  ///< Key event parameters (Event::keyPressed, Event::keyReleased)
        TextEvent             text;                 ///< Text event parameters (Event::textEntered)
        MouseMoveEvent        mouseMove;            ///< Mouse move event parameters (Event::mouseMoved)
        MouseButtonAction     mouseButton;          ///< Mouse button event parameters (Event::mouseButtonPressed, Event::mouseButtonReleased)
        MouseWheelScrollEvent mouseWheelScroll;     ///< Mouse wheel event parameters (Event::mouseWheelScrolled)
    };
};

inline const char* toString(const Event::EventType& type)
{
    switch(type)
    {
        case Event::EventType::none: return "none";
        case Event::EventType::closed: return "closed";
        case Event::EventType::resized: return "resized";
        case Event::EventType::lostFocus: return "lostFocus";
        case Event::EventType::gainedFocus: return "gainedFocus";
        case Event::EventType::textEntered: return "textEntered";
        case Event::EventType::keyPressed: return "keyPressed";
        case Event::EventType::keyReleased: return "keyReleased";
        case Event::EventType::mouseWheelMoved: return "mouseWheelMoved";
        case Event::EventType::mouseWheelScrolled: return "mouseWheelScrolled";
        case Event::EventType::mouseButtonPressed: return "mouseButtonPressed";
        case Event::EventType::mouseButtonReleased: return "mouseButtonReleased";
        case Event::EventType::mouseMoved: return "mouseMoved";
        case Event::EventType::mouseEntered: return "mouseEntered";
        case Event::EventType::mouseLeft: return "mouseLeft";
        default: return "None";
    }
}

struct EventHasher
{
    std::size_t operator()(const Event& event) const
    {
        std::size_t h1 = std::hash<std::string>{}(toString(event.type));

        if (event.type == Event::EventType::mouseButtonPressed)
        {
            std::size_t h2 = std::hash<std::string>{}(toString(event.mouseButton.button));
            return h1 ^ (h2 << 1);
        }
        return h1 << 1;
    }
};

inline bool operator ==(const Event& left, const Event& right)
{
    return (left.type == right.type) and (left.key.code == right.key.code);
}
} // namespace window
} // namespace he