////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2018 Laurent Gomila (laurent@sfml-dev.org)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
////////////////////////////////////////////////////////////

#pragma once

#include <cstdint>
// #include <SFML/Config.hpp>
#include "sfml2/window/Keyboard.hpp"
#include "sfml2/window/Mouse.hpp"

namespace sfml2
{
class Event
{
public:
    struct SizeEvent
    {
        unsigned int width;  ///< New width, in pixels
        unsigned int height; ///< New height, in pixels
    };

    struct KeyEvent
    {
        sfml2::Keyboard::Key code;    ///< Code of the key that has been pressed
        bool          alt;     ///< Is the Alt key pressed?
        bool          control; ///< Is the Control key pressed?
        bool          shift;   ///< Is the Shift key pressed?
        bool          system;  ///< Is the System key pressed?
    };

    struct TextEvent
    {
         std::uint32_t unicode; ///< UTF-32 Unicode value of the character
    };

    struct MouseMoveEvent
    {
        double x; ///< X position of the mouse pointer, relative to the left of the owner window
        double y; ///< Y position of the mouse pointer, relative to the top of the owner window
    };

    struct MouseButtonAction
    {
        sfml2::Mouse::Button button; ///< Code of the button that has been pressed
        double           x;      ///< X position of the mouse pointer, relative to the left of the owner window
        double           y;      ///< Y position of the mouse pointer, relative to the top of the owner window
    };

    struct MouseWheelEvent
    {
        int delta; ///< Number of ticks the wheel has moved (positive is up, negative is down)
        double x;     ///< X position of the mouse pointer, relative to the left of the owner window
        double y;     ///< Y position of the mouse pointer, relative to the top of the owner window
    };

    struct MouseWheelScrollEvent
    {
        sfml2::Mouse::Wheel wheel; ///< Which wheel (for mice with multiple ones)
        float        delta; ///< Wheel offset (positive is up/left, negative is down/right). High-precision mice may use non-integral offsets.
        double          x;     ///< X position of the mouse pointer, relative to the left of the owner window
        double          y;     ///< Y position of the mouse pointer, relative to the top of the owner window
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

    EventType type; ///< Type of the event

    union
    {
        SizeEvent             size;              ///< Size event parameters (Event::Resized)
        KeyEvent              key;               ///< Key event parameters (Event::KeyPressed, Event::KeyReleased)
        TextEvent             text;              ///< Text event parameters (Event::TextEntered)
        MouseMoveEvent        mouseMove;         ///< Mouse move event parameters (Event::MouseMoved)
        MouseButtonAction      mouseButton;       ///< Mouse button event parameters (Event::MouseButtonPressed, Event::MouseButtonReleased)
        MouseWheelEvent       mouseWheel;        ///< Mouse wheel event parameters (Event::MouseWheelMoved) (deprecated)
        MouseWheelScrollEvent mouseWheelScroll;  ///< Mouse wheel event parameters (Event::MouseWheelScrolled)
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

struct EventHasher
{
    std::size_t operator()(const sfml2::Event& event) const
    {
        std::size_t h1 = std::hash<std::string>{}(toString(event.type));

        if (event.type == sfml2::Event::EventType::MouseButtonPressed)
        {
            std::size_t h2 = std::hash<std::string>{}(toString(event.mouseButton.button));
            return h1 ^ (h2 << 1);
        }
        return h1 << 1;
    }
};

inline bool operator ==(const sfml2::Event& left, const sfml2::Event& right)
{
    return (left.type == right.type) && (left.mouseButton.button == right.mouseButton.button);
}
} // namespace sfml