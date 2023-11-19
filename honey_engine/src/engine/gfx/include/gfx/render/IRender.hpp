#pragma once

#include "gfx/render/RenderSettings.hpp"

namespace he
{
namespace gfx
{

namespace draw
{
class IDrawable;
}
namespace render
{

class IRender
{
public:
    virtual ~IRender() = default;

public:
    virtual void draw(he::gfx::draw::IDrawable&) = 0;
    virtual void draw(he::gfx::draw::IDrawable&, const he::gfx::render::RenderSettings&) = 0;
};
} // namespace render
} // namespace gfx
} // namespace he