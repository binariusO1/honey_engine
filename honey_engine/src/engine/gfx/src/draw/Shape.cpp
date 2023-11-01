#include "gfx/draw/Shape.hpp"

#include "gfx/render/Render.hpp"
#include "logger/Logger.hpp"

namespace 
{
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
Shape::Shape(const std::string& name, const std::shared_ptr<he::gfx::geometry::figures::Figure>& figure)
    : m_figure{figure}
    , m_context(name)
{
    updateVertexArray();
}


////////////////////////////////////////////////////////////
Shape::Shape(const std::string& name, 
            const std::shared_ptr<he::gfx::geometry::figures::Figure>& figure, 
            const he::gfx::Color& color, 
            const he::gfx::geometry::Point2Df& position,
            const he::gfx::OriginPosition& originPosition)
    : m_figure{figure}
    , m_context(name)
{
    m_context.color = color;
    m_context.originPosition = originPosition;
    setPosition(position);
    setOriginPosition(originPosition);
}


////////////////////////////////////////////////////////////
Shape::~Shape() = default;


////////////////////////////////////////////////////////////
bool Shape::isPointInside(const geometry::Point2Df& point)
{
    he::gfx::geometry::Point2Df pointToCheck{point};
    inverseTransformPoint(pointToCheck);
    return m_figure->isPointInside(pointToCheck);
}


////////////////////////////////////////////////////////////
void Shape::setColor(const he::gfx::Color& color)
{
    m_context.color = color;
    updateVertexArray();
}


////////////////////////////////////////////////////////////
void Shape::setOrigin(const he::gfx::geometry::Point2Df& origin)
{
    Transformable2d::setOrigin(origin);
    updateVertexArray();
    if (origin == he::gfx::geometry::Point2Df{0.0, 0.0})
    {
         m_context.originPosition = he::gfx::OriginPosition::leftDown;
    }
    else if(origin == he::gfx::geometry::Point2Df{m_figure->getCenterPoint().x , m_figure->getCenterPoint().y})
    {
         m_context.originPosition = he::gfx::OriginPosition::center;
    }
    else
    {
         m_context.originPosition = he::gfx::OriginPosition::any;
    }
}


////////////////////////////////////////////////////////////
void Shape::setOriginInCenter()
{
    setOrigin({m_figure->getCenterPoint().x , m_figure->getCenterPoint().y});
    updateVertexArray();
    m_context.originPosition = he::gfx::OriginPosition::center;
}

////////////////////////////////////////////////////////////
void Shape::setPosition(const he::gfx::geometry::Point2Df& position)
{
    Transformable2d::setPosition(position);
    updateVertexArray();
}


////////////////////////////////////////////////////////////    
void Shape::setRotation(const he::gfx::geometry::Angle& angle)
{
    Transformable2d::setRotation(angle);
    updateVertexArray();
}


////////////////////////////////////////////////////////////
he::gfx::Color Shape::getColor() const
{
    return m_context.color;
}


////////////////////////////////////////////////////////////
const std::string Shape::getName() const
{
    return m_context.name;
}


////////////////////////////////////////////////////////////
const he::gfx::geometry::Point2Df& Shape::getOrigin() const
{
    return Transformable2d::getOrigin();
}


////////////////////////////////////////////////////////////
const he::gfx::geometry::Point2Df& Shape::getPosition() const
{
    return Transformable2d::getPosition();
}


////////////////////////////////////////////////////////////
const he::gfx::geometry::Angle& Shape::getRotation() const
{
    return Transformable2d::getRotation();
}


////////////////////////////////////////////////////////////
const he::gfx::VertexArray2d& Shape::getVertexArray() const
{
    return m_vertexArray;
}


////////////////////////////////////////////////////////////
unsigned int Shape::getTextureId() const
{
    return 0;
}


////////////////////////////////////////////////////////////
void Shape::closeVertexArray()
{
    m_closedVertexArray = true;
    updateVertexArray();
}


////////////////////////////////////////////////////////////
void Shape::openVertexArray()
{
    m_closedVertexArray = false;
    updateVertexArray();
}


////////////////////////////////////////////////////////////
void Shape::draw(he::gfx::render::Render& render, const he::gfx::render::RenderSettings& renderSettings) const
{
    if (not getVertexArray().empty())
    {
        render.drawVertexPrimitive(getVertexArray(), getTextureId(), getColor(), renderSettings);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// PRIVATE
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
void Shape::setOriginPosition(const he::gfx::OriginPosition& originPosition)
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
void Shape::updateVertexArray()
{
    m_vertexArray.clear();

    if (m_figure)
    {
        for (std::size_t i = 0 ; i < m_figure->getNumOfPoints() ; ++i)
        {
            auto point = m_figure->getPoint(i);
            transformPoint(point);
            // TODO: remove 1200, 800 - screen width and height
            convertPixelPointToVertexPoint(point, 1200, 800);
            m_vertexArray.push_back({point, m_context.color});
        }
    }

    if (m_closedVertexArray)
    {
        m_vertexArray.push_back(m_vertexArray[0]);
    }
}

} // namespace draw
} // namespace gfx
} // namespace he