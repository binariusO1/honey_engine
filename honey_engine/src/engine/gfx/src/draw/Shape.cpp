#include "gfx/draw/Shape.hpp"

#include "gfx/render/Render.hpp"

namespace he
{
namespace gfx
{
namespace draw
{
////////////////////////////////////////////////////////////
Shape::Shape(const std::string& name, const geometry::figures::Figure& figure)   
    : IShape(name)
    , m_figure{figure}
{
    m_vertexArrayNeedUpdate = true;
}


////////////////////////////////////////////////////////////
Shape::Shape(const Shape& copy, const geometry::figures::Figure& figure)
    : IShape(copy)
    , m_figure{figure}
    , m_closedVertexArray{copy.m_closedVertexArray}
{
}


////////////////////////////////////////////////////////////
Shape::~Shape() = default;


////////////////////////////////////////////////////////////
bool Shape::isPointInside(const geometry::Point2Df& point)
{
    he::gfx::geometry::Point2Df pointToCheck{point};
    inverseTransformPoint(pointToCheck);
    return m_figure.isPointInside(pointToCheck);
}


////////////////////////////////////////////////////////////
void Shape::setColor(const he::gfx::Color& color)
{
    IShape::setColor(color);
}


////////////////////////////////////////////////////////////
bool Shape::setOrigin(const he::gfx::geometry::Point2Df& origin)
{
    if (origin == he::gfx::geometry::Point2Df{0.0, 0.0})
    {
         m_context.originPosition = he::gfx::OriginPosition::leftDown;
    }
    else if(origin == he::gfx::geometry::Point2Df{m_figure.getCenterPoint().x , m_figure.getCenterPoint().y})
    {
         m_context.originPosition = he::gfx::OriginPosition::center;
    }
    else
    {
         m_context.originPosition = he::gfx::OriginPosition::any;
    }

    return IShape::setOrigin(origin);
}


////////////////////////////////////////////////////////////
void Shape::setOriginInCenter()
{
    m_context.originPosition = he::gfx::OriginPosition::center;
    setOrigin({m_figure.getCenterPoint().x , m_figure.getCenterPoint().y});
}

////////////////////////////////////////////////////////////
bool Shape::setPosition(const he::gfx::geometry::Point2Df& position)
{
    return IShape::setPosition(position);
}


////////////////////////////////////////////////////////////    
bool Shape::setRotation(const he::gfx::geometry::Angle& angle)
{
    return IShape::setRotation(angle);
}


////////////////////////////////////////////////////////////
const he::gfx::Color Shape::getColor() const
{
    return IShape::getColor();
}


////////////////////////////////////////////////////////////
const std::string Shape::getName() const
{
    return IShape::getName();
}


////////////////////////////////////////////////////////////
const he::gfx::geometry::Point2Df& Shape::getOrigin() const
{
    return IShape::getOrigin();
}


////////////////////////////////////////////////////////////
const he::gfx::geometry::Point2Df& Shape::getPosition() const
{
    return IShape::getPosition();
}


////////////////////////////////////////////////////////////
const he::gfx::geometry::Angle& Shape::getRotation() const
{
    return IShape::getRotation();
}


////////////////////////////////////////////////////////////
const he::gfx::VertexArray2d& Shape::getVertexArray() const // TODO : do usunięcia, ponieważ korzystam tylko z m_vertexArray (poza tym nie trzeba dawać na zewnątrz) 
{
    return IShape::getVertexArray();
}


////////////////////////////////////////////////////////////
void Shape::closeVertexArray()
{
    m_closedVertexArray = true;
    m_vertexArrayNeedUpdate = true;
}


////////////////////////////////////////////////////////////
void Shape::openVertexArray()
{
    m_closedVertexArray = false;
    m_vertexArrayNeedUpdate = true;
}


////////////////////////////////////////////////////////////
void Shape::draw(he::gfx::render::Render& render, const he::gfx::render::RenderSettings& renderSettings)
{
    if (m_vertexArrayNeedUpdate)
    {
        updateVertexArray();
    }

    if (not m_vertexArray.empty())
    {
        render.drawVertex2d(m_vertexArray, 0, getColor(), renderSettings, getTransform().getMatrix(), m_vertexArrayNeedUpdate);
    }

    m_vertexArrayNeedUpdate = false;
}


////////////////////////////////////////////////////////////
void Shape::setOriginPosition(const he::gfx::OriginPosition& originPosition)
{
    IShape::setOriginPosition(originPosition);
    m_vertexArrayNeedUpdate = true;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// PRIVATE
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
void Shape::updateVertexArray()
{
    m_vertexArray.clear();

    for (std::size_t i = 0 ; i < m_figure.getNumOfPoints() ; ++i)
    {
        m_vertexArray.push_back({m_figure.getPoint(i), m_context.color});
    }

    if (m_closedVertexArray)
    {
        m_vertexArray.push_back(m_vertexArray[0]);
    }
}

} // namespace draw
} // namespace gfx
} // namespace he