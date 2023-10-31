#include "gfx/draw/Button.hpp"

#include "gfx/render/Render.hpp"
#include "logger/Logger.hpp"

namespace he
{
namespace gfx
{
namespace draw
{
////////////////////////////////////////////////////////////
Button::Button(const std::string& name, const std::shared_ptr<he::gfx::render::ITexture>& texture) : Sprite(name, texture)
{
    //
}


////////////////////////////////////////////////////////////
void Button::process_event(const he::window::Event& event)
{
    //
}


////////////////////////////////////////////////////////////
void Button::draw(const he::gfx::render::Render& render, const he::gfx::render::RenderSettings& renderSettings) const
{
    if (not Shape::getVertexArray().empty())
    {
        render.drawVertex(Shape::getVertexArray(), getTextureId(), getColor(), renderSettings);
    }
}
} // namespace draw
} // namespace gfx
} // namespace he