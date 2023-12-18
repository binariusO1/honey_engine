#include "gfx/draw/IShape.hpp"

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
    , Trans2d(copy)
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
    m_vertexArrayNeedUpdate = Trans2d::setPosition(position);
    return m_vertexArrayNeedUpdate;
}


////////////////////////////////////////////////////////////
bool IShape::setOrigin(const he::gfx::geometry::Point2Df& point)
{
    m_vertexArrayNeedUpdate = Trans2d::setOrigin(point);
    return m_vertexArrayNeedUpdate;
}


////////////////////////////////////////////////////////////    
bool IShape::setRotation(const he::gfx::geometry::Angle& angle, const int)
{
    m_vertexArrayNeedUpdate = Trans2d::setRotation(angle);
    return m_vertexArrayNeedUpdate;
}


////////////////////////////////////////////////////////////
const he::gfx::geometry::Point2Df& IShape::getPosition() const
{
    return Trans2d::getPosition();
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
    return Trans2d::getOrigin();
}


////////////////////////////////////////////////////////////
const he::gfx::geometry::Angle& IShape::getRotation(const int) const
{
    return Trans2d::getRotation();
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