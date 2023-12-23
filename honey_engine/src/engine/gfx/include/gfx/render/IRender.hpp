#pragma once

#include "gfx/render/RenderSettings.hpp"
#include "gfx/render/TransformMatrix.hpp"

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
    virtual void draw(he::gfx::draw::IDrawable&, TransformMatrix&) = 0;
    virtual void draw(he::gfx::draw::IDrawable&, const he::gfx::render::RenderSettings&, TransformMatrix&) = 0;
    virtual void render() = 0;
};
} // namespace render
} // namespace gfx
} // namespace he