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
    : IShape<geometry::Point2Df, geometry::Vector2Df, he::gfx::VertexArray2d>(name)
    , m_figure{figure}
{
    m_vertexArrayNeedUpdate = true;
}


////////////////////////////////////////////////////////////
Shape::Shape(const Shape& copy, const geometry::figures::Figure& figure)
    : IShape<geometry::Point2Df, geometry::Vector2Df, he::gfx::VertexArray2d>(copy)
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

    return IShape<geometry::Point2Df, geometry::Vector2Df, he::gfx::VertexArray2d>::setOrigin(origin);
}


////////////////////////////////////////////////////////////
void Shape::setOriginInCenter()
{
    m_context.originPosition = he::gfx::OriginPosition::center;
    setOrigin({m_figure.getCenterPoint().x , m_figure.getCenterPoint().y});
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
void Shape::draw(he::gfx::render::Render& render, const he::gfx::render::RenderSettings& renderSettings, render::TransformMatrix& transformMatrix)
{
    if (m_vertexArrayNeedUpdate)
    {
        updateVertexArray();
        transformMatrix.isNeedUpdate = m_vertexArrayNeedUpdate;
    }

    if (not m_vertexArray.empty())
    {
        transformMatrix.modelMatrix = getTransform().getMatrix();
        render.drawVertex2d(m_vertexArray, 0, getColor(), renderSettings, transformMatrix);
    }

    m_vertexArrayNeedUpdate = false;
}


////////////////////////////////////////////////////////////
void Shape::setOriginPosition(const he::gfx::OriginPosition& originPosition)
{
    IShape<geometry::Point2Df, geometry::Vector2Df, he::gfx::VertexArray2d>::setOriginPosition(originPosition);
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