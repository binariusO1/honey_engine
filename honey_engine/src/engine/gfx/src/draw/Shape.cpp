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
bool Shape<POINT, VECTOR, VERTEX>::isPointInside(const geometry::Point2Df& point)
{
    geometry::Point2Df pointToCheck{point};
    IShapeTmpl::inverseTransformPoint(pointToCheck);
    return m_figure.isPointInside(pointToCheck);
}


////////////////////////////////////////////////////////////
template<typename POINT, typename VECTOR, typename VERTEX>
bool Shape<POINT, VECTOR, VERTEX>::setPosition(const POINT& position)
{
    return IShapeTmpl::setPosition(position);
}


////////////////////////////////////////////////////////////
template<>
bool Shape2dFor3d::setPosition(const geometry::Point3Df& position)
{
    IShapeTmpl::setOrigin({m_origin.x, m_origin.y, position.z});
    return IShapeTmpl::setPosition(position);
}



////////////////////////////////////////////////////////////
template<typename POINT, typename VECTOR, typename VERTEX>
bool Shape<POINT, VECTOR, VERTEX>::setOrigin(const POINT& origin)
{
    if (origin == geometry::Point2Df{})
    {
        IShapeTmpl::m_context.originPosition = OriginPosition::leftDown;
    }
    else if(origin == geometry::Point2Df{m_figure.getCenterPoint().x , m_figure.getCenterPoint().y})
    {
        IShapeTmpl::m_context.originPosition = OriginPosition::center;
    }
    else
    {
        IShapeTmpl::m_context.originPosition = OriginPosition::any;
    }

    return IShapeTmpl::setOrigin(origin);
}


////////////////////////////////////////////////////////////
template<>
bool Shape2dFor3d::setOrigin(const geometry::Point3Df& origin)
{
    if (origin == geometry::Point3Df{0.0, 0.0, m_position.z})
    {
        IShapeTmpl::m_context.originPosition = OriginPosition::leftDown;
    }
    else if(origin == geometry::Point3Df{m_figure.getCenterPoint().x, m_figure.getCenterPoint().y, m_position.z})
    {
        IShapeTmpl::m_context.originPosition = OriginPosition::center;
    }
    else
    {
        IShapeTmpl::m_context.originPosition = OriginPosition::any;
    }

    return IShapeTmpl::setOrigin(origin);
}

////////////////////////////////////////////////////////////
template<typename POINT, typename VECTOR, typename VERTEX>
void Shape<POINT, VECTOR, VERTEX>::setOriginInCenter()
{
    IShapeTmpl::m_context.originPosition = he::gfx::OriginPosition::center;
    setOrigin({m_figure.getCenterPoint().x , m_figure.getCenterPoint().y});
}


////////////////////////////////////////////////////////////
template<>
void Shape2dFor3d::setOriginInCenter()
{
    IShapeTmpl::m_context.originPosition = he::gfx::OriginPosition::center;
    setOrigin({m_figure.getCenterPoint().x, m_figure.getCenterPoint().y, m_position.z});
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


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// PROTECTED
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


////////////////////////////////////////////////////////////
template<>
void Shape2dFor3d::updateVertexArray()
{
    IShapeTmpl::m_vertexArray.clear();

    for (std::size_t i = 0 ; i < m_figure.getNumOfPoints() ; ++i)
    {
        const geometry::Point3Df point(m_figure.getPoint(i).x, m_figure.getPoint(i).y, m_position.z);
        IShapeTmpl::m_vertexArray.push_back({point, IShapeTmpl::m_context.color});
    }

    if (m_closedVertexArray)
    {
        IShapeTmpl::m_vertexArray.push_back(IShapeTmpl::m_vertexArray[0]);
    }
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// PUBLIC CD
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
template<typename POINT, typename VECTOR, typename VERTEX> 
void Shape<POINT, VECTOR, VERTEX>::draw(render::Render& render, const render::RenderSettings& renderSettings, render::TransformMatrix& transformMatrix)
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


////////////////////////////////////////////////////////////
template<>
void Shape2dFor3d::draw(render::Render& render, const render::RenderSettings& renderSettings, render::TransformMatrix& transformMatrix)
{
    if (IShapeTmpl::m_vertexArrayNeedUpdate)
    {
        updateVertexArray();
        transformMatrix.isNeedUpdate = IShapeTmpl::m_vertexArrayNeedUpdate;
    }

    if (not IShapeTmpl::m_vertexArray.empty())
    {
        transformMatrix.modelMatrix = IShapeTmpl::getTransform().getMatrix();
        render.drawVertex3d(IShapeTmpl::m_vertexArray, 0, IShapeTmpl::getColor(), renderSettings, transformMatrix);
    }

    IShapeTmpl::m_vertexArrayNeedUpdate = false;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// PRIVATE
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


template class Shape<geometry::Point2Df, geometry::Vector2Df, VertexArray2d>;
template class Shape<geometry::Point3Df, geometry::Vector3Df, VertexArray3d>;

} // namespace draw
} // namespace gfx
} // namespace he