#include "gfx/draw/IShape.hpp"

#include "gfx/render/Render.hpp"
#include "logger/Logger.hpp"

namespace 
{
    // TODO make this function common not inside thic .cpp file
void convertPixelPointToVertexPoint(he::gfx::geometry::Point2Df& point, const int screenWidth, const int screenHeight)
{
    const float SCREEN_SCALE = 2.0f;
    const float OFFSET_ORIGIN_SCREEN = -1.0f;

    if (screenWidth > 0 and screenHeight > 0)
    {
        float x = (point.x / static_cast<float>(screenWidth)*SCREEN_SCALE) + OFFSET_ORIGIN_SCREEN;
        float y = (point.y / static_cast<float>(screenHeight)*SCREEN_SCALE) + OFFSET_ORIGIN_SCREEN;
        point.x = x;
        point.y = y;
        return;
    }
    LOG_WARNING << "No convert. Window width or height is equal to 0!";
}
} // namespace
namespace he
{
namespace gfx
{
namespace draw
{
////////////////////////////////////////////////////////////
IShape::IShape(const std::string& name) : m_context(name)
{
}


////////////////////////////////////////////////////////////
IShape::IShape(const IShape& copy)
    : m_vertexArray{copy.m_vertexArray}
    , m_context{copy.m_context} // TODO: warning - during copy, unique string will be not unique. maybe name should be: hash+name ?
    , m_vertexArrayNeedUpdate{copy.m_vertexArrayNeedUpdate}
    , geometry::transform::Transformable2d(copy)
{
}


////////////////////////////////////////////////////////////
IShape::~IShape() = default;


////////////////////////////////////////////////////////////
void IShape::setColor(const he::gfx::Color& color)
{
    if (color != m_context.color)
    {
        m_context.color = color;
    
        for (std::size_t i = 0; i < m_vertexArray.size(); ++i)
        {
            m_vertexArray[i].color = color;
        }
    }
}


////////////////////////////////////////////////////////////
const he::gfx::Color IShape::getColor() const
{
    return m_context.color;
}


////////////////////////////////////////////////////////////
void IShape::setPosition(const he::gfx::geometry::Point2Df& position)
{
    Transformable2d::setPosition(position);
    m_vertexArrayNeedUpdate = true;
}


////////////////////////////////////////////////////////////
const he::gfx::geometry::Point2Df& IShape::getPosition() const
{
    return Transformable2d::getPosition();
}


////////////////////////////////////////////////////////////
const std::string IShape::getName() const
{
    return m_context.name;
}


////////////////////////////////////////////////////////////
const he::gfx::VertexArray2d& IShape::getVertexArray() const
{
    return m_vertexArray;
}


////////////////////////////////////////////////////////////
void IShape::setOrigin(const he::gfx::geometry::Point2Df& point)
{
    Transformable2d::setOrigin(point);
    m_vertexArrayNeedUpdate = true;
}


////////////////////////////////////////////////////////////
const he::gfx::geometry::Point2Df& IShape::getOrigin() const
{
    return Transformable2d::getOrigin();
}


////////////////////////////////////////////////////////////    
void IShape::setRotation(const he::gfx::geometry::Angle& angle)
{
    Transformable2d::setRotation(angle);
    m_vertexArrayNeedUpdate = true;
}


////////////////////////////////////////////////////////////
const he::gfx::geometry::Angle& IShape::getRotation() const
{
    return Transformable2d::getRotation();
}


////////////////////////////////////////////////////////////
void IShape::setOriginPosition(const he::gfx::OriginPosition& originPosition)
{
    switch (originPosition)
    {
    case he::gfx::OriginPosition::any:
        break;
    case he::gfx::OriginPosition::leftDown:
        setOrigin({0.0, 0.0});
        break;
    case he::gfx::OriginPosition::center:
        setOriginInCenter();
        break;
    default:
        break;
    }
}


////////////////////////////////////////////////////////////
void IShape::update()
{
    updateVertexArray();
    m_vertexArrayNeedUpdate = false;
}


////////////////////////////////////////////////////////////
void IShape::checkIfVertexArrayNeedUpdateThenUpdate()
{
    if (m_vertexArrayNeedUpdate)
    {
        update();
    }
}
} // namespace draw
} // namespace gfx
} // namespace he