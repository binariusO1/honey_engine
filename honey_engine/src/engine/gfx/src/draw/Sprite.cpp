#include "gfx/draw/Sprite.hpp"

#include "gfx/geometry/figures/Rectangle.hpp"
#include "gfx/geometry/Size2d.hpp"
#include "gfx/graphic/ITexture.hpp"
#include "gfx/render/Render.hpp"
#include "logger/Logger.hpp"

namespace he
{
namespace gfx
{
namespace draw
{
////////////////////////////////////////////////////////////
Sprite::Sprite(const std::string& name, const std::shared_ptr<he::gfx::render::ITexture>& texture) 
    : Shape(name, std::make_shared<he::gfx::geometry::figures::Rectangle>(he::gfx::geometry::Size2Df{static_cast<float>(texture->getSize().width), static_cast<float>(texture->getSize().height)}))
{ 
    m_texture = texture;
    updateTextureCoords();
}


////////////////////////////////////////////////////////////
Sprite::~Sprite() = default;


////////////////////////////////////////////////////////////
unsigned int Sprite::getTextureId() const
{
    return m_texture->getTextureId();
}


////////////////////////////////////////////////////////////
void Sprite::setPosition(const he::gfx::geometry::Point2Df& position)
{
    Shape::setPosition(position);
    updateTextureCoords();
}


////////////////////////////////////////////////////////////
void Sprite::draw(he::gfx::render::Render& render, he::gfx::render::RenderSettings& renderSettings) const
{
    if (not Shape::getVertexArray().empty())
    {
        render.drawVertex(Shape::getVertexArray(), getTextureId(), getColor(), renderSettings);
    }
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// PRIVATE
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
void Sprite::updateTextureCoords()
{
    m_vertexArray[0].texCoords = he::gfx::geometry::Point2Df{0.0, 0.0};
    m_vertexArray[1].texCoords = he::gfx::geometry::Point2Df{1.0, 0.0};
    m_vertexArray[2].texCoords = he::gfx::geometry::Point2Df{1.0, 1.0};
    m_vertexArray[3].texCoords = he::gfx::geometry::Point2Df{0.0, 1.0};
}
} // namespace draw
} // namespace gfx
} // namespace he