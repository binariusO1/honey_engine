#pragma once

#include "window/events/Event.hpp"

namespace he
{
namespace gfx
{
namespace draw
{
class IButton
{
public:
    virtual ~IButton() = default;

public:
    virtual bool onMauseButtonPressed(const he::window::Event::MouseButtonAction&) = 0;
};
} // namespace draw
} // namespace gfx
} // namespace he