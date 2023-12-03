#pragma once

#include <string>

namespace he
{
namespace gfx
{
namespace render
{
class Render;
struct RenderSettings;
} // namespace render
namespace draw
{
class IDrawable
{
public:
    virtual ~IDrawable() = default;

public:
    virtual const std::string getName() const = 0;
    virtual void setName(const std::string&) = 0;
    virtual void draw(he::gfx::render::Render&, const he::gfx::render::RenderSettings&) = 0;
};
} // namespace draw
} // namespace gfx
} // namespace he