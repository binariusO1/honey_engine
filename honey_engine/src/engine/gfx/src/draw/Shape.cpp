#include "gfx/draw/Shape.hpp"

#include "gfx/render/Render.hpp"

namespace he
{
namespace gfx
{
namespace draw
{
////////////////////////////////////////////////////////////
template<typename POINT, typename VECTOR, typename VERTEX>
Shape<POINT, VECTOR, VERTEX>::Shape(const std::string& name, const geometry::figures::Figure& figure)   
    : IShapeTmpl(name)
    , m_figure{figure}
{
    IShapeTmpl::m_vertexArrayNeedUpdate = true;
}


////////////////////////////////////////////////////////////
template<typename POINT, typename VECTOR, typename VERTEX>
Shape<POINT, VECTOR, VERTEX>::Shape(const Shape& copy, const geometry::figures::Figure& figure)
    : IShapeTmpl(copy)
    , m_figure{figure}
    , m_closedVertexArray{copy.m_closedVertexArray}
{
}


////////////////////////////////////////////////////////////
template<typename POINT, typename VECTOR, typename VERTEX>
Shape<POINT, VECTOR, VERTEX>::~Shape() = default;


////////////////////////////////////////////////////////////
template<typename POINT, typename VECTOR, typename VERTEX>
bool Shape<POINT, VECTOR, VERTEX>::isPointInside(const POINT& point)
{
    POINT pointToCheck{point};
    IShapeTmpl::inverseTransformPoint(pointToCheck);
    return m_figure.isPointInside(pointToCheck);
}


////////////////////////////////////////////////////////////
template<typename POINT, typename VECTOR, typename VERTEX> // TODO : SPECIALIZATION 3D
bool Shape<POINT, VECTOR, VERTEX>::setOrigin(const POINT& origin)
{
    if (origin == he::gfx::geometry::Point2Df{0.0, 0.0})
    {
        IShapeTmpl::m_context.originPosition = he::gfx::OriginPosition::leftDown;
    }
    else if(origin == he::gfx::geometry::Point2Df{m_figure.getCenterPoint().x , m_figure.getCenterPoint().y})
    {
        IShapeTmpl::m_context.originPosition = he::gfx::OriginPosition::center;
    }
    else
    {
        IShapeTmpl::m_context.originPosition = he::gfx::OriginPosition::any;
    }

    return IShapeTmpl::setOrigin(origin);
}


////////////////////////////////////////////////////////////
template<typename POINT, typename VECTOR, typename VERTEX> // TODO : SPECIALIZATION 3D
void Shape<POINT, VECTOR, VERTEX>::setOriginInCenter()
{
    IShapeTmpl::m_context.originPosition = he::gfx::OriginPosition::center;
    setOrigin({m_figure.getCenterPoint().x , m_figure.getCenterPoint().y});
}


////////////////////////////////////////////////////////////
template<typename POINT, typename VECTOR, typename VERTEX>
void Shape<POINT, VECTOR, VERTEX>::closeVertexArray()
{
    m_closedVertexArray = true;
    IShapeTmpl::m_vertexArrayNeedUpdate = true;
}


////////////////////////////////////////////////////////////
template<typename POINT, typename VECTOR, typename VERTEX>
void Shape<POINT, VECTOR, VERTEX>::openVertexArray()
{
    m_closedVertexArray = false;
    IShapeTmpl::m_vertexArrayNeedUpdate = true;
}


////////////////////////////////////////////////////////////
template<typename POINT, typename VECTOR, typename VERTEX> // TODO : SPECIALIZATION 3D
void Shape<POINT, VECTOR, VERTEX>::draw(he::gfx::render::Render& render, const he::gfx::render::RenderSettings& renderSettings, render::TransformMatrix& transformMatrix)
{
    if (IShapeTmpl::m_vertexArrayNeedUpdate)
    {
        updateVertexArray();
        transformMatrix.isNeedUpdate = IShapeTmpl::m_vertexArrayNeedUpdate;
    }

    if (not IShapeTmpl::m_vertexArray.empty())
    {
        transformMatrix.modelMatrix = IShapeTmpl::getTransform().getMatrix();
        render.drawVertex2d(IShapeTmpl::m_vertexArray, 0, IShapeTmpl::getColor(), renderSettings, transformMatrix);
    }

    IShapeTmpl::m_vertexArrayNeedUpdate = false;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// PRIVATE
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
template<typename POINT, typename VECTOR, typename VERTEX>
void Shape<POINT, VECTOR, VERTEX>::updateVertexArray()
{
    IShapeTmpl::m_vertexArray.clear();

    for (std::size_t i = 0 ; i < m_figure.getNumOfPoints() ; ++i)
    {
        IShapeTmpl::m_vertexArray.push_back({m_figure.getPoint(i), IShapeTmpl::m_context.color});
    }

    if (m_closedVertexArray)
    {
        IShapeTmpl::m_vertexArray.push_back(IShapeTmpl::m_vertexArray[0]);
    }
}

template class Shape<geometry::Point2Df, geometry::Vector2Df, he::gfx::VertexArray2d>;

} // namespace draw
} // namespace gfx
} // namespace he