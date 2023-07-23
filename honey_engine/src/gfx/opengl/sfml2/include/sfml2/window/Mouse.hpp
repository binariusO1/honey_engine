#pragma once

// #include <SFML/Window/Export.hpp>
// #include <SFML/System/Vector2.hpp>

namespace sfml2
{
class Window;

class Mouse
{
public:
    enum Button
    {
        Left,       ///< The left mouse button
        Right,      ///< The right mouse button
        Middle,     ///< The middle (wheel) mouse button
        XButton1,   ///< The first extra mouse button
        XButton2,   ///< The second extra mouse button

        ButtonCount ///< Keep last -- the total number of mouse buttons
    };

    enum Wheel
    {
        VerticalWheel,  ///< The vertical mouse wheel
        HorizontalWheel ///< The horizontal mouse wheel
    };

    // static bool isButtonPressed(Button button);

    // static Vector2i getPosition();

    // static Vector2i getPosition(const Window& relativeTo);

    // static void setPosition(const Vector2i& position);

    // static void setPosition(const Vector2i& position, const Window& relativeTo);
};

inline const char* toString(const Mouse::Button& button)
{
    switch(button)
    {
        case Mouse::Button::Left: return "Left";
        case Mouse::Button::Right: return "Right";
        case Mouse::Button::Middle: return "Middle";
        case Mouse::Button::XButton1: return "XButton1";
        case Mouse::Button::XButton2: return "XButton2";
        case Mouse::Button::ButtonCount: return "ButtonCount";
        default: return "None";
    }
}

inline const char* toString(const Mouse::Wheel& wheel)
{
    switch(wheel)
    {
        case Mouse::Wheel::VerticalWheel: return "VerticalWheel";
        case Mouse::Wheel::HorizontalWheel: return "HorizontalWheel";
        default: return "None";
    }
}

} // namespace sfml