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
    : ShapeRectangle(name, {static_cast<float>(texture->getSize().width), static_cast<float>(texture->getSize().height)})
{ 
    m_texture = texture;
    m_textureCoordsNeedUpdate = true;
}


////////////////////////////////////////////////////////////
Sprite::Sprite(const Sprite& copy)
    : m_texture{copy.m_texture}
    , ShapeRectangle(copy)
{
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
    m_textureCoordsNeedUpdate = true;
}


////////////////////////////////////////////////////////////
void Sprite::draw(he::gfx::render::Render& render, const he::gfx::render::RenderSettings& renderSettings)
{
    if (m_vertexArrayNeedUpdate)
    {
        update();
    }

    if (not Shape::getVertexArray().empty())
    {
        if (m_isFilledByColor)
        {
            render.drawVertexPrimitive(Shape::getVertexArray(), getTextureId(), getColor(), renderSettings);
        }
        else
        {
            render.drawVertex(Shape::getVertexArray(), getTextureId(), getColor(), renderSettings);
        }
    }
}


////////////////////////////////////////////////////////////
void Sprite::setColor(const he::gfx::Color& color)
{
    m_context.color = color;
    m_isFilledByColor = true;
    m_vertexArrayNeedUpdate = true;
}


////////////////////////////////////////////////////////////
void Sprite::unsetColor()
{
    m_isFilledByColor = false;
    m_vertexArrayNeedUpdate = true;
}


////////////////////////////////////////////////////////////
void Sprite::update()
{
    Shape::update();

    updateTextureCoords();
    m_textureCoordsNeedUpdate = false;
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