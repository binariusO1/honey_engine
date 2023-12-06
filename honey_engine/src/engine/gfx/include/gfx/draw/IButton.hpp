#pragma once

#include "gfx/geometry/Point2d.hpp"
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
    virtual bool onMouseCursorMoved(const he::window::Event::MouseMoveEvent&) = 0;
};
} // namespace draw
} // namespace gfx
} // namespace he