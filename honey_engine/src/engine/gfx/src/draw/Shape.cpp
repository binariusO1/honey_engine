#include "gfx/draw/Shape.hpp"

#include "gfx/render/Render.hpp"

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
    }
}
} // namespace
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
Shape::Shape(const Shape& copy)
    : IShape(copy)
    , m_figure{copy.m_figure}
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
void Shape::setOrigin(const he::gfx::geometry::Point2Df& origin)
{
    IShape::setOrigin(origin);

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
    m_vertexArrayNeedUpdate = true;
}


////////////////////////////////////////////////////////////
void Shape::setOriginInCenter()
{
    setOrigin({m_figure.getCenterPoint().x , m_figure.getCenterPoint().y});
    m_vertexArrayNeedUpdate = true;
    m_context.originPosition = he::gfx::OriginPosition::center;
}

////////////////////////////////////////////////////////////
void Shape::setPosition(const he::gfx::geometry::Point2Df& position)
{
    IShape::setPosition(position);
}


////////////////////////////////////////////////////////////    
void Shape::setRotation(const he::gfx::geometry::Angle& angle)
{
    IShape::setRotation(angle);
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
const he::gfx::VertexArray2d& Shape::getVertexArray() const
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
        update();
    }

    if (not getVertexArray().empty())
    {
        render.drawVertexPrimitive(getVertexArray(), 0, getColor(), renderSettings);
    }
}


////////////////////////////////////////////////////////////
void Shape::update()
{
    IShape::update();
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
        auto point = m_figure.getPoint(i);
        transformPoint(point);
        // TODO: remove 1200, 800 - screen width and height
        convertPixelPointToVertexPoint(point, 1200, 800);
        m_vertexArray.push_back({point, m_context.color});
    }

    if (m_closedVertexArray)
    {
        m_vertexArray.push_back(m_vertexArray[0]);
    }
}

} // namespace draw
} // namespace gfx
} // namespace he