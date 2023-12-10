#pragma once

namespace he
{
namespace gfx
{
enum class ButtonState
{
    Idle,
    Touched,
    Clicked,
    Checked,
    Pushed
};

inline const char* toString(const ButtonState& state)
{
    switch(state)
    {
        case ButtonState::Idle: return "Idle";
        case ButtonState::Touched: return "Touched";
        case ButtonState::Clicked: return "Clicked";
        case ButtonState::Checked: return "Checked";
        case ButtonState::Pushed: return "Pushed";
        default: return "none";
    }
}

} // namespace gfx
} // namespace he