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
bool IShape::setPosition(const he::gfx::geometry::Point2Df& position)
{
    m_vertexArrayNeedUpdate = Transformable2d::setPosition(position);
    return m_vertexArrayNeedUpdate;
}


////////////////////////////////////////////////////////////
bool IShape::setOrigin(const he::gfx::geometry::Point2Df& point)
{
    m_vertexArrayNeedUpdate = Transformable2d::setOrigin(point);
    return m_vertexArrayNeedUpdate;
}


////////////////////////////////////////////////////////////    
bool IShape::setRotation(const he::gfx::geometry::Angle& angle)
{
    m_vertexArrayNeedUpdate = Transformable2d::setRotation(angle);
    return m_vertexArrayNeedUpdate;
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
void IShape::setLayerName(const std::string& name)
{
    m_context.layerName = name;
}


////////////////////////////////////////////////////////////
const std::string IShape::getLayerName() const
{
    return m_context.layerName;
}


////////////////////////////////////////////////////////////
void IShape::setName(const std::string& name)
{
    m_context.name = name;
}


////////////////////////////////////////////////////////////
const he::gfx::VertexArray2d& IShape::getVertexArray() const
{
    return m_vertexArray;
}


////////////////////////////////////////////////////////////
const he::gfx::geometry::Point2Df& IShape::getOrigin() const
{
    return Transformable2d::getOrigin();
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
gfx::OriginPosition IShape::getOriginPosition() const
{
    return m_context.originPosition;
}
} // namespace draw
} // namespace gfx
} // namespace he