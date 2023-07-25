#pragma once

#include "gfx/render/IRender.hpp"

namespace he
{
namespace gfx
{
namespace render
{
class IRenderable
{
public:
    virtual ~IRenderable() = default;

    virtual void render(gfx::render::IRender&) = 0;
}; 
} // namespace render
} // namespace gfx
} // namespace he